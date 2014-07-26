#include <algorithm>
#include "Graphics/GraphicsSystem.h"

using namespace GRAPHICS;

// STATIC CLASS CONSTANT INITIALIZION.
const float GraphicsSystem::GROUND_LAYER_Z_VALUE = 0.9f;
const float GraphicsSystem::PLAYER_LAYER_Z_VALUE = 0.8f;

GraphicsSystem::GraphicsSystem(HGE* pGameEngine, const std::shared_ptr<hgeResourceManager>& resourceManager) :
    m_pGameEngine(pGameEngine),
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
    const float CENTER_X_POSITION = 0.0f;
    const float CENTER_Y_POSITION = 0.0f;
    const float NO_ROTATION = 0.0f;
    // The displacements need to be flipped for the transformation because the
    // camera's view represents absolute world positions for the top-left corner,
    // and we want to translate the graphics at this position so that they match
    // up to 0,0 on the screen.  For example, if the camera wants to view the world
    // at coordinates 100,100, then we need to translate all graphics being rendered
    // so that 100,100 (world) becomes 0,0 (screen).
    float xDisplacement = -1.0f * m_camera.GetLeftViewPosition();
    float yDisplacement = -1.0f * m_camera.GetTopViewPosition();
    // Note that in order to actually move the camera, we need to specify
    // the scale.  Otherwise, "0" will be used, which will prevent any
    // actual transformations from occurring.
    const float NORMAL_HORIZONTAL_SCALE = 1.0f;
    const float NORMAL_VERTICAL_SCALE = 1.0f;
    m_pGameEngine->Gfx_SetTransform(
        CENTER_X_POSITION,
        CENTER_Y_POSITION,
        xDisplacement,
        yDisplacement,
        NO_ROTATION,
        NORMAL_HORIZONTAL_SCALE,
        NORMAL_VERTICAL_SCALE);
    
    // RENDER ALL OF THE VISIBLE GRAPHICS COMPONENTS.
    for (std::weak_ptr<IGraphicsComponent>& graphicsComponentWeakReference : m_graphicsComponents)
    {
        // Check if the graphics component is still in use.
        if (!graphicsComponentWeakReference.expired())
        {
            // Render the current graphics component.
            std::shared_ptr<IGraphicsComponent> graphicsComponent = graphicsComponentWeakReference.lock();
            RenderIfVisible(graphicsComponent);
        }
    }

    // RESET THE CAMERA'S VIEW.
    m_pGameEngine->Gfx_SetTransform();
}

void GraphicsSystem::Update(const float elapsedTimeInSeconds)
{
    // UPDATE ALL GRAPHICS COMPONENTS.
    for (std::weak_ptr<IGraphicsComponent>& graphicsComponentWeakReference : m_graphicsComponents)
    {
        // Check if the graphics component is still in use.
        if (!graphicsComponentWeakReference.expired())
        {        
            // Update the current graphics component.
            std::shared_ptr<IGraphicsComponent> graphicsComponent = graphicsComponentWeakReference.lock();
            graphicsComponent->Update(elapsedTimeInSeconds);
        }
    }

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
    // GET THE HGE TEXTURE RESOURCE.
    HTEXTURE hgeTexture = m_resourceManager->GetTexture(textureResourceName.c_str());
    bool textureResourceRetrieved = (Texture::INVALID_TEXTURE_HANDLE != hgeTexture);
    if (!textureResourceRetrieved)
    {
        return nullptr;
    }

    // CONVERT THE HGE TEXTURE RESOURCE TO A CUSTOM TEXTURE OBJECT.
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(hgeTexture);
    return texture;
}

std::shared_ptr<AnimationSequence> GraphicsSystem::GetAnimationSequence(const std::string& animationResourceName)
{
    // GET THE HGE ANIMATION RESOURCE.
    hgeAnimation* pAnimation = m_resourceManager->GetAnimation(animationResourceName.c_str());
    bool animationResourceRetrieved = (nullptr != pAnimation);
    if (!animationResourceRetrieved)
    {
        return nullptr;
    }

    // CONVERT THE HGE TEXTURE RESOURCE TO A CUSTOM ANIMATION SEQUENCE OBJECT.
    std::shared_ptr<AnimationSequence> animationSequence = std::make_shared<AnimationSequence>(pAnimation);
    return animationSequence;
}

std::shared_ptr<Sprite> GraphicsSystem::CreateSprite(
    const std::shared_ptr<Texture>& texture,
    const float xPositionInTexels,
    const float yPositionInTexels,
    const float widthInTexels,
    const float heightInTexels)
{  
    // CREATE THE HGE SPRITE.
    // Get the HGE texture resource for the HGE sprite.
    HTEXTURE hgeTexture = Texture::INVALID_TEXTURE_HANDLE;
    bool textureResourceExists = ( nullptr != texture.get() );
    if (textureResourceExists)
    {
        hgeTexture = texture->GetTextureHandle();
    }

    // Create the HGE sprite.
    std::shared_ptr<hgeSprite> spriteResource = std::make_shared<hgeSprite>(
        hgeTexture, 
        xPositionInTexels,
        yPositionInTexels,
        widthInTexels,
        heightInTexels);
    
    // CREATE A NEW SPRITE MANAGED BY THIS SYSTEM.
    std::shared_ptr<Sprite> newSprite = std::make_shared<Sprite>(spriteResource);

    m_graphicsComponents.push_back(newSprite);

    return newSprite;
}

std::shared_ptr<AnimatedSprite> GraphicsSystem::CreateAnimatedSprite()
{
    // CREATE A NEW ANIMATED SPRITE MANAGED BY THIS SYSTEM.
    std::shared_ptr<AnimatedSprite> newAnimatedSprite = std::make_shared<AnimatedSprite>();

    m_graphicsComponents.push_back(newAnimatedSprite);

    return newAnimatedSprite;
}

void GraphicsSystem::RenderIfVisible(std::shared_ptr<IGraphicsComponent>& graphicsComponent)
{
    if (graphicsComponent->IsVisible())
    {
        graphicsComponent->Render();
    }
}

void GraphicsSystem::RemoveUnusedGraphicsComponents()
{
    // Remove any graphics components that are no longer being used.
    m_graphicsComponents.remove_if(
        [](std::weak_ptr<IGraphicsComponent>& graphicsComponent) { return graphicsComponent.expired(); });
}