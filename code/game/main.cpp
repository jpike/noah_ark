// BIG TODO LIST:
// - Create new background music with better timing:
//      - For intro
//      - For new game intro
//      - For gameplay
// - Move animals more properly in ark
// - Add underground caves
// - Add special stuff for specific Bible verses
// - Allow planting of trees
// - Raft creation for crossing waterways
// - People...specifically Noah's family
// - Add gathering of family
// - Have animals cry if hit with axe
// - Flood cutscene graphics
// - Ark gameplay
// - Post-ark gameplay/cutscenes
// - Completed game credits
// - Cutscene/text boxes for transition into ark
// - Polish (custom fancy graphics) for inventory GUI
// - Better word wrapping with verses
// - Handle stopping music when quickly switching between states?

#include <chrono>
#include <cmath>
#include <ctime>
#include <exception>
#include <initializer_list>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Debugging/DebugConsole.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Input/InputController.h"
#include "Maps/Data/ArkInteriorTileMapData.h"
#include "Maps/Data/OverworldMapData.h"
#include "Maps/Tileset.h"
#include "Math/Number.h"
#include "Resources/AnimalSounds.h"
#include "Resources/AssetPackage.h"
#include "Resources/Assets.h"
#include "Resources/FoodGraphics.h"
#include "Resources/PredefinedAssetPackages.h"
#include "States/CreditsScreen.h"
#include "States/FloodCutscene.h"
#include "States/GameplayState.h"
#include "States/GameSelectionScreen.h"
#include "States/GameState.h"
#include "States/IntroSequence.h"
#include "States/NewGameIntroSequence.h"
#include "States/SavedGameData.h"
#include "States/TitleScreen.h"

/// The game exited successfully.
int EXIT_CODE_SUCCESS = 0;
/// A standard exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN = 1;
/// An unknown exception was caught in main() that caused the game to exit.
int EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN = 2;
/// Game assets failed to be loaded.
int EXIT_CODE_FAILURE_LOADING_ASSETS = 3;
/// The font failed to be loaded.
int EXIT_CODE_FAILURE_LOADING_FONT = 4;
/// Other generic failure.
int EXIT_CODE_FAILURE = 5;

/// Populates the overworld based on data read from in-memory assets.
/// @param[in,out]  assets - The assets for the overworld.
/// @param[in,out]  overworld - The overworld to populate.
void PopulateOverworld(
    RESOURCES::Assets& assets,
    MAPS::MultiTileMapGrid& overworld)
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
            overworld.TileMaps(column, row) = std::move(tile_map);
        }
    }
}

