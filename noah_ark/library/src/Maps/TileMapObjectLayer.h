#pragma once

#include <vector>
#include "Maps/ITileMapLayer.h"
#include "Maps/TileMapObjectDescription.h"
#include "Maps/Tileset.h"

namespace MAPS
{
    /// A single layer of objects in a tile map.
    class TileMapObjectLayer : public ITileMapLayer
    {
    public:
        /// #todo   Finish implementation!
        explicit TileMapObjectLayer(
            const std::vector<TileMapObjectDescription>& objects,
            const Tileset& tileset) {}
        virtual ~TileMapObjectLayer() {}

        /// @todo   Finish implementation!
        virtual std::shared_ptr<Tile> GetTileAtGridPosition(
            const unsigned int tile_x_grid_position,
            const unsigned int tile_y_grid_position) const
        {
            return nullptr;
        }

    private:

    };
}
