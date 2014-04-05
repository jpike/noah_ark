#include "Graphics/GraphicsSystem.h"

using namespace GRAPHICS;

GraphicsSystem::GraphicsSystem(const std::shared_ptr<hgeResourceManager>& resourceManager) :
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
    // RENDER ALL OF THE VISIBLE GRAPHICS COMPONENTS.
    for (std::shared_ptr<IGraphicsComponent>& graphicsComponent : m_graphicsComponents)
    {
        RenderIfVisible(graphicsComponent);
    }
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

void GraphicsSystem::RenderIfVisible(std::shared_ptr<IGraphicsComponent>& graphicsComponent)
{
    if (graphicsComponent->IsVisible())
    {
        graphicsComponent->Render();
    }
}