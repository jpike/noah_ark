#include <algorithm>
#include "Graphics/GraphicsSystem.h"

using namespace GRAPHICS;

GraphicsSystem::GraphicsSystem(
    const std::shared_ptr<sf::RenderTarget>& renderTarget,
    const std::shared_ptr<RESOURCES::ResourceManager>& resourceManager) :
    m_renderTarget(renderTarget),
    m_camera(),
    m_resourceManager(resourceManager),
    m_graphicsComponents()
{
    // Nothing else to do.
}

GraphicsSystem::~GraphicsSystem()
{
    // Nothing else to do.
}

void GraphicsSystem::Render()
{
    // SET THE CAMERA'S VIEW.
    // Get the old view of the render target so we can restore it later.
    sf::View originalView = m_renderTarget->getView();

    // Calculate the new center position.
    sf::Vector2f originalViewCenter = originalView.getCenter();
    // The displacements need to be flipped for the transformation because the
    // camera's view represents absolute world positions for the top-left corner,
    // and we want to translate the graphics at this position so that they match
    // up to 0,0 on the screen.  For example, if the camera wants to view the world
    // at coordinates 100,100, then we need to translate all graphics being rendered
    // so that 100,100 (world) becomes 0,0 (screen).
    /// @todo   This camera view logic needs to be re-verified.  SFML has a nicer
    ///         interface than HGE, so we can probably re-work the camera class to
    ///         take advantage of this.
    float newCenterXPosition = 1.0f * m_camera.GetLeftViewPosition() + originalViewCenter.x;
    float newCenterYPosition = 1.0f * m_camera.GetTopViewPosition() + originalViewCenter.y;

    // No rotation is needed.
    const float NO_ROTATION = 0.0f;

    // Set the new view from the camera.
    sf::View cameraView;
    cameraView.setCenter(newCenterXPosition, newCenterYPosition);
    cameraView.setRotation(NO_ROTATION);
    cameraView.setSize(originalView.getSize());
    m_renderTarget->setView(cameraView);
    
    // DEFINE A LAMBDA FUNCTION FOR RENDERING ONLY GRAPHICS COMPONENTS THAT ARE STILL IN-USE.
    auto RenderIfInUseAndVisible = [&](const std::pair< GraphicsLayer, std::weak_ptr<IGraphicsComponent> >& graphicsComponentWeakReference)
    {
        // Check if the graphics component is still in use.
        if (!graphicsComponentWeakReference.second.expired())
        {
            // Render the current graphics component.
            std::shared_ptr<IGraphicsComponent> graphicsComponent = graphicsComponentWeakReference.second.lock();
            RenderIfVisible(graphicsComponent, *m_renderTarget);
        }
    };

    // RENDER ALL GRAPHICS IN THE GROUND LAYER.
    auto& groundComponents = m_graphicsComponents.equal_range(GraphicsLayer::GROUND);
    std::for_each(groundComponents.first, groundComponents.second, RenderIfInUseAndVisible);

    // RENDER ALL GRAPHICS IN THE PLAYER LAYER.
    auto& playerComponents = m_graphicsComponents.equal_range(GraphicsLayer::PLAYER);
    std::for_each(playerComponents.first, playerComponents.second, RenderIfInUseAndVisible);

    // RESET THE RENDER TARGET'S VIEW.
    m_renderTarget->setView(originalView);
}

void GraphicsSystem::Update(const float elapsedTimeInSeconds)
{
    // DEFINE A LAMBDA FUNCTION FOR UPDATING ONLY GRAPHICS COMPONENTS THAT ARE STILL IN-USE.
    auto UpdateIfInUse = [&](const std::pair< GraphicsLayer, std::weak_ptr<IGraphicsComponent> >& graphicsComponentWeakReference)
    {
        // Check if the graphics component is still in use.
        if (!graphicsComponentWeakReference.second.expired())
        {
            // Update the current graphics component.
            std::shared_ptr<IGraphicsComponent> graphicsComponent = graphicsComponentWeakReference.second.lock();
            graphicsComponent->Update(elapsedTimeInSeconds);
        }
    };
    
    // UPDATE ALL GRAPHICS COMPONENTS IN THE GROUND LAYER.
    auto& groundComponents = m_graphicsComponents.equal_range(GraphicsLayer::GROUND);
    std::for_each(groundComponents.first, groundComponents.second, UpdateIfInUse);

    // UPDATE ALL GRAPHICS IN THE PLAYER LAYER.
    auto& playerComponents = m_graphicsComponents.equal_range(GraphicsLayer::PLAYER);
    std::for_each(playerComponents.first, playerComponents.second, UpdateIfInUse);

    // REMOVE ANY GRAPHICS COMPONENTS THAT ARE NO LONGER NEEDED.
    // This is done here, rather than in the render call, because more overall time
    // is spent in the render call than the update call, so this helps balance things out.
    RemoveUnusedGraphicsComponents();
}

void GraphicsSystem::SetCamera(const Camera& camera)
{
    m_camera = camera;
}

