#include "Maps/TileMapTileLayer.h"

namespace MAPS
{
    TileMapTileLayer::TileMapTileLayer(
        const std::string& name,
        const MATH::Vector2f& top_left_world_position,
        const CORE::Array2D<TileId>& tile_ids,
        const MAPS::Tileset& tileset) :
    Name(name),
    Tiles(tile_ids.GetWidth(), tile_ids.GetHeight())
    {
        Tiles = CreateTiles(top_left_world_position, tile_ids, tileset);
    }

    TileMapTileLayer::~TileMapTileLayer()
    {}

    std::shared_ptr<Tile> TileMapTileLayer::GetTileAtGridPosition(
        const unsigned int tile_x_grid_position,
        const unsigned int tile_y_grid_position) const
    {
        // MAKE SURE THE POSITION IS WITHIN BOUNDS.
        bool x_position_within_bounds = (tile_x_grid_position < Tiles.GetWidth());
        bool y_position_within_bounds = (tile_y_grid_position < Tiles.GetHeight());
        bool position_within_bounds = (x_position_within_bounds && y_position_within_bounds);
        if (!position_within_bounds)
        {
            // No tile exists at the position within this layer.
            return nullptr;
        }

        // GET THE TILE WITH THE SPECIFIED ID.
        std::shared_ptr<Tile> tile = Tiles(tile_x_grid_position, tile_y_grid_position);
        return tile;
    }

    void TileMapTileLayer::Render(sf::RenderTarget& render_target)
    {
        // RENDER ALL TILES.
        unsigned int layer_height_in_tiles = Tiles.GetHeight();
        unsigned int layer_width_in_tiles = Tiles.GetWidth();
        for (unsigned int current_tile_y = 0;
            current_tile_y < layer_height_in_tiles;
            ++current_tile_y)
        {
            // RENDER TILES FOR THIS ROW.
            for (unsigned int current_tile_x = 0;
                current_tile_x < layer_width_in_tiles;
                ++current_tile_x)
            {
                // ONLY RENDER THE TILE IF IS EXISTS.
                std::shared_ptr<Tile> tile = Tiles(current_tile_x, current_tile_y);
                bool tile_exists = (nullptr != tile);
                if (tile_exists)
                {
                    tile->Render(render_target);
                }
                else
                {
                    // Skip to the next tile.  Nothing will be rendered for this position.
                    continue;
                }
            }
        }
    }

    CORE::Array2D< std::shared_ptr<Tile> > TileMapTileLayer::CreateTiles(
        const MATH::Vector2f& top_left_world_position,
        const CORE::Array2D<TileId>& tile_ids,
        const MAPS::Tileset& tileset) const
    {
        // CREATE TILES FOR EACH ID IN THE PROVIDED ARRAY.
        unsigned int layer_width_in_tiles = tile_ids.GetWidth();
        unsigned int layer_height_in_tiles = tile_ids.GetHeight();
        CORE::Array2D< std::shared_ptr<Tile> > tiles(layer_width_in_tiles, layer_height_in_tiles);
        for (unsigned int current_tile_y = 0;
            current_tile_y < layer_height_in_tiles;
            ++current_tile_y)
        {
            // CREATE TILES FOR THIS ROW.
            for (unsigned int current_tile_x = 0;
                current_tile_x < layer_width_in_tiles;
                ++current_tile_x)
            {
                // GET THE BASIC TILE INFORMATION FOR THE CURRENT TILE.
                TileId tile_id = tile_ids(current_tile_x, current_tile_y);
                std::shared_ptr<Tile> tile = tileset.CreateTile(tile_id);
                bool tile_exists_in_tileset = (nullptr != tile);
                if (!tile_exists_in_tileset)
                {
                    // Skip to trying to create the next tile.  The layer
                    // simply won't have any any tile at this location.
                    continue;
                }

                // RE-POSITION THE TILE AT THE CORRECT POSITION WITHIN THE WORLD.
                float tile_width_in_pixels = tile->GetWidthInPixels();
                float tile_x_world_position = (
                    static_cast<float>(current_tile_x) * tile_width_in_pixels)
                    + top_left_world_position.X;
                float tile_height_in_pixels = tile->GetHeightInPixels();
                float tile_y_world_position = (
                    static_cast<float>(current_tile_y)* tile_height_in_pixels)
                    + top_left_world_position.Y;

                tile->SetTopLeftWorldPosition(tile_y_world_position, tile_x_world_position);

                // ADD THE TILE TO THE ARRAY.
                tiles(current_tile_x, current_tile_y) = tile;
            }
        }

        return tiles;
    }
}
