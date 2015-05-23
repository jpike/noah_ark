#pragma once

#include <memory>
#include "Maps/Tile.h"

namespace MAPS
{
    /// An interface for different types of layers
    /// that may exist within a tile map.
    class ITileMapLayer
    {
    public:
        /// Virtual destructor to ensure proper destruction
        /// of classes implementing this interface.
        virtual ~ITileMapLayer() {}

        /// Retrieves a tile at the specified position within
        /// the grid for the layers.  (0,0) is the top-left of
        /// of the grid.
        /// @param[in]  tile_x_grid_position - The horizontal position
        ///     of the tile to retrieve.
        /// @param[in]  tile_y_grid_position - The vertical position
        ///     of the tile to retrieve.
        /// @return The tile at the specified position or null
        ///     if no tile exists at the position.
        virtual std::shared_ptr<Tile> GetTileAtGridPosition(
            const unsigned int tile_x_grid_position,
            const unsigned int tile_y_grid_position) const = 0;
    };
}
