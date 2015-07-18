#include <iostream>
#include "World/GroundLayer.h"

namespace WORLD
{
    GroundLayer::GroundLayer(
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles) :
    CenterWorldPosition(center_world_position),
    /// @todo   Maybe we don't need to store the dimensions separately
    /// if they'll automatically be included in the Tiles array?
    DimensionsInTiles(dimensions_in_tiles),
    Tiles(dimensions_in_tiles.X, dimensions_in_tiles.Y)
    {}

    void GroundLayer::SetTile(
        const unsigned int x_offset_from_left_in_tiles,
        const unsigned int y_offset_from_top_in_tiles,
        const std::shared_ptr<MAPS::Tile>& tile)
    {
        /// @todo   The tile dimensions should be an inherent property in the layer.
        float tile_width_in_pixels = tile->GetWidthInPixels();
        float tile_height_in_pixels = tile->GetHeightInPixels();

        /// @todo   Helper function for the below logic...
        float layer_left_x_position = CenterWorldPosition.X - (DimensionsInTiles.X / 2 * tile_width_in_pixels);
        /// @todo   Document inversion of coordinates!
        float layer_top_y_position = CenterWorldPosition.Y - (DimensionsInTiles.Y / 2 * tile_height_in_pixels);

        // SET THE TILE'S WORLD POSITION.
        float x_offset_in_pixels = x_offset_from_left_in_tiles * tile_width_in_pixels;
        float world_x_position = layer_left_x_position + x_offset_in_pixels;

        float y_offset_in_pixels = y_offset_from_top_in_tiles * tile_height_in_pixels;
        float world_y_position = layer_top_y_position + y_offset_in_pixels;

        //std::cout << world_x_position << ", " << world_y_position << std::endl;

        tile->GetSprite()->SetWorldPosition(world_x_position, world_y_position);
        
        // STORE THE TILE IN THE LAYER.
        /// @todo   Validate range?
        Tiles(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles) = tile;
    }

    std::shared_ptr<MAPS::Tile> GroundLayer::GetTile(
        const unsigned int x_offset_from_left_in_tiles,
        const unsigned int y_offset_from_top_in_tiles) const
    {
        // MAKE SURE THE POSITION IS WITHIN BOUNDS.
        bool x_position_within_bounds = (x_offset_from_left_in_tiles < Tiles.GetWidth());
        bool y_position_within_bounds = (y_offset_from_top_in_tiles < Tiles.GetHeight());
        bool position_within_bounds = (x_position_within_bounds && y_position_within_bounds);
        if (!position_within_bounds)
        {
            // No tile exists at the position within this layer.
            return nullptr;
        }

        // GET THE TILE WITH THE SPECIFIED ID.
        std::shared_ptr<MAPS::Tile> tile = Tiles(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles);
        return tile;
    }

    std::vector< std::shared_ptr<GRAPHICS::IGraphicsComponent> > GroundLayer::GetRenderables() const
    {
        std::vector< std::shared_ptr<GRAPHICS::IGraphicsComponent> > tile_sprites;

        // GATHER ALL TILE SPRITES.
        unsigned int height_in_tiles = Tiles.GetHeight();
        unsigned int width_in_tiles = Tiles.GetWidth();
        for (unsigned int tile_y = 0; tile_y < height_in_tiles; ++tile_y)
        {
            for (unsigned int tile_x = 0; tile_x < width_in_tiles; ++tile_x)
            {
                // GET THE SPRITE FOR THE CURRENT TILE.
                const std::shared_ptr<MAPS::Tile>& tile = Tiles(tile_x, tile_y);
                std::shared_ptr<GRAPHICS::Sprite> sprite = tile->GetSprite();
                tile_sprites.push_back(sprite);

                //std::cout << tile->GetId() << " ";
            }

            //std::cout << std::endl;
        }

        return tile_sprites;
    }

    MATH::Vector2f GroundLayer::GetSizeInPixels() const
    {
        /// @todo   Consider a way to query for the tile size,
        /// if this method stays.
        const unsigned int TILE_DIMENSION_IN_PIXELS = 16;
        
        // GET THE WIDTH IN PIXELS.
        float width_in_tiles = static_cast<float>(DimensionsInTiles.X);
        float tile_width_in_pixels = static_cast<float>(TILE_DIMENSION_IN_PIXELS);
        float width_in_pixels = width_in_tiles * tile_width_in_pixels;

        // GET THE HEIGHT IN PIXELS.
        float height_in_tiles = static_cast<float>(DimensionsInTiles.Y);
        float tile_height_in_pixels = static_cast<float>(TILE_DIMENSION_IN_PIXELS);
        float height_in_pixels = height_in_tiles * tile_height_in_pixels;

        // RETURN THE GROUND SIZE IN PIXELS.
        MATH::Vector2f size_in_pixels(width_in_pixels, height_in_pixels);
        return size_in_pixels;
    }
}