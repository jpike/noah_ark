#pragma once

#include <vector>

namespace STATES
{
    /// Holds data needed to save and restore which ark pieces
    /// have been built in the overworld.
    struct BuiltArkPieceTileMapData
    {
        /// The x position (column) of the tile map for the built
        /// ark piece in the overworld's 2D grid of tile maps.
        unsigned int TileMapGridXPosition = 0;
        /// The y position (row) of the tile map for the built
        /// ark piece in the overworld's 2D grid of tile maps.
        unsigned int TileMapGridYPosition = 0;
        /// The indices of the ark pieces in the tile map
        /// that have been built.
        std::vector<std::size_t> BuiltArkPieceIndices = {};
    };
}
