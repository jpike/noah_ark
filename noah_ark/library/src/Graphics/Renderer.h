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
        explicit Renderer(const std::shared_ptr<sf::RenderTarget>& render_target);

        // RENDERING.
        void Render(
            const float elapsed_time_in_seconds,
            MAPS::Overworld& overworld, 
            sf::RenderTarget& render_target);
        void Render(const MAPS::TileMap& tile_map, sf::RenderTarget& render_target);
        void Render(const GRAPHICS::Sprite& sprite, sf::RenderTarget& render_target);
        void Render(const GRAPHICS::AnimatedSprite& sprite, sf::RenderTarget& render_target);
        void Render(const GRAPHICS::GUI::HeadsUpDisplay& hud, sf::RenderTarget& render_target);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The camera defining what portion of the world is currently viewable.
        GRAPHICS::Camera Camera;
    };
}
