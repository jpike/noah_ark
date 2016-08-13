#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Camera.h"
#include "Graphics/Color.h"
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Gui/Text.h"
#include "Graphics/Screen.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Maps/Overworld.h"
#include "Maps/TileMap.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"

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

        // STATIC RENDERING.
        static void RenderScreenRectangle(
            const MATH::FloatRectangle& rectangle,
            const GRAPHICS::Color& color,
            Screen& screen);
        static void Render(const GRAPHICS::GUI::Text& text, Screen& screen);
        static void RenderKeyIcon(
            const char key,
            const GRAPHICS::GUI::Font& font,
            const MATH::Vector2ui& top_left_screen_position_in_pixels,
            Screen& screen);
        static void RenderGuiIcon(
            const GRAPHICS::Texture& texture,
            const MATH::FloatRectangle& texture_sub_rectangle,
            const MATH::Vector2ui& top_left_screen_position_in_pixels,
            Screen& screen);

        // RENDERING.
        void Render(
            const float elapsed_time_in_seconds,
            MAPS::Overworld& overworld, 
            Screen& screen);
        void Render(const MAPS::TileMap& tile_map, Screen& screen);
        void Render(const GRAPHICS::Sprite& sprite, Screen& screen);
        void Render(const GRAPHICS::AnimatedSprite& sprite, Screen& screen);
        void Render(const GRAPHICS::GUI::HeadsUpDisplay& hud, Screen& screen);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The camera defining what portion of the world is currently viewable.
        GRAPHICS::Camera Camera;
    };
}
