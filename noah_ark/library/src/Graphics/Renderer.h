#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/Camera.h"
#include "Graphics/RenderBatch.h"

namespace GRAPHICS
{
    /// Responsible for rendering graphics to a 2D target.
    class Renderer
    {
    public:
        // CONSTRUCTION.
        /// Default constructor.
        explicit Renderer();

        // OTHER METHODS.
        /// Renders a batch of graphics to a render target.
        /// @param[in,out]  graphics_to_render - The graphics to render.
        ///     Graphics are removed from the batch as they are rendered.
        /// @param[in,out]  render_target - The target to render to.
        void Render(
            RenderBatch& graphics_to_render,
            sf::RenderTarget& render_target) const;

        // PUBLIC FIELDS FOR EASY ACCESS.
        /// The camera used for rendering.
        GRAPHICS::Camera Camera;

    };
}