#include "Maps/Overworld.h"

namespace MAPS
{
    Overworld::Overworld(
        const unsigned int width_in_tile_maps,
        const unsigned int height_in_tile_maps) :
    TileMaps(width_in_tile_maps, height_in_tile_maps)
    {}

    std::shared_ptr<MAPS::TileMap> Overworld::GetTileMap(const unsigned int row, const unsigned int column) const
    {
        // MAKE SURE THE PROVIDED INDICES ARE IN RANGE.
        /// @todo   Make a similar method on the Array2D class?
        unsigned int row_count = TileMaps.GetHeight();
        unsigned int column_count = TileMaps.GetWidth();
        bool row_valid = (row < row_count);
        bool column_valid = (column < column_count);
        bool tile_map_indices_valid = (row_valid && column_valid);
        if (!tile_map_indices_valid)
        {
            // No tile map exists at an invalid location.
            return nullptr;
        }

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        std::shared_ptr<MAPS::TileMap> tile_map = TileMaps(column, row);
        return tile_map;
    }
    
    std::shared_ptr<MAPS::TileMap> Overworld::GetTileMap(const float world_x_position, const float world_y_position) const
    {
        // CONVERT THE WORLD POSITIONS TO ROW/COLUMN INDICES.
        /// @todo   This probably shouldn't be hardcoded...Perhaps include in overworld map file and pass in during construction.
        const float TILE_DIMENSION_IN_PIXELS = 16.0f;
        const float TILE_MAP_WIDTH_IN_TILES = 32.0f;
        const float TILE_MAP_WIDTH_IN_PIXELS = TILE_MAP_WIDTH_IN_TILES * TILE_DIMENSION_IN_PIXELS;
        const float TILE_MAP_HEIGHT_IN_TILES = 24.0f;
        const float TILE_MAP_HEIGHT_IN_PIXELS = TILE_MAP_HEIGHT_IN_TILES * TILE_DIMENSION_IN_PIXELS;
        unsigned int column_index = static_cast<unsigned int>(world_x_position / TILE_MAP_WIDTH_IN_PIXELS);
        unsigned int row_index = static_cast<unsigned int>(world_y_position / TILE_MAP_HEIGHT_IN_PIXELS);

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        std::shared_ptr<MAPS::TileMap> tile_map = GetTileMap(row_index, column_index);
        return tile_map;
    }
}