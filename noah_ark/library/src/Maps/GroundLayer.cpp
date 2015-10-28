#include "Maps/GroundLayer.h"

namespace MAPS
{
    GroundLayer::GroundLayer(
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles) :
    CenterWorldPosition(center_world_position),
    Tiles(dimensions_in_tiles.X, dimensions_in_tiles.Y)
    {}

    std::shared_ptr<MAPS::Tile> GroundLayer::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        // CALCULATE THE TOP-LEFT POSITION OF THIS GROUND LAYER.
        /// @todo   This probably shouldn't be hardcoded...Perhaps include in overworld map file and pass in during construction.
        const float TILE_DIMENSION_IN_PIXELS = 16.0f;
        
        MATH::Vector2f ground_layer_half_size_in_pixels;
        
        float ground_layer_width_in_tiles = static_cast<float>(Tiles.GetWidth());
        ground_layer_half_size_in_pixels.X = ground_layer_width_in_tiles * TILE_DIMENSION_IN_PIXELS / 2.0f;

        float ground_layer_height_in_tiles = static_cast<float>(Tiles.GetHeight());
        ground_layer_half_size_in_pixels.Y = ground_layer_height_in_tiles * TILE_DIMENSION_IN_PIXELS / 2.0f;

        /// @todo   Might there be a clearer way to do this than what is being done throughout the body of this function?
        MATH::Vector2f top_left_ground_layer_position = CenterWorldPosition - ground_layer_half_size_in_pixels;

        // NORMALIZE THE PROVIDED WORLD POSITION RELATIVE TO THE TOP-LEFT OF THIS GROUND LAYER.
        float x_offset_in_pixels_from_left_of_layer = world_x_position - top_left_ground_layer_position.X;
        float y_offset_in_pixels_from_top_of_layer = world_y_position - top_left_ground_layer_position.Y;

        // CALCULATE THE TILE INDEX OFFSETS FOR THE PROVIDED WORLD POSITION.
        // It is fine if the offsets in pixels are negative - they'll just get converted to unsigned integers
        // that are too large (out-of-bounds) for the array (which get ignored below).
        unsigned int x_offset_from_left_in_tiles = static_cast<unsigned int>(x_offset_in_pixels_from_left_of_layer / TILE_DIMENSION_IN_PIXELS);
        unsigned int y_offset_from_top_in_tiles = static_cast<unsigned int>(y_offset_in_pixels_from_top_of_layer / TILE_DIMENSION_IN_PIXELS);

        // MAKE SURE THE TILE OFFSETS ARE VALID.
        bool x_offset_valid = (x_offset_from_left_in_tiles < ground_layer_width_in_tiles);
        bool y_offset_valid = (y_offset_from_top_in_tiles < ground_layer_height_in_tiles);
        bool offsets_valid = (x_offset_valid && y_offset_valid);
        if (!offsets_valid)
        {
            // No tile exists at the provided position within this ground layer.
            return nullptr;
        }

        // GET THE TILE AT THE SPECIFIED OFFSET.
        std::shared_ptr<MAPS::Tile> tile = Tiles(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles);
        return tile;
    }

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