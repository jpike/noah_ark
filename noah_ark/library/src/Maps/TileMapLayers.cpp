#include "Maps/TileMapLayers.h"

namespace MAPS
{
    TileMapLayers::TileMapLayers() :
    LayersFromBottomToTop()
    {}

    void TileMapLayers::Clear()
    {
        LayersFromBottomToTop.clear();
    }

    void TileMapLayers::AddToTop(std::unique_ptr<ITileMapLayer>& layer)
    {
        LayersFromBottomToTop.push_back(std::move(layer));
    }

    std::shared_ptr<Tile> TileMapLayers::GetTileAtGridPosition(
        const unsigned int tile_x_grid_position,
        const unsigned int tile_y_grid_position) const
    {
        // MAKE SURE WE HAVE AT LEAST ONE LAYER.
        bool layers_exist = !LayersFromBottomToTop.empty();
        if (!layers_exist)
        {
            // No tile exists for returning.
            return nullptr;
        }

        // GET THE TILE AT THE SPECIFIED POSITION.
        /// @todo   We probably need to us z-positions.
        /// For now, just iterate from top to bottom.
        for (auto layer = LayersFromBottomToTop.rbegin();
            layer != LayersFromBottomToTop.rend();
            ++layer)
        {
            // RETURN THE TILE AT THE CURRENT LAYER IF ONE EXISTS.
            std::shared_ptr<Tile> tile = (*layer)->GetTileAtGridPosition(tile_x_grid_position, tile_y_grid_position);
            bool tile_exists = (nullptr != tile);
            if (tile_exists)
            {
                return tile;
            }
        }
        
        // No tile could be found.
        return nullptr;
    }

    void TileMapLayers::Render(sf::RenderTarget& render_target)
    {
        // RENDER LAYERS FROM BOTTOM TO TOP.
        for (auto& layer : LayersFromBottomToTop)
        {
            layer->Render(render_target);
        }
    }
}
