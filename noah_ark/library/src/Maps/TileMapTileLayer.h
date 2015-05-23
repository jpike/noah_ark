#pragma once

#include "Core/Array2D.h"
#include "Maps/ITileMapLayer.h"
#include "Maps/Tile.h"
#include "Maps/Tileset.h"

namespace MAPS
{
    /// A single layer of tiles in a tile map.
    class TileMapTileLayer : public ITileMapLayer
    {
    public:
        /// Constructor.
        /// @param[in]  tile_ids - IDs for tiles in this layer.
        /// @param[in]  tileset - The set of tiles used for this layer.
        explicit TileMapTileLayer(
            const CORE::Array2D<TileId>& tile_ids,
            const MAPS::Tileset& tileset);
        /// Destructor.
        virtual ~TileMapTileLayer();

        /// @copydoc    ITileMapLayer::GetTileAtGridPosition
        virtual std::shared_ptr<Tile> GetTileAtGridPosition(
            const unsigned int tile_x_grid_position,
            const unsigned int tile_y_grid_position) const;

    private:
        

        /// The tiles in this layer.
        CORE::Array2D< std::shared_ptr<Tile> > Tiles;

        /// IDs for tiles in this layer.
        CORE::Array2D<TileId> TileIds;
        /// The set of tiles used for this layer.
        MAPS::Tileset Tileset;
    };
}
