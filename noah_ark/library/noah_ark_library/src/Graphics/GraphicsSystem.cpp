#include "Graphics/GraphicsSystem.h"

using namespace GRAPHICS;

GraphicsSystem::GraphicsSystem() :
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
    /// @todo
    // RENDER ALL OF THE VISIBLE GRAPHICS OBJECTS.
    // For each graphics object
        // Is the object visible?
            // Yes? -> Render
}

std::shared_ptr<Sprite> GraphicsSystem::CreateSprite()
{
    /// @todo
    return std::shared_ptr<Sprite>();
}

void GraphicsSystem::RenderIfVisible(std::shared_ptr<IGraphicsComponent>& graphicsComponent)
{
    /// @todo
}