/// Populates the ark interior based on data read from in-memory assets.
/// @param[in,out]  assets - The assets for the overworld.
/// @param[in,out]  ark_interior - The ark interior to populate.
void PopulateArkInterior(RESOURCES::Assets& assets, MAPS::LayeredMultiTileMapGrids& ark_interior)
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
        ark_interior.LayersFromBottomToTop.emplace_back(MAPS::World::ARK_INTERIOR_WIDTH_IN_TILE_MAPS, MAPS::World::ARK_INTERIOR_HEIGHT_IN_TILE_MAPS);
        auto& current_map_layer = ark_interior.LayersFromBottomToTop.back();

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
        MAPS::MultiTileMapGrid& current_map_layer = ark_interior.LayersFromBottomToTop.at(layer_index);

        MAPS::MultiTileMapGrid* previous_map_layer = nullptr;
        bool previous_layer_exists = (layer_index > 0);
        if (previous_layer_exists)
        {
            std::size_t previous_map_layer_index = layer_index - 1;
            previous_map_layer = &ark_interior.LayersFromBottomToTop.at(previous_map_layer_index);
        }

        MAPS::MultiTileMapGrid* next_map_layer = nullptr;
        std::size_t last_layer_index = layer_count - 1;
        bool next_layer_exists = (layer_index < last_layer_index);
        if (next_layer_exists)
        {
            std::size_t next_map_layer_index = layer_index + 1;
            next_map_layer = &ark_interior.LayersFromBottomToTop.at(next_map_layer_index);
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

/// Loads the world.
/// @param[in,out]  assets - The assets to use for the world.
/// @return The world, if successfully loaded; null otherwise.
std::shared_ptr<MAPS::World> LoadWorld(RESOURCES::Assets& assets)
{
    // CREATE THE EMPTY WORLD.
    auto load_start_time = std::chrono::system_clock::now();
    std::shared_ptr<MAPS::World> world = std::make_shared<MAPS::World>();

    // POPULATE THE WORLD.
    PopulateOverworld(assets, world->Overworld);
    PopulateArkInterior(assets, world->ArkInterior);

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

    auto load_end_time = std::chrono::system_clock::now();

    // Overworld load time: 82,263,372 (file-based)
    // Overworld load time: 17,972,479 (array-based)
    // World load time (with ark interior): 31,132,753
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("World load time: ", load_time_diff.count());

    return world;
}

/// Loads the sounds/music for the game, adding them to the speakers.
/// @param[in,out]  assets - The assets from which to load the sounds/music.
/// @param[in,out]  speakers - The speakers to add sounds to.
void LoadSoundsAfterIntroAssets(RESOURCES::Assets& assets, AUDIO::Speakers& speakers)
{
    auto load_start_time = std::chrono::system_clock::now();

    // ADD EACH GENERIC SOUND EFFECT TO THE SPEAKERS.
    const std::array<RESOURCES::AssetId, 5> GENERIC_SOUND_EFFECT_IDS =
    {
        RESOURCES::AssetId::ARK_BUILDING_SOUND,
        RESOURCES::AssetId::AXE_HIT_SOUND,
        RESOURCES::AssetId::COLLECT_BIBLE_VERSE_SOUND,
        RESOURCES::AssetId::FOOD_PICKUP_SOUND,
        RESOURCES::AssetId::TREE_SHAKE_SOUND

    };
    for (const RESOURCES::AssetId sound_id : GENERIC_SOUND_EFFECT_IDS)
    {
        std::shared_ptr<sf::SoundBuffer> audio_samples = assets.GetSound(sound_id);
        if (audio_samples)
        {
            speakers.AddSound(sound_id, audio_samples);
        }
    }

    // ADD ALL OF THE ANIMAL SOUND EFFECTS TO THE SPEAKERS.
    for (int species_id = 0; species_id < static_cast<int>(OBJECTS::AnimalSpecies::COUNT); ++species_id)
    {
        OBJECTS::AnimalSpecies species = static_cast<OBJECTS::AnimalSpecies>(species_id);
        RESOURCES::AssetId animal_sound_id = RESOURCES::AnimalSounds::GetSound(species);
        std::shared_ptr<sf::SoundBuffer> audio_samples = assets.GetSound(animal_sound_id);
        if (audio_samples)
        {
            speakers.AddSound(animal_sound_id, audio_samples);
        }
    }

    // ADD ALL OF THE MUSIC TO THE SPEAKERS.
    std::shared_ptr<sf::Music> overworld_music = assets.GetMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC);
    if (overworld_music)
    {
        overworld_music->setLoop(true);
        speakers.AddMusic(RESOURCES::AssetId::OVERWORLD_BACKGROUND_MUSIC, overworld_music);
    }

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Sound load time: ", load_time_diff.count());
}

bool LoadIntroSequenceAssets(RESOURCES::Assets& asset_collection)
{
    auto load_start_time = std::chrono::system_clock::now();

    std::vector<RESOURCES::Asset> intro_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::INTRO_SEQUENCE_ASSET_PACKAGE_FILENAME);
    bool assets_loaded = !intro_assets.empty() && asset_collection.Populate(intro_assets);

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Intro sequence load time: ", load_time_diff.count());

    return assets_loaded;
}

