#include "Maps/TileMapTileLayer.h"

namespace MAPS
{
    TileMapTileLayer::TileMapTileLayer(
        const CORE::Array2D<TileId>& tile_ids,
        const MAPS::Tileset& tileset) :
    Tiles(),
    TileIds(tile_ids),
    Tileset(tileset)
    {}
    
    TileMapTileLayer::~TileMapTileLayer()
    {}

    std::shared_ptr<Tile> TileMapTileLayer::GetTileAtGridPosition(
        const unsigned int tile_x_grid_position,
        const unsigned int tile_y_grid_position) const
    {
        // MAKE SURE THE POSITION IS WITHIN BOUNDS.
        bool x_position_within_bounds = (tile_x_grid_position < TileIds.GetWidth());
        bool y_position_within_bounds = (tile_y_grid_position < TileIds.GetHeight());
        bool position_within_bounds = (x_position_within_bounds && y_position_within_bounds);
        if (!position_within_bounds)
        {
            // No tile exists at the position within this layer.
            return nullptr;
        }

        // GET THE TILE WITH THE SPECIFIED ID.
        /// @todo   We need to actually get a "clone" of the tile.
        TileId tile_id = TileIds(tile_x_grid_position, tile_y_grid_position);
        std::shared_ptr<Tile> tile = Tileset.GetTile(tile_id);
        return tile;
    }
}
