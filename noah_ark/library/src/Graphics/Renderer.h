#pragma once

#include <memory>
#include <string>
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
        void Render(const GRAPHICS::GUI::HeadsUpDisplay& hud, Screen& screen);
        void RenderText(
            const std::string& text, 
            const MATH::Vector2f& left_top_screen_position_in_pixels,
            Screen& screen) const;
        void RenderText(
            const std::string& text,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            Screen& screen) const;
        void RenderCenteredText(
            const std::string& text,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            Screen& screen) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The camera defining what portion of the world is currently viewable.
        GRAPHICS::Camera Camera;
        /// The font to use for rendering text.
        std::shared_ptr<GRAPHICS::GUI::Font> Font;

    private:
        // RENDERING.
        void Render(const MAPS::TileMap& tile_map, Screen& screen);
        void Render(const GRAPHICS::Sprite& sprite, Screen& screen);
        void Render(const GRAPHICS::AnimatedSprite& sprite, Screen& screen);
    };
}
