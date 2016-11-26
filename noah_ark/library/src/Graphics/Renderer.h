#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Camera.h"
#include "Graphics/Color.h"
#include "Graphics/Gui/Font.h"
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
        explicit Renderer(
            const std::shared_ptr<sf::RenderTarget>& render_target,
            const std::shared_ptr<GRAPHICS::GUI::Font>& font,
            const std::shared_ptr<sf::Shader>& colored_text_shader);

        // GENERIC SCREEN-SPACE RENDERING.
        void RenderScreenRectangle(
            const MATH::FloatRectangle& rectangle,
            const GRAPHICS::Color& color);
        void RenderKeyIcon(
            const char key,
            const MATH::Vector2ui& top_left_screen_position_in_pixels);
        void RenderGuiIcon(
            const GRAPHICS::Texture& texture,
            const MATH::FloatRectangle& texture_sub_rectangle,
            const MATH::Vector2ui& top_left_screen_position_in_pixels);

        // TEXT RENDERING.
        void RenderText(
            const std::string& text, 
            const MATH::Vector2f& left_top_screen_position_in_pixels,
            const Color& text_color = GRAPHICS::Color::BLACK,
            const float text_scale_ratio = 1.0f);
        void RenderText(
            const std::string& text,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            const Color& text_color = GRAPHICS::Color::BLACK,
            const float text_scale_ratio = 1.0f);
        void RenderCenteredText(
            const std::string& text,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            const Color& text_color = GRAPHICS::Color::BLACK,
            const float text_scale_ratio = 1.0f);

        // OTHER RENDERING.
        void Render(const MAPS::Overworld& overworld);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The screen that gets rendered to.
        GRAPHICS::Screen Screen;
        /// The camera defining what portion of the world is currently viewable.
        GRAPHICS::Camera Camera;
        /// The font to use for rendering text.
        std::shared_ptr<GRAPHICS::GUI::Font> Font;
        /// The shader to use for colored text.
        std::shared_ptr<sf::Shader> ColoredTextShader;

    private:
        // RENDERING.
        void Render(const MAPS::TileMap& tile_map);

        // SHADERS.
        sf::RenderStates ConfigureColoredTextShader(const Color& color);
    };
}
