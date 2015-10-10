#include "Maps/GroundLayer.h"

namespace MAPS
{
    GroundLayer::GroundLayer(
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles) :
    CenterWorldPosition(center_world_position),
    Tiles(dimensions_in_tiles.X, dimensions_in_tiles.Y)
    {}

    void GroundLayer::SetTile(
        const unsigned int x_offset_from_left_in_tiles,
        const unsigned int y_offset_from_top_in_tiles,
        const std::shared_ptr<MAPS::Tile>& tile)
    {
        // VALIDATE THAT THE OFFSETS ARE VALID.
        unsigned int layer_width_in_tiles = Tiles.GetWidth();
        unsigned int layer_height_in_tiles = Tiles.GetHeight();
        bool x_offset_valid = (x_offset_from_left_in_tiles < layer_width_in_tiles);
        bool y_offset_valid = (y_offset_from_top_in_tiles < layer_height_in_tiles);
        bool offsets_valid = (x_offset_valid && y_offset_valid);
        if (!offsets_valid)
        {
            // The tile cannot be properly set in the layer since its
            // offsets are outside of the layer.
            return;
        }

        // CALCULATE THE TOP-LEFT WORLD POSITION OF THE LAYER.
        /// @todo   The tile dimensions should be an inherent property in the layer.
        float tile_width_in_pixels = tile->GetWidthInPixels();
        float layer_half_width_in_tiles = static_cast<float>(layer_width_in_tiles) / 2.0f;
        float layer_half_width_in_pixels = layer_half_width_in_tiles * tile_width_in_pixels;
        float layer_left_x_position = CenterWorldPosition.X - layer_half_width_in_pixels;

        float tile_height_in_pixels = tile->GetHeightInPixels();
        float layer_half_height_in_tiles = static_cast<float>(layer_height_in_tiles) / 2.0f;
        float layer_half_height_in_pixels = layer_half_height_in_tiles * tile_height_in_pixels;
        // Smaller y-coordinates are at the top of the screen.
        float layer_top_y_position = CenterWorldPosition.Y - layer_half_height_in_pixels;

        // SET THE TILE'S WORLD POSITION.
        float tile_x_offset_in_pixels = x_offset_from_left_in_tiles * tile_width_in_pixels;
        float tile_world_x_position = layer_left_x_position + tile_x_offset_in_pixels;

        float tile_y_offset_in_pixels = y_offset_from_top_in_tiles * tile_height_in_pixels;
        float tile_world_y_position = layer_top_y_position + tile_y_offset_in_pixels;

        tile->SetWorldPosition(tile_world_x_position, tile_world_y_position);
        
        // STORE THE TILE IN THE LAYER.
        Tiles(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles) = tile;
    }
}