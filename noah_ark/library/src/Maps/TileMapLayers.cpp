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
        /// @todo   The first (bottom) layer is arbitrarily chosen
        /// for now.  Something else will need to be done later so
        /// that a tile for a specific layer can be retrieved.
        const std::unique_ptr<ITileMapLayer>& layer = LayersFromBottomToTop.front();
        return layer->GetTileAtGridPosition(tile_x_grid_position, tile_y_grid_position);
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
