#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Color.h"
#include "Graphics/Sprite.h"
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/Text.h"
#include "Maps/TileMap.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
    void Render(const GRAPHICS::Sprite& sprite, sf::RenderTarget& render_target);

    void Render(const GRAPHICS::AnimatedSprite& sprite, sf::RenderTarget& render_target);

    void RenderScreenRectangle(
        const MATH::FloatRectangle& rectangle,
        const GRAPHICS::Color& color,
        sf::RenderTarget& render_target);

    void Render(const GRAPHICS::GUI::Text& text, sf::RenderTarget& render_target);

    void RenderKeyIcon(
        const char key, 
        const GRAPHICS::GUI::Font& font, 
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        sf::RenderTarget& render_target);

    void RenderGuiIcon(
        const GRAPHICS::Texture& texture, 
        const MATH::FloatRectangle& texture_sub_rectangle, 
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        sf::RenderTarget& render_target);

    void Render(const MAPS::TileMap& tile_map, sf::RenderTarget& render_target);
}
