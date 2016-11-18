#pragma once

#include <memory>
#include <string>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Camera.h"
#include "Graphics/Color.h"
#include "Graphics/Gui/Font.h"
/// @todo #include "Graphics/Gui/Text.h"
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
    class Renderer
    {
    public:
        // CONSTRUCTION.
        explicit Renderer(const MATH::FloatRectangle& camera_view_bounds);

        // STATIC RENDERING.
        static void RenderScreenRectangle(
            const MATH::FloatRectangle& rectangle,
            const GRAPHICS::Color& color,
            Screen& screen);
        /// @todo static void Render(const GRAPHICS::GUI::Text& text, Screen& screen);
        /// @todo   Move this method since it is no longer static.  In fact, remove all static methods?
        void RenderKeyIcon(
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
        //void Render(const GRAPHICS::GUI::HeadsUpDisplay& hud, Screen& screen);
        void RenderText(
            const std::string& text, 
            const MATH::Vector2f& left_top_screen_position_in_pixels,
            const Color& text_color,
            Screen& screen);
        void RenderText(
            const std::string& text,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            const Color& text_color,
            Screen& screen);
        void RenderCenteredText(
            const std::string& text,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            const Color& text_color,
            Screen& screen);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The camera defining what portion of the world is currently viewable.
        GRAPHICS::Camera Camera;
        /// The font to use for rendering text.
        std::shared_ptr<GRAPHICS::GUI::Font> Font;
        /// The shader to use for colored text.
        sf::Shader ColoredTextShader;

    private:
        // RENDERING.
        void Render(const MAPS::TileMap& tile_map, Screen& screen);
        void Render(const GRAPHICS::Sprite& sprite, Screen& screen);
        void Render(const GRAPHICS::AnimatedSprite& sprite, Screen& screen);
    };
}
