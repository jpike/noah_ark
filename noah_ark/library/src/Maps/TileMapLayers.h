#pragma once

#include <memory>
#include <vector>
#include "Maps/ITileMapLayer.h"
#include "Maps/Tile.h"

namespace MAPS
{
    /// A collection of layers within a tile map.
    class TileMapLayers
    {
    public:
        /// Creates an empty collection of tile map layers.
        explicit TileMapLayers();

        /// Clears all layers from the collection.
        void Clear();
        /// Adds a layer to the top of the collection.
        /// @param[in,out]  layer - The layer to add.
        void AddToTop(std::unique_ptr<ITileMapLayer>& layer);

        /// Retrieves a tile at the specified position within
        /// the grid for the layers.  (0,0) is the top-left of
        /// of the grid.
        /// @todo   How to determine which layer?
        /// @param[in]  tile_x_grid_position - The horizontal position
        ///     of the tile to retrieve.
        /// @param[in]  tile_y_grid_position - The vertical position
        ///     of the tile to retrieve.
        /// @return The tile at the specified position or null
        ///     if no tile exists at the position.
        std::shared_ptr<Tile> GetTileAtGridPosition(
            const unsigned int tile_x_grid_position, 
            const unsigned int tile_y_grid_position) const;

        /// Renders all layers in this collection, from bottom to top.
        /// @param[in,out]  renderTarget - The target to render to.
        void Render(sf::RenderTarget& render_target);

    private:

        /// Layers in the map organized from bottom (first) to top (last).
        std::vector< std::unique_ptr<ITileMapLayer> > LayersFromBottomToTop;

    };
}
