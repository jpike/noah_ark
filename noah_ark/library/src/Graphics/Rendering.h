#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Sprite.h"
#include "Graphics/Text.h"
#include "Maps/TileMap.h"

namespace GRAPHICS
{
    void Render(const GRAPHICS::Sprite& sprite, sf::RenderTarget& render_target);

    void Render(const GRAPHICS::AnimatedSprite& sprite, sf::RenderTarget& render_target);

    void Render(const GRAPHICS::Text& text, sf::RenderTarget& render_target);

    void Render(const MAPS::TileMap& tile_map, sf::RenderTarget& render_target);
}
