#include "Maps/Ark.h"
#include "Maps/Tileset.h"
#include "Maps/Data/ArkInteriorTileMapData.h"

namespace MAPS
{
    /// Populates the ark interior based on data read from in-memory assets.
    void Ark::Populate()
    {
        // LOAD THE TILESET TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> tileset_texture = assets.GetTexture(RESOURCES::AssetId::MAIN_TILESET_TEXTURE);
        if (!tileset_texture)
        {
            return;
        }

        // CLEAR ANY PREVIOUS INFORMATION IN THE INTERIOR.
        Interior.LayersFromBottomToTop.clear();

        // CREATE THE TILESET.
        MAPS::Tileset tileset(tileset_texture);

        // LOAD TILE MAPS FOR EACH LAYER INTO THE ARK INTERIOR.
        std::size_t layer_count = MAPS::DATA::ARK_INTERIOR_LAYER_DATA_FROM_BOTTOM_TO_TOP.size();
        for (std::size_t layer_index = 0; layer_index < layer_count; ++layer_index)
        {
            // CREATE AN EMPTY MAP GRID FOR THE CURRENT LAYER.
            auto& current_layer_data = MAPS::DATA::ARK_INTERIOR_LAYER_DATA_FROM_BOTTOM_TO_TOP[layer_index];
            Interior.LayersFromBottomToTop.emplace_back(Ark::INTERIOR_WIDTH_IN_TILE_MAPS, Ark::INTERIOR_HEIGHT_IN_TILE_MAPS);
            auto& current_map_layer = Interior.LayersFromBottomToTop.back();

            // LOAD TILE MAPS FOR EACH ROW.
            for (unsigned int row = 0; row < Ark::INTERIOR_HEIGHT_IN_TILE_MAPS; ++row)
            {
                // LOAD TILE MAPS FOR EACH COLUMN.
                for (unsigned int column = 0; column < Ark::INTERIOR_WIDTH_IN_TILE_MAPS; ++column)
                {
                    // CALCULATE THE POSITION OF THE CURRENT TILE MAP.
                    MATH::Vector2f map_center_world_position;

                    float map_width_in_pixels = static_cast<float>(MAPS::TileMap::WIDTH_IN_TILES * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);
                    float map_half_width_in_pixels = map_width_in_pixels / 2.0f;
                    float map_left_world_position = static_cast<float>(column * map_width_in_pixels);
                    map_center_world_position.X = map_left_world_position + map_half_width_in_pixels;

                    float map_height_in_pixels = static_cast<float>(MAPS::TileMap::HEIGHT_IN_TILES * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);
                    float map_half_height_in_pixels = map_height_in_pixels / 2.0f;
                    float map_top_world_position = static_cast<float>(row * map_height_in_pixels);
                    map_center_world_position.Y = map_top_world_position + map_half_height_in_pixels;

                    // CREATE AN EMPTY TILE MAP.
                    MATH::Vector2ui map_dimensions_in_tiles(
                        MAPS::TileMap::WIDTH_IN_TILES,
                        MAPS::TileMap::HEIGHT_IN_TILES);
                    MAPS::TileMap tile_map(
                        row,
                        column,
                        map_center_world_position,
                        map_dimensions_in_tiles,
                        MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);

                    // GET THE CURRENT TILE MAP.
                    const auto& tile_map_data = (*current_layer_data)(column, row);

                    // CREATE TILES IN THE GROUND LAYER.
                    for (unsigned int current_tile_y = 0;
                        current_tile_y < MAPS::TileMap::HEIGHT_IN_TILES;
                        ++current_tile_y)
                    {
                        // CREATE TILES FOR THIS ROW.
                        for (unsigned int current_tile_x = 0;
                            current_tile_x < MAPS::TileMap::WIDTH_IN_TILES;
                            ++current_tile_x)
                        {
                            // CREATE THE CURRENT TILE.
                            MAPS::TileId tile_id = (*tile_map_data)(current_tile_x, current_tile_y);
                            std::shared_ptr<MAPS::Tile> tile = tileset.CreateTile(tile_id);
                            bool tile_exists_in_tileset = (nullptr != tile);
                            if (!tile_exists_in_tileset)
                            {
                                // Skip to trying to create the next tile.  The layer
                                // simply won't have any tile at this location.
                                continue;
                            }

                            // SET THE TILE IN THE GROUND LAYER.
                            tile_map.Ground.SetTile(current_tile_x, current_tile_y, tile);
                        }
                    }

                    // STORE THE TILE MAP OF THE ARK INTERIOR.
                    current_map_layer.TileMaps(column, row) = std::move(tile_map);
                }
            }
        }

        // CREATE EXIT POINTS FOR MOVING BETWEEN FLOORS OF THE ARK INTERIOR.
        for (std::size_t layer_index = 0; layer_index < layer_count; ++layer_index)
        {
            // GET THE APPLICABLE LAYERS.
            MAPS::MultiTileMapGrid& current_map_layer = Interior.LayersFromBottomToTop.at(layer_index);

            MAPS::MultiTileMapGrid* previous_map_layer = nullptr;
            bool previous_layer_exists = (layer_index > 0);
            if (previous_layer_exists)
            {
                std::size_t previous_map_layer_index = layer_index - 1;
                previous_map_layer = &Interior.LayersFromBottomToTop.at(previous_map_layer_index);
            }

            MAPS::MultiTileMapGrid* next_map_layer = nullptr;
            std::size_t last_layer_index = layer_count - 1;
            bool next_layer_exists = (layer_index < last_layer_index);
            if (next_layer_exists)
            {
                std::size_t next_map_layer_index = layer_index + 1;
                next_map_layer = &Interior.LayersFromBottomToTop.at(next_map_layer_index);
            }

            // CHECK ALL MAPS IN THE CURRENT LAYER'S ROWS FOR STAIRCASES.
            for (unsigned int row = 0; row < Ark::INTERIOR_HEIGHT_IN_TILE_MAPS; ++row)
            {
                // CHECK ALL MAPS IN THE CURRENT LAYER'S COLUMNS FOR STAIRCASES.
                for (unsigned int column = 0; column < Ark::INTERIOR_WIDTH_IN_TILE_MAPS; ++column)
                {
                    // GET THE CURRENT TILE MAP.
                    auto& current_tile_map = current_map_layer.TileMaps(column, row);

                    // CHECK TILES ACROSS ALL ROWS.
                    for (unsigned int current_tile_y = 0;
                        current_tile_y < MAPS::TileMap::HEIGHT_IN_TILES;
                        ++current_tile_y)
                    {
                        // CHECK TILES ACROSS ALL COLUMNS.
                        for (unsigned int current_tile_x = 0;
                            current_tile_x < MAPS::TileMap::WIDTH_IN_TILES;
                            ++current_tile_x)
                        {
                            // GET THE CURRENT TILE.
                            auto& current_tile = current_tile_map.Ground.Tiles(current_tile_x, current_tile_y);
                            if (!current_tile)
                            {
                                // SKIP THIS TILE SINCE IT DOESN'T EXIST.
                                continue;
                            }

                            // CHECK IF THE TILE IS FOR AN UPWARD STAIRCASE.
                            bool tile_is_upward_stairs = (MAPS::TileType::ARK_INTERIOR_UP_STAIRS == current_tile->Type);
                            if (tile_is_upward_stairs)
                            {
                                // ADD AN EXIT POINT TO THE ABOVE FLOOR IF ONE EXISTS.
                                if (next_map_layer)
                                {
                                    auto& above_tile_map = next_map_layer->TileMaps(column, row);

                                    MAPS::ExitPoint ark_floor_exit_point;
                                    MATH::FloatRectangle tile_bounding_box = current_tile->Sprite.GetWorldBoundingBox();
                                    ark_floor_exit_point.BoundingBox = tile_bounding_box;
                                    ark_floor_exit_point.NewMapGrid = next_map_layer;
                                    ark_floor_exit_point.NewTileMap = &above_tile_map;
                                    ark_floor_exit_point.NewPlayerWorldPosition = tile_bounding_box.Center();

                                    current_tile_map.ExitPoints.push_back(ark_floor_exit_point);
                                }
                            }

                            // CHECK IF THE TILE IS FOR AN DOWNWARD STAIRCASE.
                            bool tile_is_downward_stairs = (MAPS::TileType::ARK_INTERIOR_DOWN_STAIRS == current_tile->Type);
                            if (tile_is_downward_stairs)
                            {
                                // ADD AN EXIT POINT TO THE BELOW FLOOR IF ONE EXISTS.
                                if (previous_map_layer)
                                {
                                    auto& below_tile_map = previous_map_layer->TileMaps(column, row);

                                    MAPS::ExitPoint ark_floor_exit_point;
                                    MATH::FloatRectangle tile_bounding_box = current_tile->Sprite.GetWorldBoundingBox();
                                    ark_floor_exit_point.BoundingBox = tile_bounding_box;
                                    ark_floor_exit_point.NewMapGrid = previous_map_layer;
                                    ark_floor_exit_point.NewTileMap = &below_tile_map;
                                    ark_floor_exit_point.NewPlayerWorldPosition = tile_bounding_box.Center();

                                    current_tile_map.ExitPoints.push_back(ark_floor_exit_point);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
