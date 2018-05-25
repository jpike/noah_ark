#pragma once

#include <vector>
#include "Maps/MultiTileMapGrid.h"

namespace MAPS
{
    /// A collection of multiple multi-tile-map-grids organized into multiple
    /// vertical layers.  This supports vertical movement across different
    /// larger maps.
    class LayeredMultiTileMapGrids
    {
    public:
        bool Contains(const MultiTileMapGrid* const map_grid) const;
        
        /// The layers of maps from bottom (smallest index) to top (largest index).
        std::vector<MultiTileMapGrid> LayersFromBottomToTop = {};
    };
}
