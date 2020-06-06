#include "Maps/Data/ArkInteriorTileMapData.h"
#include "Maps/Data/OverworldMapData.h"
#include "Maps/Tileset.h"
#include "Maps/World.h"
#include "Math/Number.h"
#include "Resources/FoodGraphics.h"

namespace MAPS
{
    /// Populates the world using the provided assets.
    /// @param[in,out]  assets - The assets to use for populating the world.
    /// @return The world, if successfully populated; null otherwise.
    std::shared_ptr<World> World::Populate(RESOURCES::Assets& assets)
    {
        // POPULATE THE WORLD.
        std::shared_ptr<MAPS::World> world = std::make_shared<MAPS::World>();
        world->PopulateOverworld(assets);
        world->PopulateArkInterior(assets);

        // CREATE THE EXIT POINTS BETWEEN MAP GRIDS.
        // The ark interior data used for exit points is currently hard-coded to simplify
        // things since I'm not sure yet exactly how we want to structure this.
        // The ark should always be entered at the bottom layer.
        auto& starting_ark_map_grid = world->ArkInterior.LayersFromBottomToTop.front();
        MAPS::TileMap* starting_ark_interior_tile_map = &starting_ark_map_grid.TileMaps(1, 0);
        // For now, this is hardcoded to an arbitrary place near the bottom of the starting tile map.
        MATH::FloatRectangle ark_interior_bounding_box = starting_ark_interior_tile_map->GetWorldBoundingBox();
        float ark_interior_center_x_position = ark_interior_bounding_box.CenterX();
        float ark_interior_bottom_y_position = ark_interior_bounding_box.RightBottom.Y;
        MATH::Vector2f ark_interior_bottom_center_position(ark_interior_center_x_position, ark_interior_bottom_y_position);
        MATH::Vector2f ark_interior_player_start_position = ark_interior_bottom_center_position;
        ark_interior_player_start_position.Y -= MAPS::Tile::DIMENSION_IN_PIXELS<float> * 3.0f;

        MATH::FloatRectangle ark_interior_exit_point_bounding_box = MATH::FloatRectangle::FromCenterAndDimensions(
            ark_interior_player_start_position.X,
            ark_interior_player_start_position.Y + MAPS::Tile::DIMENSION_IN_PIXELS<float>,
            MAPS::Tile::DIMENSION_IN_PIXELS<float> * 4.0f,
            MAPS::Tile::DIMENSION_IN_PIXELS<float> * 2.0f);

        // Note that this is intentionally done in a a not-very-efficient way right now.
        // While we could theoretically create the initial exit points when creating
        // the different map grids, that's not currently being done now due to risk in
        // having incomplete exit points and the fact that I'm not sure yet exactly
        // what form we'll want these exit points in.
        unsigned int overworld_height_in_tile_maps = world->Overworld.TileMaps.GetHeight();
        unsigned int overworld_width_in_tile_maps = world->Overworld.TileMaps.GetWidth();
        for (unsigned int tile_map_row = 0; tile_map_row < overworld_height_in_tile_maps; ++tile_map_row)
        {
            for (unsigned int tile_map_column = 0; tile_map_column < overworld_width_in_tile_maps; ++tile_map_column)
            {
                // SEARCH THE CURRENT TILE MAP FOR ANY ARK PIECE EXISTS.
                MAPS::TileMap& overworld_tile_map = world->Overworld.TileMaps(tile_map_column, tile_map_row);
                for (const auto& ark_piece : overworld_tile_map.ArkPieces)
                {
                    // CREATE EXIT POINTS FOR ANY DOORWAYS.
                    if (ark_piece.IsExternalDoorway)
                    {
                        // CREATE AN EXIT POINT FROM THE OVERWORLD INTO THE ARK.
                        MAPS::ExitPoint overworld_to_ark_exit_point;
                        MATH::FloatRectangle ark_piece_bounding_box = ark_piece.Sprite.GetWorldBoundingBox();
                        overworld_to_ark_exit_point.BoundingBox = ark_piece_bounding_box;
                        overworld_to_ark_exit_point.NewMapGrid = &starting_ark_map_grid;
                        overworld_to_ark_exit_point.NewTileMap = starting_ark_interior_tile_map;
                        overworld_to_ark_exit_point.NewPlayerWorldPosition = ark_interior_player_start_position;

                        overworld_tile_map.ExitPoints.push_back(overworld_to_ark_exit_point);

                        // CREATE AN EXIT POINT FROM THE ARK INTERIOR TO THE OVERWORLD.
                        MAPS::ExitPoint ark_to_overworld_exit_point;
                        ark_to_overworld_exit_point.BoundingBox = ark_interior_exit_point_bounding_box;
                        ark_to_overworld_exit_point.NewMapGrid = &world->Overworld;
                        ark_to_overworld_exit_point.NewTileMap = &overworld_tile_map;
                        MATH::Vector2f overworld_exit_starting_position = ark_piece_bounding_box.Center();
                        overworld_exit_starting_position.Y = ark_piece_bounding_box.RightBottom.Y;
                        ark_to_overworld_exit_point.NewPlayerWorldPosition = overworld_exit_starting_position;

                        starting_ark_interior_tile_map->ExitPoints.push_back(ark_to_overworld_exit_point);
                    }
                }
            }
        }

        // RETURN THE POPULATED WORLD.
        return world;
    }

