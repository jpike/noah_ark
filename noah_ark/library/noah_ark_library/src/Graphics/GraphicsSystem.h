#pragma once

#include <list>
#include <memory>
#include "Graphics/IGraphicsComponent.h"
#include "Graphics/Sprite.h"

/// A namespace for graphics code.
namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// \brief  The graphics system for the game.  Manages and renders
    ///         graphics components.
    ///////////////////////////////////////////////////////////
    class GraphicsSystem
    {
    public:
        /// @brief  Constructor.
        explicit GraphicsSystem();
        /// @brief  Destructor.
        ~GraphicsSystem();

        /// @brief  Renders all visible graphics components managed by this system.
        ///         Components are rendered in order of creation within the graphics
        ///         system and relies on z-buffering to handle certains components
        ///         being in back/front of others.  Therefore, you should make
        ///         sure that z values have been properly set for all components objects.
        void Render();

        /// @brief  Creates a new sprite within the graphics system.
        /// @return The newly created sprite.
        std::shared_ptr<Sprite> CreateSprite();

    private:
        GraphicsSystem(const GraphicsSystem& systemToCopy); ///< Private to disallow copying.
        GraphicsSystem& operator= (const GraphicsSystem& rhsSystem);    ///< Private to disallow assignment.

        /// @brief      Renders the provided graphics component if it is visible.
        /// @param[in]  graphicsComponent - The graphics component to render.
        void RenderIfVisible(std::shared_ptr<IGraphicsComponent>& graphicsComponent);

        std::list< std::shared_ptr<IGraphicsComponent> > m_graphicsComponents;    ///< The graphics components to be rendered.
    };
}