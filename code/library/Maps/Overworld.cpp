#include "Debugging/DebugConsole.h"
#include "Maps/Data/OverworldMapData.h"
#include "Maps/Overworld.h"
#include "Maps/Tileset.h"
#include "Math/Number.h"
#include "Math/RandomNumberGenerator.h"
#include "Resources/FoodGraphics.h"

namespace MAPS
{
    /// Constructor.  Creates the the overworld in its initial state.
    /// @param[in]  world - The larger world this overworld is part of.
    Overworld::Overworld(MEMORY::NonNullRawPointer<World> world) :
        MapGrid(WIDTH_IN_TILE_MAPS, HEIGHT_IN_TILE_MAPS, world),
        AnimalsGoingIntoArk()
    {
        ResetToInitialState();
    }

    /// Resets the overworld to its initial state.  Useful for switching between different saved games.
    void Overworld::ResetToInitialState()
    {
        // CLEAR ANY ANIMALS GOING INTO THE ARK.
        // If a saved game is being reloaded, the animals should re-appear behind Noah since
        // they don't officially "transfer state" until they enter the ark.
        AnimalsGoingIntoArk.clear();

        // CLEAR ANY PREVIOUSLY BUILT ARK PIECES.
        // This prevents ark pieces left over from different saved games from still appearing as built
        // when playing a different saved game.
        constexpr bool UNBUILT = true;
        SetArkPiecesBuiltStatus(UNBUILT);

        // The rest of the code below effectively re-creates the game world, ensuring that
        // leftover entities in each tile map aren't preserved between saved games.

        // CREATE THE TILESET.
        Tileset tileset;

        // CREATE A RANDOM NUMBER GENERATOR FOR CREATING FOOD.
        MATH::RandomNumberGenerator random_number_generator;

        unsigned int tree_count = 0;
        unsigned int ark_piece_count = 0;

        // LOAD TILE MAPS FOR EACH ROW.
        for (unsigned int row = 0; row < HEIGHT_IN_TILE_MAPS; ++row)
        {
            // LOAD TILE MAPS FOR EACH COLUMN.
            for (unsigned int column = 0; column < WIDTH_IN_TILE_MAPS; ++column)
            {
                // GET THE CURRENT TILE MAP DATA.
                const auto& tile_map_data = DATA::OVERWORLD_MAP_DATA(column, row);

                // CALCULATE THE POSITION OF THE CURRENT TILE MAP.
                MATH::Vector2f map_center_world_position;

                float map_width_in_pixels = static_cast<float>(TileMap::WIDTH_IN_TILES * Tile::DIMENSION_IN_PIXELS<unsigned int>);
                float map_half_width_in_pixels = map_width_in_pixels / 2.0f;
                float map_left_world_position = static_cast<float>(column * map_width_in_pixels);
                map_center_world_position.X = map_left_world_position + map_half_width_in_pixels;

                float map_height_in_pixels = static_cast<float>(TileMap::HEIGHT_IN_TILES * Tile::DIMENSION_IN_PIXELS<unsigned int>);
                float map_half_height_in_pixels = map_height_in_pixels / 2.0f;
                float map_top_world_position = static_cast<float>(row * map_height_in_pixels);
                map_center_world_position.Y = map_top_world_position + map_half_height_in_pixels;

                // CREATE AN EMPTY TILE MAP.
                MATH::Vector2ui map_dimensions_in_tiles(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES);
                auto tile_map = std::make_shared<TileMap>(
                    TileMapType::OVERWORLD,
                    MEMORY::NonNullRawPointer<MultiTileMapGrid>(&MapGrid),
                    row,
                    column,
                    map_center_world_position,
                    map_dimensions_in_tiles);

                // CREATE TILES IN THE GROUND LAYER.
                for (unsigned int current_tile_y = 0; current_tile_y < TileMap::HEIGHT_IN_TILES; ++current_tile_y)
                {
                    // CREATE TILES FOR THIS ROW.
                    for (unsigned int current_tile_x = 0; current_tile_x < TileMap::WIDTH_IN_TILES; ++current_tile_x)
                    {
                        // CREATE THE CURRENT TILE.
                        TileId tile_id = (*tile_map_data.GroundLayer)(current_tile_x, current_tile_y);
                        std::shared_ptr<Tile> tile = tileset.CreateTile(tile_id);
                        bool tile_exists_in_tileset = (nullptr != tile);
                        if (!tile_exists_in_tileset)
                        {
                            // Skip to trying to create the next tile.  The layer
                            // simply won't have any tile at this location.
                            continue;
                        }

                        // SET THE TILE IN THE GROUND LAYER.
                        tile_map->Ground.SetTile(current_tile_x, current_tile_y, tile);
                    }
                }

                // POPULATE THE ARK LAYER IF ONE EXISTS.
                if (tile_map_data.ArkLayer)
                {
                    // CREATE PIECES IN THE ARK LAYER.
                    for (unsigned int current_tile_y = 0; current_tile_y < TileMap::HEIGHT_IN_TILES; ++current_tile_y)
                    {
                        // CREATE ARK PIECES FOR THIS ROW.
                        for (unsigned int current_tile_x = 0; current_tile_x < TileMap::WIDTH_IN_TILES; ++current_tile_x)
                        {
                            // CHECK IF THE TILE ID IS VALID.
                            // Some tiles in this layer may not be for valid ark pieces.
                            TileId tile_id = (*tile_map_data.ArkLayer)(current_tile_x, current_tile_y);
                            bool tild_id_valid = (tile_id > 0);
                            if (!tild_id_valid)
                            {
                                continue;
                            }

                            // CREATE THE ARK PIECE.
                            OBJECTS::ArkPiece ark_piece(tile_id);
                            MATH::Vector2f ark_piece_local_center = ark_piece.Sprite.Origin;
                            float tile_left_x_position = static_cast<float>(current_tile_x * Tile::DIMENSION_IN_PIXELS<unsigned int>);
                            float ark_piece_world_x_position = map_left_world_position + tile_left_x_position + ark_piece_local_center.X;
                            float tile_top_y_position = static_cast<float>(current_tile_y * Tile::DIMENSION_IN_PIXELS<unsigned int>);
                            float ark_piece_world_y_position = map_top_world_position + tile_top_y_position + ark_piece_local_center.Y;
                            ark_piece.Sprite.WorldPosition.X = ark_piece_world_x_position;
                            ark_piece.Sprite.WorldPosition.Y = ark_piece_world_y_position;

                            // ADD THE ARK PIECE TO THE TILE MAP.
                            tile_map->ArkPieces.push_back(ark_piece);

                            ++ark_piece_count;
                        }
                    }
                }

                // POPULATE THE OBJECT LAYER IF ONE EXISTS.
                if (tile_map_data.ObjectLayer)
                {
                    // CREATE ANY TREES IN THE LAYER.
                    for (unsigned int current_tile_y = 0; current_tile_y < TileMap::HEIGHT_IN_TILES; ++current_tile_y)
                    {
                        // CREATE ARK PIECES FOR THIS ROW.
                        for (unsigned int current_tile_x = 0; current_tile_x < TileMap::WIDTH_IN_TILES; ++current_tile_x)
                        {
                            // CHECK IF THE TILE ID IS VALID.
                            // Some tiles in this layer may not be for valid ark pieces.
                            uint8_t object_id = (*tile_map_data.ObjectLayer)(current_tile_x, current_tile_y);
                            const uint8_t TREE_OBJECT_ID = 1;
                            bool is_tree = (TREE_OBJECT_ID == object_id);
                            if (is_tree)
                            {
                                ++tree_count;

                                // DETERMINE THE SUB-RECTANGLE OF THE TEXTURE TO USE FOR THE TREE.
                                MATH::FloatRectangle TALL_TREE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(32.0f, 0.0f, 16.0f, 32.0f);
                                MATH::FloatRectangle tree_texture_sub_rectangle = TALL_TREE_TEXTURE_SUB_RECTANGLE;

                                // CREATE THE TREE'S SPRITE.
                                GRAPHICS::AnimatedSprite tree_sprite(GRAPHICS::Sprite(RESOURCES::AssetId::TREE_TEXTURE, tree_texture_sub_rectangle));
                                MATH::Vector2f tree_local_center = tree_sprite.CurrentFrameSprite.Origin;
                                auto tree_left_x = current_tile_x * Tile::DIMENSION_IN_PIXELS<unsigned int>;
                                auto tree_top_y = current_tile_y * Tile::DIMENSION_IN_PIXELS<unsigned int>;
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
                                // Food will be randomly generated unless food already exists on the tree from a previous state.
                                // This is to preserve the food that might exist from the new game intro sequence.
                                std::optional<OBJECTS::Food> food = std::nullopt;

                                bool previous_tree_exists = false;
                                TileMap* previous_tile_map_at_current_location = MapGrid.GetTileMap(row, column);
                                if (previous_tile_map_at_current_location)
                                {
                                    unsigned int previous_tile_map_tree_count = static_cast<unsigned int>(previous_tile_map_at_current_location->Trees.size());
                                    previous_tree_exists = (previous_tile_map_tree_count >= tree_count);
                                    if (previous_tree_exists)
                                    {
                                        unsigned int tree_index = tree_count - 1;
                                        OBJECTS::Tree& previous_tree = previous_tile_map_at_current_location->Trees[tree_index];
                                        food = previous_tree.Food;
                                    }
                                }

                                // If a previous tree didn't exist, then randomly put some food on the tree.
                                if (!previous_tree_exists)
                                {
                                    unsigned int random_number_for_food_existing = random_number_generator.RandomNumber<unsigned int>();
                                    bool food_on_tree = MATH::Number::IsEven(random_number_for_food_existing);
                                    if (food_on_tree)
                                    {
                                        // DETERMINE THE TYPE OF FOOD.
                                        // It will be randomly determined.
                                        OBJECTS::Food::TypeId food_type = random_number_generator.RandomEnum<OBJECTS::Food::TypeId>();

                                        // GET THE SPRITE FOR THE FOOD.
                                        // The food can only be created if the sprite was retrieved.
                                        std::shared_ptr<GRAPHICS::Sprite> food_sprite = RESOURCES::FoodGraphics::GetSprite(food_type);
                                        if (food_sprite)
                                        {
                                            // CREATE THE FOOD.
                                            food = OBJECTS::Food();
                                            food->Type = food_type;
                                            food->Count = OBJECTS::Food::COUNT_PER_INSTANCE_FROM_TREES;
                                            food->Sprite = GRAPHICS::Sprite(*food_sprite);

                                            // The food should be positioned on the tree.
                                            food->Sprite.WorldPosition = tree_sprite.GetWorldPosition();
                                        }
                                    }
                                }

                                // CREATE THE TREE.
                                OBJECTS::Tree tree;
                                tree.Sprite = tree_sprite;
                                tree.Food = food;
                                tile_map->Trees.push_back(tree);
                            }
                        }
                    }
                }

                // SET THE TILE MAP IN THE OVERWORLD.
                MapGrid.TileMaps(column, row) = tile_map;
            }
        }

        DEBUGGING::DebugConsole::WriteLine("Created tree count: ", tree_count);
        DEBUGGING::DebugConsole::WriteLine("Ark piece count: ", ark_piece_count);
    }

    /// Sets the built status of all ark pieces in the world.
    /// @param[in]  built - True if ark pieces should be considered built; false if not.
    void Overworld::SetArkPiecesBuiltStatus(const bool built)
    {
        // SET THE BUILD STATUS OF ALL ARK PIECES IN EACH TILE MAP.
        unsigned int tile_map_row_count = MapGrid.TileMaps.GetHeight();
        unsigned int tile_map_column_count = MapGrid.TileMaps.GetWidth();
        for (unsigned int tile_map_row_index = 0; tile_map_row_index < tile_map_row_count; ++tile_map_row_index)
        {
            for (unsigned int tile_map_column_index = 0; tile_map_column_index < tile_map_column_count; ++tile_map_column_index)
            {
                // GET THE CURRENT TILE MAP.
                TileMap* current_tile_map = MapGrid.GetTileMap(tile_map_row_index, tile_map_column_index);
                if (!current_tile_map)
                {
                    continue;
                }

                // SET THE BUILT STATUS EACH ARK PIECE IN THE CURRENT TILE MAP.
                for (OBJECTS::ArkPiece& ark_piece : current_tile_map->ArkPieces)
                {
                    ark_piece.Built = built;
                }
            }
        }
    }
}
