#include "Maps/LayeredMultiTileMapGrids.h"

namespace MAPS
{
    /// Determines if the specified map grid is in these layers of map grids.
    /// @param[in]  map_grid - The map grid to check for.
    /// @return True if the provided map grid is in these layers; false otherwise.
    bool LayeredMultiTileMapGrids::Contains(const MultiTileMapGrid* const map_grid) const
    {
        // CHECK EACH LAYER FOR THE PROVIDED MAP GRID.
        for (const auto& layer : LayersFromBottomToTop)
        {
            // CHECK IF THE CURRENT LAYER IS THE PROVIDED MAP GRID.
            bool current_layer_is_map_grid = (&layer == map_grid);
            if (current_layer_is_map_grid)
            {
                return true;
            }
        }

        // INDICATE THAT THE MAP GRID WASN'T FOUND.
        return false;
    }
}
