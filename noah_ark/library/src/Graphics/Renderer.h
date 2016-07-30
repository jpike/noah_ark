#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Camera.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Sprite.h"
#include "Maps/Overworld.h"
#include "Maps/TileMap.h"

namespace GRAPHICS
{
    /// The main renderer for the game.
    /// @todo   This class has been designed to just hold all basic rendering functionality
    /// so far.  It might be possible to move this functionality to some other well-named
    /// classes, but the concept of a "renderer" seems well-defined enough to have this
    /// class for now.
    class Renderer
    {
    public:
        // CONSTRUCTION.
        /// Constructor.
        /// @param[in]  render_target - The target to render to.
        ///     @todo   Try and decouple the camera from the render target
        ///     so that it doesn't have to be passed here.
        explicit Renderer(const std::shared_ptr<sf::RenderTarget>& render_target);

        // RENDERING.
        /// Renders an overworld.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time since the last rendering of the world.
        ///     @todo   Look at ways to remove this parameter.
        /// @param[in]  overworld - The overworld to render.
        ///     @todo   Look at how to make overworld parameter const.
        /// @param[in,out]  render_target - The target to render to.
        void Render(
            const float elapsed_time_in_seconds,
            MAPS::Overworld& overworld, 
            sf::RenderTarget& render_target);
        /// Renders a tile map.
        /// @param[in]  tile_map - The tile map to render.
        /// @param[in,out]  render_target - The target to render to.
        void Render(const MAPS::TileMap& tile_map, sf::RenderTarget& render_target);
        /// Renders a sprite.
        /// @param[in]  sprite - The sprite to render.
        /// @param[in,out]  render_target - The target to render to.
        void Render(const GRAPHICS::Sprite& sprite, sf::RenderTarget& render_target);
        /// Renders an animated sprite.
        /// @param[in]  sprite - The animated sprite to render.
        /// @param[in,out]  render_target - The target to render to.
        void Render(const GRAPHICS::AnimatedSprite& sprite, sf::RenderTarget& render_target);
        /// Renders a HUD.
        /// @param[in]  hud - The HUD to render.
        /// @param[in,out]  render_target - The target to render to.
        void Render(const GRAPHICS::GUI::HeadsUpDisplay& hud, sf::RenderTarget& render_target);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The camera defining what portion of the world is currently viewable.
        GRAPHICS::Camera Camera;
    };
}
