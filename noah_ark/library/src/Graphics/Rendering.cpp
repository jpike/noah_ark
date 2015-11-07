#include "Graphics/Rendering.h"

namespace GRAPHICS
{
    void Render(const GRAPHICS::Sprite& sprite, sf::RenderTarget& render_target)
    {
        sprite.Render(render_target);
    }

    void Render(const GRAPHICS::AnimatedSprite& sprite, sf::RenderTarget& render_target)
    {
        sprite.Render(render_target);
    }

    void Render(const MAPS::TileMap& tile_map, sf::RenderTarget& render_target)
    {
        // RENDER THE CURRENT TILE MAP'S GROUND LAYER.
        MATH::Vector2ui ground_dimensions_in_tiles = tile_map.GetDimensionsInTiles();
        for (unsigned int tile_row = 0; tile_row < ground_dimensions_in_tiles.Y; ++tile_row)
        {
            for (unsigned int tile_column = 0; tile_column < ground_dimensions_in_tiles.X; ++tile_column)
            {
                const std::shared_ptr<MAPS::Tile>& tile = tile_map.Ground.Tiles(tile_column, tile_row);
                Render(tile->Sprite, render_target);
            }
        }

        // RENDER THE CURRENT TILE MAP'S TREES.
        for (const auto& tree : tile_map.Trees)
        {
            Render(tree.Sprite, render_target);
        }
    }
}
