#include "Maps/MultiTileMapGrid.h"

namespace MAPS
{
    /// Constructs the grid with the specified width/height.
    /// @param[in]  width_in_tile_maps - The width of the grid in tile maps.
    /// @param[in]  height_in_tile_maps - The height of the grid in tile maps.
    /// @param[in]  world - The world this map grid is part of.
    MultiTileMapGrid::MultiTileMapGrid(
        const unsigned int width_in_tile_maps, 
        const unsigned int height_in_tile_maps,
        MEMORY::NonNullRawPointer<MAPS::World> world) :
        TileMaps(width_in_tile_maps, height_in_tile_maps),
        AxeSwings(),
        World(world)
    {}

    /// Gets the tile map at the specified row and column indices.
    /// @param[in]  row - The 0-based index (from the top) of the tile map to retrieve.
    /// @param[in]  column - The 0-based index (from the left) of the tile map to retrieve.
    /// @return The tile map at the specified location, if one exists; null otherwise.
    ///     The pointer is only valid as long as this map grid remains in memory.
    const MAPS::TileMap* MultiTileMapGrid::GetTileMap(const unsigned int row, const unsigned int column) const
    {
        // MAKE SURE THE PROVIDED INDICES ARE IN RANGE.
        bool tile_map_indices_valid = TileMaps.IndicesInRange(column, row);
        if (!tile_map_indices_valid)
        {
            // No tile map exists at an invalid location.
            return nullptr;
        }

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        const auto& tile_map = TileMaps(column, row);
        return tile_map.get();
    }

    /// Gets the tile map at the specified row and column indices.
    /// @param[in]  row - The 0-based index (from the top) of the tile map to retrieve.
    /// @param[in]  column - The 0-based index (from the left) of the tile map to retrieve.
    /// @return The tile map at the specified location, if one exists; null otherwise.
    ///     The pointer is only valid as long as this grid remains in memory.
    MAPS::TileMap* MultiTileMapGrid::GetTileMap(const unsigned int row, const unsigned int column)
    {
        // MAKE SURE THE PROVIDED INDICES ARE IN RANGE.
        bool tile_map_indices_valid = TileMaps.IndicesInRange(column, row);
        if (!tile_map_indices_valid)
        {
            // No tile map exists at an invalid location.
            return nullptr;
        }

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        const auto& tile_map = TileMaps(column, row);
        return tile_map.get();
    }
    
    /// Gets the tile map that includes the specified world coordinates.
    /// @param[in]  world_x_position - The world x position of the tile map to retrieve.
    /// @param[in]  world_y_position - The world y position of the tile map to retrieve.
    /// @return The tile map at the specified location, if one exists; null otherwise.
    ///     The pointer is only valid as long as this grid remains in memory.
    const MAPS::TileMap* MultiTileMapGrid::GetTileMap(const float world_x_position, const float world_y_position) const
    {
        // CONVERT THE WORLD POSITIONS TO ROW/COLUMN INDICES.
        float tile_map_width_in_pixels = static_cast<float>(TileMap::WIDTH_IN_TILES * Tile::DIMENSION_IN_PIXELS<unsigned int>);
        float tile_map_height_in_pixels = static_cast<float>(TileMap::HEIGHT_IN_TILES * Tile::DIMENSION_IN_PIXELS<unsigned int>);
        unsigned int column_index = static_cast<unsigned int>(world_x_position / tile_map_width_in_pixels);
        unsigned int row_index = static_cast<unsigned int>(world_y_position / tile_map_height_in_pixels);

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        const MAPS::TileMap* tile_map = GetTileMap(row_index, column_index);
        return tile_map;
    }

    /// Gets the tile map that includes the specified world coordinates.
    /// @param[in]  world_x_position - The world x position of the tile map to retrieve.
    /// @param[in]  world_y_position - The world y position of the tile map to retrieve.
    /// @return The tile map at the specified location, if one exists; null otherwise.
    ///     The pointer is only valid as long as this grid remains in memory.
    MAPS::TileMap* MultiTileMapGrid::GetTileMap(const float world_x_position, const float world_y_position)
    {
        // CONVERT THE WORLD POSITIONS TO ROW/COLUMN INDICES.
        float tile_map_width_in_pixels = static_cast<float>(TileMap::WIDTH_IN_TILES * Tile::DIMENSION_IN_PIXELS<unsigned int>);
        float tile_map_height_in_pixels = static_cast<float>(TileMap::HEIGHT_IN_TILES * Tile::DIMENSION_IN_PIXELS<unsigned int>);
        unsigned int column_index = static_cast<unsigned int>(world_x_position / tile_map_width_in_pixels);
        unsigned int row_index = static_cast<unsigned int>(world_y_position / tile_map_height_in_pixels);

        // GET THE TILE MAP AT THE SPECIFIED LOCATION.
        MAPS::TileMap* tile_map = GetTileMap(row_index, column_index);
        return tile_map;
    }

    /// Gets the tile that includes the specified world coordinates.
    /// @param[in]  world_x_position - The world x position of the tile to retrieve.
    /// @param[in]  world_y_position - The world y position of the tile to retrieve.
    /// @return The tile at the specified location, if one exists; null otherwise.
    std::shared_ptr<MAPS::Tile> MultiTileMapGrid::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        // GET THE TILE MAP AT THE SPECIFIED WORLD POSITION.
        const MAPS::TileMap* tile_map = GetTileMap(world_x_position, world_y_position);
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