    /// Populates the overworld based on data read from in-memory assets.
    /// @param[in,out]  assets - The assets for the overworld.
    void World::PopulateOverworld(RESOURCES::Assets& assets)
    {
        // LOAD THE TILESET TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> tileset_texture = assets.GetTexture(RESOURCES::AssetId::MAIN_TILESET_TEXTURE);
        if (!tileset_texture)
        {
            return;
        }

        // LOAD THE TREE TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> tree_texture = assets.GetTexture(RESOURCES::AssetId::TREE_TEXTURE);
        if (!tree_texture)
        {
            return;
        }

        // CREATE THE TILESET.
        MAPS::Tileset tileset(tileset_texture);

        // CREATE A RANDOM NUMBER GENERATOR FOR CREATING FOOD.
        MATH::RandomNumberGenerator random_number_generator;

        // LOAD TILE MAPS FOR EACH ROW.
        for (unsigned int row = 0; row < MAPS::World::OVERWORLD_HEIGHT_IN_TILE_MAPS; ++row)
        {
            // LOAD TILE MAPS FOR EACH COLUMN.
            for (unsigned int column = 0; column < MAPS::World::OVERWORLD_WIDTH_IN_TILE_MAPS; ++column)
            {
                // GET THE CURRENT TILE MAP DATA.
                const auto& tile_map_data = MAPS::DATA::OVERWORLD_MAP_DATA(column, row);

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
                        MAPS::TileId tile_id = (*tile_map_data.GroundLayer)(current_tile_x, current_tile_y);
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

                // POPULATE THE ARK LAYER IF ONE EXISTS.
                if (tile_map_data.ArkLayer)
                {
                    // GET THE ARK TEXTURE.
                    std::shared_ptr<GRAPHICS::Texture> ark_texture = assets.GetTexture(RESOURCES::AssetId::ARK_TEXTURE);
                    if (ark_texture)
                    {
                        // CREATE PIECES IN THE ARK LAYER.
                        for (unsigned int current_tile_y = 0;
                            current_tile_y < MAPS::TileMap::HEIGHT_IN_TILES;
                            ++current_tile_y)
                        {
                            // CREATE ARK PIECES FOR THIS ROW.
                            for (unsigned int current_tile_x = 0;
                                current_tile_x < MAPS::TileMap::WIDTH_IN_TILES;
                                ++current_tile_x)
                            {
                                // CHECK IF THE TILE ID IS VALID.
                                // Some tiles in this layer may not be for valid ark pieces.
                                MAPS::TileId tile_id = (*tile_map_data.ArkLayer)(current_tile_x, current_tile_y);
                                bool tild_id_valid = (tile_id > 0);
                                if (!tild_id_valid)
                                {
                                    continue;
                                }

                                // CREATE THE ARK PIECE.
                                OBJECTS::ArkPiece ark_piece(tile_id, ark_texture);
                                MATH::Vector2f ark_piece_local_center = ark_piece.Sprite.GetOrigin();
                                float tile_left_x_position = static_cast<float>(current_tile_x * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);
                                float ark_piece_world_x_position = map_left_world_position + tile_left_x_position + ark_piece_local_center.X;
                                float tile_top_y_position = static_cast<float>(current_tile_y * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>);
                                float ark_piece_world_y_position = map_top_world_position + tile_top_y_position + ark_piece_local_center.Y;
                                ark_piece.Sprite.SetWorldPosition(ark_piece_world_x_position, ark_piece_world_y_position);

                                // ADD THE ARK PIECE TO THE TILE MAP.
                                tile_map.ArkPieces.push_back(ark_piece);
                            }
                        }
                    }
                }

                // POPULATE THE OBJECT LAYER IF ONE EXISTS.
                if (tile_map_data.ObjectLayer)
                {
                    // CREATE ANY TREES IN THE LAYER.
                    for (unsigned int current_tile_y = 0;
                        current_tile_y < MAPS::TileMap::HEIGHT_IN_TILES;
                        ++current_tile_y)
                    {
                        // CREATE ARK PIECES FOR THIS ROW.
                        for (unsigned int current_tile_x = 0;
                            current_tile_x < MAPS::TileMap::WIDTH_IN_TILES;
                            ++current_tile_x)
                        {
                            // CHECK IF THE TILE ID IS VALID.
                            // Some tiles in this layer may not be for valid ark pieces.
                            uint8_t object_id = (*tile_map_data.ObjectLayer)(current_tile_x, current_tile_y);
                            const uint8_t TREE_OBJECT_ID = 1;
                            bool is_tree = (TREE_OBJECT_ID == object_id);
                            if (is_tree)
                            {
                                // DETERMINE THE SUB-RECTANGLE OF THE TEXTURE TO USE FOR THE TREE.
                                MATH::FloatRectangle TALL_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(32.0f, 0.0f, 16.0f, 32.0f);
                                MATH::FloatRectangle tree_texture_sub_rectangle = TALL_TREE_TEXTURE_SUB_RECTANGLE;

                                // CREATE THE TREE'S SPRITE.
                                GRAPHICS::AnimatedSprite tree_sprite(GRAPHICS::Sprite(tree_texture, tree_texture_sub_rectangle));
                                MATH::Vector2f tree_local_center = tree_sprite.Sprite.GetOrigin();
                                auto tree_left_x = current_tile_x * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>;
                                auto tree_top_y = current_tile_y * MAPS::Tile::DIMENSION_IN_PIXELS<unsigned int>;
                                float tree_left_world_x_position = map_left_world_position + static_cast<float>(tree_left_x);
                                float tree_top_world_y_position = map_top_world_position + static_cast<float>(tree_top_y);
                                float tree_world_x_position = tree_left_world_x_position + tree_local_center.X;
                                float tree_world_y_position = tree_top_world_y_position + tree_local_center.Y;
                                tree_sprite.SetWorldPosition(tree_world_x_position, tree_world_y_position);

                                // ADD APPROPRIATE ANIMATIONS TO THE TREE.
                                const std::string TREE_SHAKE_ANIMATION_NAME = "TreeShake";
                                const bool NO_LOOPING = false;
                                const sf::Time TREE_SHAKE_DURATION = sf::seconds(0.6f);
                                const std::vector<MATH::IntRectangle> TREE_SHAKE_FRAMES =
                                {
                                    MATH::IntRectangle::FromLeftTopAndDimensions(32, 0, 16, 32),
                                    MATH::IntRectangle::FromLeftTopAndDimensions(64, 0, 16, 32),
                                    MATH::IntRectangle::FromLeftTopAndDimensions(32, 0, 16, 32),
                                    MATH::IntRectangle::FromLeftTopAndDimensions(80, 0, 16, 32),
                                    MATH::IntRectangle::FromLeftTopAndDimensions(32, 0, 16, 32),
                                    MATH::IntRectangle::FromLeftTopAndDimensions(64, 0, 16, 32),
                                    MATH::IntRectangle::FromLeftTopAndDimensions(32, 0, 16, 32),
                                    MATH::IntRectangle::FromLeftTopAndDimensions(80, 0, 16, 32),
                                    MATH::IntRectangle::FromLeftTopAndDimensions(32, 0, 16, 32)
                                };
                                std::shared_ptr<GRAPHICS::AnimationSequence> tree_shake_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                                    TREE_SHAKE_ANIMATION_NAME,
                                    NO_LOOPING,
                                    TREE_SHAKE_DURATION,
                                    TREE_SHAKE_FRAMES);
                                tree_sprite.AddAnimationSequence(tree_shake_animation);

                                // CREATE ANY FOOD ON THE TREE.
                                // Food will be randomly generated.
                                std::optional<OBJECTS::Food> food = std::nullopt;
                                unsigned int random_number_for_food_existing = random_number_generator.RandomNumber<unsigned int>();
                                bool food_on_tree = MATH::Number::IsEven(random_number_for_food_existing);
                                if (food_on_tree)
                                {
                                    // DETERMINE THE TYPE OF FOOD.
                                    // It will be randomly determined.
                                    OBJECTS::FoodType food_type = random_number_generator.RandomEnum<OBJECTS::FoodType>();

                                    // GET THE SPRITE FOR THE FOOD.
                                    // The food can only be created if the sprite was retrieved.
                                    std::shared_ptr<GRAPHICS::Sprite> food_sprite = RESOURCES::FoodGraphics::GetSprite(food_type, assets);
                                    if (food_sprite)
                                    {
                                        // CREATE THE FOOD.
                                        food = OBJECTS::Food();
                                        food->Type = food_type;
                                        food->Sprite = GRAPHICS::Sprite(*food_sprite);

                                        // The food should be positioned on the tree.
                                        food->Sprite.SetWorldPosition(tree_sprite.GetWorldPosition());
                                    }
                                }

                                // CREATE THE TREE.
                                OBJECTS::Tree tree;
                                tree.Sprite = tree_sprite;
                                tree.Food = food;
                                tile_map.Trees.push_back(tree);
                            }
                        }
                    }
                }

                // SET THE TILE MAP IN THE OVERWORLD.
                Overworld.TileMaps(column, row) = std::move(tile_map);
            }
        }
    }

    /// Populates the ark interior based on data read from in-memory assets.
    /// @param[in,out]  assets - The assets for the overworld.
    void World::PopulateArkInterior(RESOURCES::Assets& assets)
    {
        // LOAD THE TILESET TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> tileset_texture = assets.GetTexture(RESOURCES::AssetId::MAIN_TILESET_TEXTURE);
        if (!tileset_texture)
        {
            return;
        }

        // CREATE THE TILESET.
        MAPS::Tileset tileset(tileset_texture);

        // LOAD TILE MAPS FOR EACH LAYER.
        std::size_t layer_count = MAPS::DATA::ARK_INTERIOR_LAYER_DATA_FROM_BOTTOM_TO_TOP.size();
        for (std::size_t layer_index = 0; layer_index < layer_count; ++layer_index)
        {
            // CREATE AN EMPTY MAP GRID FOR THE CURRENT LAYER.
            auto& current_layer_data = MAPS::DATA::ARK_INTERIOR_LAYER_DATA_FROM_BOTTOM_TO_TOP[layer_index];
            ArkInterior.LayersFromBottomToTop.emplace_back(MAPS::World::ARK_INTERIOR_WIDTH_IN_TILE_MAPS, MAPS::World::ARK_INTERIOR_HEIGHT_IN_TILE_MAPS);
            auto& current_map_layer = ArkInterior.LayersFromBottomToTop.back();

            // LOAD TILE MAPS FOR EACH ROW.
            for (unsigned int row = 0; row < MAPS::World::ARK_INTERIOR_HEIGHT_IN_TILE_MAPS; ++row)
            {
                // LOAD TILE MAPS FOR EACH COLUMN.
                for (unsigned int column = 0; column < MAPS::World::ARK_INTERIOR_WIDTH_IN_TILE_MAPS; ++column)
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
            MAPS::MultiTileMapGrid& current_map_layer = ArkInterior.LayersFromBottomToTop.at(layer_index);

            MAPS::MultiTileMapGrid* previous_map_layer = nullptr;
            bool previous_layer_exists = (layer_index > 0);
            if (previous_layer_exists)
            {
                std::size_t previous_map_layer_index = layer_index - 1;
                previous_map_layer = &ArkInterior.LayersFromBottomToTop.at(previous_map_layer_index);
            }

            MAPS::MultiTileMapGrid* next_map_layer = nullptr;
            std::size_t last_layer_index = layer_count - 1;
            bool next_layer_exists = (layer_index < last_layer_index);
            if (next_layer_exists)
            {
                std::size_t next_map_layer_index = layer_index + 1;
                next_map_layer = &ArkInterior.LayersFromBottomToTop.at(next_map_layer_index);
            }

            // CHECK ALL MAPS IN THE CURRENT LAYER'S ROWS FOR STAIRCASES.
            for (unsigned int row = 0; row < MAPS::World::ARK_INTERIOR_HEIGHT_IN_TILE_MAPS; ++row)
            {
                // CHECK ALL MAPS IN THE CURRENT LAYER'S COLUMNS FOR STAIRCASES.
                for (unsigned int column = 0; column < MAPS::World::ARK_INTERIOR_WIDTH_IN_TILE_MAPS; ++column)
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
