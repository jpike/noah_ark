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
        /// Creates an empty overworld capable of holding enough tile maps defined by the provided
        /// dimensions.
        /// @param[in]  width_in_tile_maps - The width of the overworld, in units of tile maps.
        /// @param[in]  height_in_tile_maps - The height of the overworld, in units of tile maps.
        explicit Overworld(
            const unsigned int width_in_tile_maps, 
            const unsigned int height_in_tile_maps);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Tile maps in the overworld, arranged by column/row position in a grid rather than
        /// by absolute world positions.  (0,0) (column/x,row/y) is the top-left tile map.
        CORE::Array2D< std::shared_ptr<MAPS::TileMap> > TileMaps;
    };
}