std::shared_ptr<Texture> GraphicsSystem::GetTexture(const std::string& textureResourceName)
{
    // GET THE SFML TEXTURE RESOURCE.
    std::shared_ptr<sf::Texture> sfmlTexture = m_resourceManager->GetTexture(textureResourceName.c_str());
    bool textureResourceRetrieved = (nullptr != sfmlTexture);
    if (!textureResourceRetrieved)
    {
        return nullptr;
    }

    // CONVERT THE SFML TEXTURE RESOURCE TO A CUSTOM TEXTURE OBJECT.
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(sfmlTexture);
    return texture;
}

std::shared_ptr<AnimationSequence> GraphicsSystem::GetAnimationSequence(const std::string& animationResourceName)
{
    // GET THE HGE ANIMATION RESOURCE.
    std::shared_ptr<AnimationSequence> animationSequence = m_resourceManager->GetAnimation(animationResourceName.c_str());
    bool animationResourceRetrieved = (nullptr != animationSequence);
    if (!animationResourceRetrieved)
    {
        return nullptr;
    }

    return animationSequence;
}

std::shared_ptr<Sprite> GraphicsSystem::CreateSprite(
    const std::shared_ptr<Texture>& texture,
    const float xPositionInTexels,
    const float yPositionInTexels,
    const float widthInTexels,
    const float heightInTexels,
    const GraphicsLayer graphicsLayer)
{  
    // CREATE THE SFML SPRITE.
    // Get the SFML texture resource for the SFML sprite.
    std::shared_ptr<sf::Texture> textureResource = texture->GetTextureResource();
    bool textureResourceExists = ( nullptr != texture.get() );
    if (!textureResourceExists)
    {
        return nullptr;
    }

    // Create the rectangle specifying which portion of the texture the
    // sprite image should come from.
    sf::IntRect textureRectangle(
        static_cast<int>(xPositionInTexels),
        static_cast<int>(yPositionInTexels),
        static_cast<int>(widthInTexels),
        static_cast<int>(heightInTexels));
    
    // Create the SFML sprite.
    std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>(
        *textureResource, 
        textureRectangle);
    
    // CREATE A NEW SPRITE MANAGED BY THIS SYSTEM.
    std::shared_ptr<Sprite> newSprite = std::make_shared<Sprite>(spriteResource);

    m_graphicsComponents.insert(
        std::pair< GraphicsLayer, std::weak_ptr<IGraphicsComponent> >(graphicsLayer, newSprite));

    return newSprite;
}

std::shared_ptr<AnimatedSprite> GraphicsSystem::CreateAnimatedSprite(
    const std::string& spriteResourceName,
    const GraphicsLayer graphicsLayer)
{
    // LOAD THE SPECIFIED SPRITE RESOURCE.
    std::shared_ptr<sf::Sprite> spriteResource = m_resourceManager->GetSprite(spriteResourceName);
    bool spriteResourceExists = (nullptr != spriteResource);
    if (!spriteResourceExists)
    {
        return nullptr;
    }
    
    // CREATE A NEW ANIMATED SPRITE MANAGED BY THIS SYSTEM.
    std::shared_ptr<AnimatedSprite> newAnimatedSprite = std::make_shared<AnimatedSprite>(spriteResource);

    m_graphicsComponents.insert(
        std::pair< GraphicsLayer, std::weak_ptr<IGraphicsComponent> >(graphicsLayer, newAnimatedSprite));

    return newAnimatedSprite;
}

void GraphicsSystem::RenderIfVisible(std::shared_ptr<IGraphicsComponent>& graphicsComponent, sf::RenderTarget& renderTarget)
{
    if (graphicsComponent->IsVisible())
    {
        graphicsComponent->Render(renderTarget);
    }
}

void GraphicsSystem::RemoveUnusedGraphicsComponents()
{
    // DEFINE A LAMBDA FUNCTION FOR ERASING UNUSED GRAPHICS COMPONENTS.
    typedef std::unordered_map< GraphicsLayer, std::weak_ptr<IGraphicsComponent> >::iterator LayeredGraphicsComponentIterator;
    auto EraseUnusedComponents = [&](std::pair< LayeredGraphicsComponentIterator, LayeredGraphicsComponentIterator >& graphicsComponents)
    {
        // ERASE ALL UNUSED GRAPHICS COMPONENTS IN THE RANGE.
        for (auto graphicsComponent = graphicsComponents.first; graphicsComponent != graphicsComponents.second; )
        {
            // Check if the graphics component is no longer in use.
            if (graphicsComponent->second.expired())
            {
                // Erase the unused graphics component.
                graphicsComponent = m_graphicsComponents.erase(graphicsComponent);
            }
            else
            {
                // Move the next graphics component.
                ++graphicsComponent;
            }
        }
    };

    // REMOVE ANY GRAPHICS COMPONENTS IN THE GROUND LAYER THAT ARE NO LONGER BEING USED.
    auto groundComponents = m_graphicsComponents.equal_range(GraphicsLayer::GROUND);
    EraseUnusedComponents(groundComponents);

    // REMOVE ANY GRAPHICS COMPONENTS IN THE PLAYER LAYER THAT ARE NO LONGER BEING USED.
    auto playerComponents = m_graphicsComponents.equal_range(GraphicsLayer::PLAYER);
    EraseUnusedComponents(playerComponents);
}