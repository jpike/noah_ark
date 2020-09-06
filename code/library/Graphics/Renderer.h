#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Camera.h"
#include "Graphics/Color.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/Text.h"
#include "Graphics/Screen.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Maps/MultiTileMapGrid.h"
#include "Maps/TileMap.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"
#include "Resources/AssetId.h"

namespace GRAPHICS
{
    /// The main renderer for the game.
    class Renderer
    {
    public:
        // FINAL RENDER RETRIEVAL.
        sf::Sprite RenderFinalScreen(const sf::RenderStates& render_settings = sf::RenderStates::Default);

        // LINE RENDERING.
        void RenderLine(const MATH::Vector2f& start_position, const MATH::Vector2f& end_position, const GRAPHICS::Color& color);

        // GENERIC SCREEN-SPACE RENDERING.
        void RenderScreenRectangle(
            const MATH::FloatRectangle& rectangle,
            const GRAPHICS::Color& color);
        void RenderKeyIcon(
            const char key,
            const MATH::Vector2ui& left_top_screen_position_in_pixels);
        void RenderGuiIcon(
            const RESOURCES::AssetId texture_id,
            const MATH::FloatRectangle& texture_sub_rectangle,
            const MATH::Vector2ui& left_top_screen_position_in_pixels);
        void RenderGuiIcon(
            const GRAPHICS::Sprite& sprite,
            const MATH::Vector2f& left_top_screen_position_in_pixels);

        // TEXT RENDERING.
        void Render(const Sprite& sprite);
        void Render(const GUI::Text& text);
        void RenderText(
            const std::string& text, 
            const RESOURCES::AssetId font_id,
            const MATH::Vector2f& left_top_screen_position_in_pixels,
            const Color& text_color = GRAPHICS::Color::BLACK,
            const float text_scale_ratio = 1.0f);
        void RenderText(
            const std::string& text,
            const RESOURCES::AssetId font_id,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            const Color& text_color = GRAPHICS::Color::BLACK,
            const float text_scale_ratio = 1.0f);
        void RenderCenteredText(
            const std::string& text,
            const RESOURCES::AssetId font_id,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            const Color& text_color = GRAPHICS::Color::BLACK,
            const float text_scale_ratio = 1.0f);
        void RenderTextInBorderedBox(
            const std::string& text,
            const RESOURCES::AssetId font_id,
            const Color& text_color,
            const MATH::Vector2f& text_padding_in_pixels_from_border,
            const MATH::FloatRectangle& bounding_screen_rectangle,
            const Color& border_color,
            const float border_thickness_in_pixels = 1.0f);

        // OTHER RENDERING.
        void Render(const MAPS::MultiTileMapGrid& tile_map_grid);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphics device to use for rendering.
        std::shared_ptr<GRAPHICS::GraphicsDevice> GraphicsDevice = nullptr;
        /// The screen that gets rendered to.
        std::shared_ptr<GRAPHICS::Screen> Screen = nullptr;
        /// The camera defining what portion of the world is currently viewable.
        GRAPHICS::Camera Camera = GRAPHICS::Camera(MATH::FloatRectangle());
        /// Fonts to use for rendering text.
        std::unordered_map<RESOURCES::AssetId, std::shared_ptr<GRAPHICS::GUI::Font>> Fonts = {};

    private:
        // RENDERING.
        void Render(const MAPS::TileMap& tile_map);

        // SHADERS.
        sf::RenderStates ConfigureColoredTextShader(const Color& color, sf::Shader& colored_text_shader);
    };
}
