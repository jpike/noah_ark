#pragma once

#include <memory>
#include "Core/Array2D.h"
#include "Maps/TileMap.h"

namespace MAPS
{
    /// The entire overworld of the game.  The overworld is divided into smaller chunks
    /// of individual tile maps.
    class Overworld
    {
    public:
        // CONSTRUCTION.
        /// Creates an empty overworld capable of holding enough tile maps defined by the provided
        /// dimensions.
        /// @param[in]  width_in_tile_maps - The width of the overworld, in units of tile maps.
        /// @param[in]  height_in_tile_maps - The height of the overworld, in units of tile maps.
        explicit Overworld(
            const unsigned int width_in_tile_maps, 
            const unsigned int height_in_tile_maps);

        // TILE MAP RETRIEVAL.
        /// Gets the tile map at the specified row and column indices.
        /// @param[in]  row - The 0-based index (from the top) of the tile map to retrieve.
        /// @param[in]  column - The 0-based index (from the left) of the tile map to retrieve.
        /// @return The tile map at the specified location, if one exists; null otherwise.
        std::shared_ptr<MAPS::TileMap> GetTileMap(const unsigned int row, const unsigned int column) const;
        /// Gets the tile map that includes the specified world coordinates.
        /// @param[in]  world_x_position - The world x position of the tile map to retrieve.
        /// @param[in]  world_y_position - The world y position of the tile map to retrieve.
        /// @return The tile map at the specified location, if one exists; null otherwise.
        std::shared_ptr<MAPS::TileMap> GetTileMap(const float world_x_position, const float world_y_position) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Tile maps in the overworld, arranged by column/row position in a grid rather than
        /// by absolute world positions.  (0,0) (column/x,row/y) is the top-left tile map.
        CORE::Array2D< std::shared_ptr<MAPS::TileMap> > TileMaps;
    };
}