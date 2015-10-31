#include "Maps/Overworld.h"

namespace MAPS
{
    Overworld::Overworld(
        const unsigned int width_in_tile_maps,
        const unsigned int height_in_tile_maps,
        const unsigned int tile_map_width_in_tiles,
        const unsigned int tile_map_height_in_tiles,
        const unsigned int tile_dimension_in_pixels) :
    TileMaps(width_in_tile_maps, height_in_tile_maps),
    TileMapWidthInTiles(tile_map_width_in_tiles),
    TileMapHeightInTiles(tile_map_height_in_tiles),
    TileDimensionInPixels(tile_dimension_in_pixels)
    {}

    MAPS::TileMap* Overworld::GetTileMap(const unsigned int row, const unsigned int column)
    {
        // MAKE SURE THE PROVIDED INDICES ARE IN RANGE.
        bool tile_map_indices_valid = TileMaps.IndicesInRange(column, row);
        if (!tile_map_indices_valid)
        {
            // No tile map exists at an invalid location.
            return nullptr;
        }

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        MAPS::TileMap& tile_map = TileMaps(column, row);
        return &tile_map;
    }
    
    MAPS::TileMap* Overworld::GetTileMap(const float world_x_position, const float world_y_position)
    {
        // CONVERT THE WORLD POSITIONS TO ROW/COLUMN INDICES.
        float tile_map_width_in_pixels = static_cast<float>(TileMapWidthInTiles * TileDimensionInPixels);
        float tile_map_height_in_pixels = static_cast<float>(TileMapHeightInTiles * TileDimensionInPixels);
        unsigned int column_index = static_cast<unsigned int>(world_x_position / tile_map_width_in_pixels);
        unsigned int row_index = static_cast<unsigned int>(world_y_position / tile_map_height_in_pixels);

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        MAPS::TileMap* tile_map = GetTileMap(row_index, column_index);
        return tile_map;
    }

    std::shared_ptr<MAPS::Tile> Overworld::GetTileAtWorldPosition(const float world_x_position, const float world_y_position)
    {
        // GET THE TILE MAP AT THE SPECIFIED WORLD POSITION.
        MAPS::TileMap* tile_map = GetTileMap(world_x_position, world_y_position);
        bool tile_map_exists = (nullptr != tile_map);
        if (!tile_map_exists)
        {
            // No tile map exists that could contain a tile at the specified world position.
            return nullptr;
        }

        // GET THE TILE IN THE TILE MAP AT THE SPECIFIED WORLD POSITION.
        std::shared_ptr<MAPS::Tile> tile = tile_map->GetTileAtWorldPosition(world_x_position, world_y_position);
        return tile;
    }
}