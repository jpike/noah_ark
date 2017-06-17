#include "Maps/GroundLayer.h"

namespace MAPS
{
    /// Constructor.
    /// @param[in]  center_world_position - The world position of the center
    ///     of the ground layer.
    /// @param[in]  dimensions_in_tiles - The dimensions of the layer in tiles.
    /// @param[in]  tile_dimension_in_pixels - The dimensions (both width and height)
    ///     of an individual tile in the map (in units of pixels).
    GroundLayer::GroundLayer(
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles,
        const unsigned int tile_dimension_in_pixels) :
    TileDimensionInPixels(tile_dimension_in_pixels),
    CenterWorldPosition(center_world_position),
    Tiles(dimensions_in_tiles.X, dimensions_in_tiles.Y)
    {}

    /// Gets the tile that includes the specified world coordinates.
    /// @param[in]  world_x_position - The world x position of the tile to retrieve.
    /// @param[in]  world_y_position - The world y position of the tile to retrieve.
    /// @return The tile at the specified location, if one exists; null otherwise.
    std::shared_ptr<MAPS::Tile> GroundLayer::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        // CALCULATE THE TOP-LEFT POSITION OF THIS GROUND LAYER.        
        MATH::Vector2f ground_layer_half_size_in_pixels;
        
        float ground_layer_width_in_tiles = static_cast<float>(Tiles.GetWidth());
        ground_layer_half_size_in_pixels.X = ground_layer_width_in_tiles * static_cast<float>(TileDimensionInPixels) / 2.0f;

        float ground_layer_height_in_tiles = static_cast<float>(Tiles.GetHeight());
        ground_layer_half_size_in_pixels.Y = ground_layer_height_in_tiles * static_cast<float>(TileDimensionInPixels) / 2.0f;

        MATH::Vector2f top_left_ground_layer_position = CenterWorldPosition - ground_layer_half_size_in_pixels;

        // NORMALIZE THE PROVIDED WORLD POSITION RELATIVE TO THE TOP-LEFT OF THIS GROUND LAYER.
        float x_offset_in_pixels_from_left_of_layer = world_x_position - top_left_ground_layer_position.X;
        float y_offset_in_pixels_from_top_of_layer = world_y_position - top_left_ground_layer_position.Y;

        // CALCULATE THE TILE INDEX OFFSETS FOR THE PROVIDED WORLD POSITION.
        // It is fine if the offsets in pixels are negative - they'll just get converted to unsigned integers
        // that are too large (out-of-bounds) for the array (which get ignored below).
        unsigned int x_offset_from_left_in_tiles = static_cast<unsigned int>(x_offset_in_pixels_from_left_of_layer / static_cast<float>(TileDimensionInPixels));
        unsigned int y_offset_from_top_in_tiles = static_cast<unsigned int>(y_offset_in_pixels_from_top_of_layer / static_cast<float>(TileDimensionInPixels));

        // MAKE SURE THE TILE OFFSETS ARE VALID.
        bool offsets_valid = Tiles.IndicesInRange(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles);
        if (!offsets_valid)
        {
            // No tile exists at the provided position within this ground layer.
            return nullptr;
        }

        // GET THE TILE AT THE SPECIFIED OFFSET.
        std::shared_ptr<MAPS::Tile> tile = Tiles(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles);
        return tile;
    }

    /// Populates the tile at the specified offset in the ground layer.
    /// @param[in]  x_offset_from_left_in_tiles - The x offset from the left of
    ///     the layer, in tiles.
    /// @param[in]  y_offset_from_top_in_tiles - The y offset from the top of
    ///     the layer, in tiles.
    /// @param[in]  tile - The tile to set at the specified position.  Its
    ///     world position will be populated to reflect its positioning
    ///     within this layer within the larger world.
    void GroundLayer::SetTile(
        const unsigned int x_offset_from_left_in_tiles,
        const unsigned int y_offset_from_top_in_tiles,
        const std::shared_ptr<MAPS::Tile>& tile)
    {
        // VALIDATE THAT THE OFFSETS ARE VALID.
        bool offsets_valid = Tiles.IndicesInRange(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles);
        if (!offsets_valid)
        {
            // The tile cannot be properly set in the layer since its
            // offsets are outside of the layer.
            return;
        }

        // CALCULATE THE TOP-LEFT WORLD POSITION OF THE LAYER.
        unsigned int layer_width_in_tiles = Tiles.GetWidth();
        float tile_width_in_pixels = Tile::WidthInPixels<float>();
        float layer_half_width_in_tiles = static_cast<float>(layer_width_in_tiles) / 2.0f;
        float layer_half_width_in_pixels = layer_half_width_in_tiles * tile_width_in_pixels;
        float layer_left_x_position = CenterWorldPosition.X - layer_half_width_in_pixels;

        unsigned int layer_height_in_tiles = Tiles.GetHeight();
        float tile_height_in_pixels = Tile::HeightInPixels<float>();
        float layer_half_height_in_tiles = static_cast<float>(layer_height_in_tiles) / 2.0f;
        float layer_half_height_in_pixels = layer_half_height_in_tiles * tile_height_in_pixels;
        // Smaller y-coordinates are at the top of the screen.
        float layer_top_y_position = CenterWorldPosition.Y - layer_half_height_in_pixels;

        // SET THE TILE'S WORLD POSITION.
        // The half width/height are added to position the tile based on its center rather than top-left.
        float tile_x_offset_in_pixels = x_offset_from_left_in_tiles * tile_width_in_pixels;
        float tile_half_width_in_pixels = tile_width_in_pixels / 2.0f;
        float tile_world_x_position = layer_left_x_position + tile_x_offset_in_pixels + tile_half_width_in_pixels;

        float tile_y_offset_in_pixels = y_offset_from_top_in_tiles * tile_height_in_pixels;
        float tile_half_height_in_pixels = tile_height_in_pixels / 2.0f;
        float tile_world_y_position = layer_top_y_position + tile_y_offset_in_pixels + tile_half_height_in_pixels;

        tile->SetWorldPosition(tile_world_x_position, tile_world_y_position);
        
        // STORE THE TILE IN THE LAYER.
        Tiles(x_offset_from_left_in_tiles, y_offset_from_top_in_tiles) = tile;
    }
}