std::shared_ptr<MAPS::World> LoadRemainingAssets(RESOURCES::Assets& assets, AUDIO::Speakers& speakers)
{
    auto load_start_time = std::chrono::system_clock::now();
    
    std::vector<RESOURCES::Asset> remaining_assets = RESOURCES::AssetPackage::ReadFile(RESOURCES::MAIN_ASSET_PACKAGE_FILENAME);
    bool assets_loaded = !remaining_assets.empty() && assets.Populate(remaining_assets);
    if (!assets_loaded)
    {
        return nullptr;
    }

    auto load_end_time = std::chrono::system_clock::now();
    auto load_time_diff = load_end_time - load_start_time;
    DEBUGGING::DebugConsole::WriteLine("Remaining asset raw load time: ", load_time_diff.count());

    LoadSoundsAfterIntroAssets(assets, speakers);
    std::shared_ptr<MAPS::World> world = LoadWorld(assets);
    return world;
}

/// The main entry point for the game.
/// Runs the Noah's Ark game until the user
/// chooses to exit or an error occurs.
/// @return     One of the exit codes defined in NoahArkGame.
int main()
{
    try
    {
        // CREATE THE WINDOW.
        // The display area is set to the dimensions of a single tile map.
        const unsigned int SCREEN_WIDTH_IN_PIXELS = 512;
        const unsigned int SCREEN_HEIGHT_IN_PIXELS = 384;
        const std::string GAME_TITLE = "Bible Games - Noah's Ark";
        std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS),
            GAME_TITLE);

        // Ensure that only one key event is generated for each key press.
        window->setKeyRepeatEnabled(false);

        // LOAD ASSETS NEEDED FOR THE INTRO SEQUENCE.
        // Some assets are explicitly loaded first as they are needed for initial
        // rendering of the intro sequence and title screen.  By loading them first,
        // quick startup time for the game can be maintained, showing the intro
        // sequence while other assets are being loaded.
        std::shared_ptr<RESOURCES::Assets> assets = std::make_shared<RESOURCES::Assets>();
        bool intro_sequence_assets_loaded = LoadIntroSequenceAssets(*assets);
        if (!intro_sequence_assets_loaded)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Failed to load intro sequence assets.");
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }

        auto colored_texture_shader = assets->GetShader(RESOURCES::AssetId::COLORED_TEXTURE_SHADER);
        if (!colored_texture_shader)
        {
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }

        std::shared_ptr<AUDIO::Speakers> speakers = std::make_shared<AUDIO::Speakers>();
        auto intro_music = assets->GetMusic(RESOURCES::AssetId::INTRO_MUSIC);
        if (!intro_music)
        {
            return EXIT_CODE_FAILURE_LOADING_ASSETS;
        }
        speakers->AddMusic(RESOURCES::AssetId::INTRO_MUSIC, intro_music);

        // LOAD REMAINING ASSETS.
        std::future< std::shared_ptr<MAPS::World> > world_being_loaded = std::async(LoadRemainingAssets, std::ref(*assets), std::ref(*speakers));

        // CREATE THE SCREEN.
        std::unique_ptr<GRAPHICS::Screen> screen = GRAPHICS::Screen::Create(SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS);
        if (!screen)
        {
            DEBUGGING::DebugConsole::WriteErrorLine("Failed to create game screen.");
            return EXIT_CODE_FAILURE;
        }

        // INITIALIZE THE RENDERER.
        GRAPHICS::Renderer renderer(
            colored_texture_shader,
            std::move(screen));

        // Fonts must be added separately.
        auto serif_font = assets->GetFont(RESOURCES::AssetId::SERIF_FONT_TEXTURE);
        auto default_font = assets->GetFont(RESOURCES::AssetId::FONT_TEXTURE);
        bool fonts_loaded = (serif_font && default_font);
        if (!fonts_loaded)
        {
            return EXIT_CODE_FAILURE_LOADING_FONT;
        }
        renderer.Fonts[RESOURCES::AssetId::SERIF_FONT_TEXTURE] = serif_font;
        renderer.Fonts[RESOURCES::AssetId::FONT_TEXTURE] = default_font;

        // INITIALIZE REMAINING SUBSYSTEMS.
        INPUT_CONTROL::InputController input_controller;
        STATES::IntroSequence intro_sequence;
        speakers->PlayMusic(RESOURCES::AssetId::INTRO_MUSIC);
        STATES::TitleScreen title_screen;
        STATES::CreditsScreen credits_screen;
        STATES::GameSelectionScreen game_selection_screen;
        STATES::NewGameIntroSequence new_game_intro_sequence;
        STATES::FloodCutscene flood_cutscene;
        STATES::GameplayState gameplay_state(speakers, assets);

        // DEFINE IMPORTANT SHADERS.
        // They are only needed in main, but it doesn't need to be loaded until main gameplay starts.
        std::shared_ptr<sf::Shader> time_of_day_shader = nullptr;

        // RUN THE GAME LOOP AS LONG AS THE WINDOW IS OPEN.
        STATES::GameState game_state = STATES::GameState::INTRO_SEQUENCE;
        sf::Clock game_loop_clock;
        sf::Time total_elapsed_time;
        while (window->isOpen())
        {
            // PROCESS WINDOW EVENTS.
            sf::Event event;
            while (window->pollEvent(event))
            {
                // HANDLE THE CURRENT EVENT BASED ON ITS TYPE.
                switch (event.type)
                {
                    case sf::Event::Closed:
                        // CLOSE THE WINDOW.
                        // The game will end later.
                        window->close();
                        break;
                    case sf::Event::LostFocus:
                        // DISABLE INPUT.
                        // This will prevent the user from accidentally moving
                        // the player until the window regains focus.
                        DEBUGGING::DebugConsole::WriteLine("Disabling input.");
                        input_controller.DisableInput();
                        break;
                    case sf::Event::GainedFocus:
                        // ENABLE INPUT.
                        // This will allow the user to resume moving the player
                        // now that the window has focus.
                        DEBUGGING::DebugConsole::WriteLine("Enabling input.");
                        input_controller.EnableInput();
                        break;
                }
            }

            // UPDATE AND DISPLAY THE GAME IN THE WINDOW.
            if (window->isOpen())
            {
                // READ USER INPUT.
                sf::Vector2i mouse_screen_position = sf::Mouse::getPosition(*window);
                input_controller.Mouse.ScreenPosition = MATH::Vector2f(
                    static_cast<float>(mouse_screen_position.x),
                    static_cast<float>(mouse_screen_position.y));
                input_controller.ReadInput();

                // GET THE ELAPSED TIME FOR THE NEW FRAME.
                sf::Time elapsed_time = game_loop_clock.restart();
                total_elapsed_time += elapsed_time;

                // UPDATE THE GAME'S CURRENT STATE.
                STATES::GameState next_game_state = game_state;
                switch (game_state)
                {
                    case STATES::GameState::INTRO_SEQUENCE:
                    {
                        // UPDATE THE INTRO SEQUENCE.
                        intro_sequence.Update(elapsed_time);

                        // MOVE TO THE TITLE SCREEN IF THE INTRO SEQUENCE HAS FINISHED.
                        bool intro_sequence_finished = intro_sequence.Completed();
                        if (intro_sequence_finished)
                        {
                            // The intro music isn't stopped before going to the next state
                            // to avoid a hard cutoff.  It is timed such that it should end
                            // shortly.
                            next_game_state = STATES::GameState::TITLE_SCREEN;
                        }
                        break;
                    }
                    case STATES::GameState::TITLE_SCREEN:
                        next_game_state = title_screen.Update(elapsed_time, input_controller);
                        break;
                    case STATES::GameState::CREDITS_SCREEN:
                        next_game_state = credits_screen.Update(elapsed_time, input_controller);
                        break;
                    case STATES::GameState::GAME_SELECTION_SCREEN:
                        next_game_state = game_selection_screen.Update(elapsed_time, input_controller);
                        break;
                    case STATES::GameState::NEW_GAME_INTRO_SEQUENCE:
                        next_game_state = new_game_intro_sequence.Update(elapsed_time, *speakers);
                        break;
                    case STATES::GameState::FLOOD_CUTSCENE:
                        next_game_state = flood_cutscene.Update(elapsed_time);
                        break;
                    case STATES::GameState::GAMEPLAY:
                        next_game_state = gameplay_state.Update(elapsed_time, input_controller, renderer.Camera);
                        break;
                }

                // CLEAR THE SCREEN OF THE PREVIOUSLY RENDERED FRAME.
                renderer.Screen->Clear();

                // RENDER THE CURRENT STATE OF THE GAME.
                switch (game_state)
                {
                    case STATES::GameState::INTRO_SEQUENCE:
                        intro_sequence.Render(renderer);
                        break;
                    case STATES::GameState::TITLE_SCREEN:
                        title_screen.Render(renderer);
                        break;
                    case STATES::GameState::CREDITS_SCREEN:
                        credits_screen.Render(renderer);
                        break;
                    case STATES::GameState::GAME_SELECTION_SCREEN:
                        game_selection_screen.Render(renderer);
                        break;
                    case STATES::GameState::NEW_GAME_INTRO_SEQUENCE:
                        new_game_intro_sequence.Render(renderer);
                        break;
                    case STATES::GameState::FLOOD_CUTSCENE:
                        flood_cutscene.Render(renderer);
                        break;
                    case STATES::GameState::GAMEPLAY:
                        gameplay_state.Render(renderer);
                        break;
                }

                // DISPLAY THE RENDERED FRAME IN THE WINDOW.
                renderer.Screen->RenderTarget.display();
                sf::Sprite screen_sprite(renderer.Screen->RenderTarget.getTexture());

                // For main gameplay, the world should be tinted based on the time of day most of the time.
                bool in_main_gameplay = (STATES::GameState::GAMEPLAY == game_state);
                if (in_main_gameplay)
                {
                    // MAKE SURE THE APPROPRIATE SHADER EXISTS.
                    // If the player is beginning a new game with God speaking to Noah, then the pulsing light
                    // shader should be used to help communicate that God is speaking to the player.
                    /// @todo   Might be better to have a fancier "spinning light" style-effect.
                    bool pulse_light_for_new_game_text = (!gameplay_state.NewGameInstructionsCompleted && colored_texture_shader);
                    if (pulse_light_for_new_game_text)
                    {
                        // COMPUTE THE TINT TO APPLY TO THE SCREEN.
                        // It should pulse based on elapsed time.
                        float elapsed_time_in_seconds = total_elapsed_time.asSeconds();
                        // The range of sin() is [-1, 1].  Since we want to compute an additional lighting factor
                        // to add to the base lighting amount, without making it too dark/too bright, the scale
                        // of this additional lighting is adjusted to be [-0.4, 0.6].
                        // The initial multiplication brings the range to [-0.5, 0.5].
                        constexpr float ADDITIONAL_LIGHTING_FACTOR_RANGE = 0.5f;
                        float additional_lighting_factor = ADDITIONAL_LIGHTING_FACTOR_RANGE * std::sinf(elapsed_time_in_seconds);
                        // An addition shifts it into the appropriate range.
                        constexpr float ADDITIONAL_LIGHTING_FACTOR_SHIFT_AMOUNT = 0.1f;
                        additional_lighting_factor += ADDITIONAL_LIGHTING_FACTOR_SHIFT_AMOUNT;

                        // To ensure that the lighting stays bright enough even as the pulsing occurs and also
                        // tends toward brighter (more indicative of God) than darker, the additional factor
                        // above is added to a base lighting amount.
                        // is added in.
                        constexpr float BASE_LIGHTING_AMOUNT = 1.0f;
                        float lighting_scale_factor = BASE_LIGHTING_AMOUNT + additional_lighting_factor;

                        // RENDER THE SCREEN WITH THE CURRENT LIGHTING.
                        constexpr float ALPHA_FOR_FULLY_OPAQUE = 1.0f;
                        sf::RenderStates pulsing_light_rendering = sf::RenderStates::Default;
                        colored_texture_shader->setUniform("color", sf::Glsl::Vec4(lighting_scale_factor, lighting_scale_factor, lighting_scale_factor, ALPHA_FOR_FULLY_OPAQUE));
                        colored_texture_shader->setUniform("texture", sf::Shader::CurrentTexture);
                        pulsing_light_rendering.shader = colored_texture_shader.get();
                        window->draw(screen_sprite, pulsing_light_rendering);
                    }
                    else if (time_of_day_shader)
                    {
                        // GET THE CURRENT COLOR SCALE BASED ON TIME-OF-DAY.
                        // If an error occurs getting the current time of day,
                        // the normal (maximum) color values will be used
                        // (the time-of-day shading feature just won't exist for those users).
                        float time_of_day_color_scale = 1.0f;
                        std::time_t* const JUST_GET_RETURNED_TIME = nullptr;
                        std::time_t current_posix_time = std::time(JUST_GET_RETURNED_TIME);
                        std::tm current_time;
                        errno_t get_local_time_return_code = localtime_s(&current_time, &current_posix_time);
                        const errno_t GET_LOCAL_TIME_SUCCESS_RETURN_CODE = 0;
                        bool current_time_retrieved_successfully = (GET_LOCAL_TIME_SUCCESS_RETURN_CODE == get_local_time_return_code);
                        if (current_time_retrieved_successfully)
                        {
                            // 0.4f is the darkest we can go and still have the screen
                            // remain reasonably visible.
                            const std::size_t HOUR_COUNT_PER_DAY = 24;
                            std::array<float, HOUR_COUNT_PER_DAY> HOUR_TO_COLOR_SCALE_LOOKUP = 
                            {
                                0.45f, // 12am
                                0.4f, // 1am (darkest time)
                                0.45f, // 2am
                                0.50f, // 3am
                                0.55f, // 4am
                                0.60f, // 5am
                                0.65f, // 6am
                                0.70f, // 7am
                                0.75f, // 8am
                                0.80f, // 9am
                                0.85f, // 10am
                                0.90f, // 11am
                                0.95f, // 12pm
                                1.0f, // 1pm (brightest time)
                                0.95f, // 2pm
                                0.90f, // 3pm
                                0.85f, // 4pm
                                0.80f, // 5pm
                                0.75f, // 6pm
                                0.70f, // 7pm
                                0.65f, // 8pm
                                0.60f, // 9pm
                                0.55f, // 10pm
                                0.50f, // 11pm
                            };

                            time_of_day_color_scale = HOUR_TO_COLOR_SCALE_LOOKUP[current_time.tm_hour];
                        }

                        // CONFIGURE THE SHADER.
                        time_of_day_shader->setUniform("color_scale", time_of_day_color_scale);
                        time_of_day_shader->setUniform("texture", sf::Shader::CurrentTexture);

                        // RENDER USING THE SHADER.
                        sf::RenderStates time_of_day_rendering = sf::RenderStates::Default;
                        time_of_day_rendering.shader = time_of_day_shader.get();
                        window->draw(screen_sprite, time_of_day_rendering);
                    }
                    else
                    {
                        // RENDER WITHOUT THE SHADER.
                        window->draw(screen_sprite);
                    }
                }
                else
                {
                    window->draw(screen_sprite);
                }

                window->display();

                // OVERRIDE GAME STATE SWITCHES WITH DEBUG KEY PRESSES.
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_INTRO_SEQUENCE_KEY))
                {
                    next_game_state = STATES::GameState::INTRO_SEQUENCE;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_TITLE_SCREEN_KEY))
                {
                    next_game_state = STATES::GameState::TITLE_SCREEN;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_CREDITS_SCREEN_KEY))
                {
                    next_game_state = STATES::GameState::CREDITS_SCREEN;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_GAME_SELECTION_SCREEN_KEY))
                {
                    next_game_state = STATES::GameState::GAME_SELECTION_SCREEN;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_NEW_GAME_INTRO_SEQUENCE_KEY))
                {
                    next_game_state = STATES::GameState::NEW_GAME_INTRO_SEQUENCE;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_FLOOD_CUTSCENE_KEY))
                {
                    next_game_state = STATES::GameState::FLOOD_CUTSCENE;
                }
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::DEBUG_SWITCH_TO_GAMEPLAY_KEY))
                {
                    next_game_state = STATES::GameState::GAMEPLAY;
                }

                // PERFORM ADDITIONAL STEPS NEEDED TO TRANSITION TO CERTAIN NEW GAME STATES.
                bool game_state_changed = (next_game_state != game_state);
                if (game_state_changed)
                {
                    // CHANGE THE GAME'S STATE.
                    game_state = next_game_state;
                    switch (game_state)
                    {
                        case STATES::GameState::CREDITS_SCREEN:
                            // RESET THE ELAPSED TIME FOR THE CREDITS SCREEN.
                            credits_screen.ElapsedTime = sf::Time::Zero;
                            break;
                        case STATES::GameState::GAME_SELECTION_SCREEN:
                            game_selection_screen.LoadSavedGames();
                            game_selection_screen.CurrentSubState = STATES::GameSelectionScreen::SubState::LISTING_GAMES;
                            break;
                        case STATES::GameState::NEW_GAME_INTRO_SEQUENCE:
                        {
                            // LOAD THE APPROPRIATE MUSIC INTO THE SPEAKERS.
                            auto new_game_intro_music = assets->GetMusic(RESOURCES::AssetId::NEW_GAME_INTRO_MUSIC);
                            speakers->AddMusic(RESOURCES::AssetId::NEW_GAME_INTRO_MUSIC, new_game_intro_music);

                            // RESET THE INTRO SEQUENCE TO THE BEGINNING.
                            new_game_intro_sequence.ResetToBeginning();
                            break;
                        }
                        case STATES::GameState::FLOOD_CUTSCENE:
                            // RESET THE ELAPSED TIME FOR THE CUTSCENE.
                            flood_cutscene.ElapsedTime = sf::Time::Zero;
                            break;
                        case STATES::GameState::GAMEPLAY:
                        {
                            // LOAD THE GAME'S SAVE FILE.
                            // The empty check is for handling the case where no saved games exist yet, primarily when
                            // switching between states in debug mode.
                            std::shared_ptr<STATES::SavedGameData> saved_game_data;
                            if (!game_selection_screen.SavedGames.empty())
                            {
                                saved_game_data = game_selection_screen.SavedGames.at(game_selection_screen.SelectedGameIndex);
                            }
                            bool saved_game_data_loaded = (nullptr != saved_game_data);
                            if (!saved_game_data_loaded)
                            {
                                // USE THE DEFAULT SAVED GAME DATA FOR A NEW GAME.
                                saved_game_data = std::make_shared<STATES::SavedGameData>(STATES::SavedGameData::DefaultSavedGameData());
                            }

                            // INITIALIZE THE GAMEPLAY STATE.
                            // This protection against the world being invalid is primarily to handle
                            // debug code for quickly switching between states.
                            std::shared_ptr<MAPS::World> world;
                            bool world_loading_valid = world_being_loaded.valid();
                            if (world_loading_valid)
                            {
                                world = world_being_loaded.get();
                            }
                            else
                            {
                                world = gameplay_state.World;
                            }
                            if (!world)
                            {
                                return EXIT_FAILURE;
                            }

                            bool gameplay_state_initialized = gameplay_state.Initialize(
                                SCREEN_WIDTH_IN_PIXELS,
                                saved_game_data,
                                world);
                            if (!gameplay_state_initialized)
                            {
                                return EXIT_FAILURE;
                            }

                            // FOCUS THE CAMERA ON THE PLAYER.
                            MATH::Vector2f player_start_world_position = gameplay_state.NoahPlayer->GetWorldPosition();
                            renderer.Camera.SetCenter(player_start_world_position);

                            // GET IMPORTANT SHADERS.
                            time_of_day_shader = assets->GetShader(RESOURCES::AssetId::TIME_OF_DAY_SHADER);
                            if (!time_of_day_shader)
                            {
                                return EXIT_FAILURE;
                            }

                            break;
                        }
                    }
                }
            }
        }

        // INDICATE THAT THE GAME EXITED SUCCESSFULLY.
        return EXIT_CODE_SUCCESS;
    }
    catch (std::exception& exception)
    {
        DEBUGGING::DebugConsole::WriteErrorLine("Standard exception in main: " + std::string(exception.what()));
        return EXIT_CODE_FAILURE_STANDARD_EXCEPTION_IN_MAIN;
    }
    catch (...)
    {   
        DEBUGGING::DebugConsole::WriteErrorLine("Unknown exception in main.");
        return EXIT_CODE_FAILURE_UNKNOWN_EXCEPTION_IN_MAIN;
    }
}
