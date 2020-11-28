#include <cassert>
#include <unordered_set>
#include "Collision/CollisionDetectionAlgorithms.h"
#include "Maps/TileMap.h"
#include "Maps/World.h"

namespace MAPS
{
    /// Creates an empty tile map.  Parameters have default values to allow
    /// for default construction.
    /// @param[in]  type - The type of tile map.
    /// @param[in]  map_grid - The map grid this tile map is part of.
    /// @param[in]  grid_row_index - The 0-based index (from the top) of
    ///     the tile map as located in its grid.
    /// @param[in]  grid_column_index - The 0-based index (from the left) of
    ///     the tile map as located in its grid.
    /// @param[in]  center_world_position - The world position of the center
    ///     of the tile map.
    /// @param[in]  dimensions_in_tiles - The dimensions of the map in tiles.
    /// @param[in]  tile_dimension_in_pixels - The dimensions (both width and height)
    ///     of an individual tile in the map (in units of pixels).
    TileMap::TileMap(
        const TileMapType type,
        MEMORY::NonNullRawPointer<MultiTileMapGrid> map_grid,
        const unsigned int grid_row_index,
        const unsigned int grid_column_index,
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles) :
    Type(type),
    MapGrid(map_grid),
    GridRowIndex(grid_row_index),
    GridColumnIndex(grid_column_index),
    Ground(center_world_position, dimensions_in_tiles, Tile::DIMENSION_IN_PIXELS<unsigned int>),
    Trees(),
    FallingFood(),
    FoodOnGround(),
    DustClouds(),
    WoodLogs(),
    ArkPieces(),
    RoamingAnimals(),
    AnimalPens(),
    ExitPoints()
    {}

    /// Gets the center world position of the tile map.
    /// @return The center world position of the tile map.
    MATH::Vector2f TileMap::GetCenterWorldPosition() const
    {
        return Ground.CenterWorldPosition;
    }

    /// Gets the dimensions (width, height) of the tile map, in units of tiles.
    /// @return The dimensions of the tile map, in tiles.
    MATH::Vector2ui TileMap::GetDimensionsInTiles() const
    {
        MATH::Vector2ui dimensions_in_tiles(
            Ground.Tiles.GetWidth(),
            Ground.Tiles.GetHeight());

        return dimensions_in_tiles;
    }

    /// Gets the bounding box of the tile map in world coordinates.
    /// @return The world bounding box of the tile map.
    MATH::FloatRectangle TileMap::GetWorldBoundingBox() const
    {
        // GET THE CENTER OF THE TILE MAP.
        MATH::Vector2f center_world_position = GetCenterWorldPosition();

        // GET THE FULL DIMENSIONS OF THE TILE MAP.
        MATH::Vector2ui dimensions_in_tiles = GetDimensionsInTiles();
        float width_in_pixels = static_cast<float>(dimensions_in_tiles.X * Ground.TileDimensionInPixels);
        float height_in_pixels = static_cast<float>(dimensions_in_tiles.Y * Ground.TileDimensionInPixels);

        // FORM THE WORLD BOUNDING BOX.
        MATH::FloatRectangle world_bounding_box = MATH::FloatRectangle::FromCenterAndDimensions(
            center_world_position.X,
            center_world_position.Y,
            width_in_pixels,
            height_in_pixels);
        return world_bounding_box;
    }

    /// Gets the tile that includes the specified world coordinates.
    /// @param[in]  world_x_position - The world x position of the tile to retrieve.
    /// @param[in]  world_y_position - The world y position of the tile to retrieve.
    /// @return The tile at the specified location, if one exists; null otherwise.
    std::shared_ptr<MAPS::Tile> TileMap::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        std::shared_ptr<MAPS::Tile> tile = Ground.GetTileAtWorldPosition(world_x_position, world_y_position);
        return tile;
    }

    /// Gets an ark piece at the specified world position, if one exists.
    /// @param[in]  world_position - The world position for which to retrieve an ark piece.
    /// @return The ark piece at the specified world position, if one exists; null otherwise.
    OBJECTS::ArkPiece* TileMap::GetArkPieceAtWorldPosition(const MATH::Vector2f& world_position)
    {
        // SEARCH FOR AN ARK PIECE THAT CONTAINS THE SPECIFIED WORLD POSITION.
        for (auto& ark_piece : ArkPieces)
        {
            // CHECK IF THE CURRENT ARK PIECE INCLUDES THE WORLD POSITION.
            MATH::FloatRectangle ark_piece_bounding_box = ark_piece.Sprite.GetWorldBoundingBox();
            bool ark_piece_contains_world_position = ark_piece_bounding_box.Contains(world_position.X, world_position.Y);
            if (ark_piece_contains_world_position)
            {
                // RETURN THE ARK PIECE.
                return &ark_piece;
            }
        }

        // INDICATE THAT NO ARK PIECE COULD BE FOUND.
        return nullptr;
    }

    /// Gets an exit point at the specified world position, if one exists.
    /// @param[in]  world_position - The world position for which to retrieve an exit point.
    /// @return The exit point at the specified world position, if one exists; null otherwise.
    MAPS::ExitPoint* TileMap::GetExitPointAtWorldPosition(const MATH::Vector2f& world_position)
    {
        // SEARCH FOR AN EXIT POINT THAT CONTAINS THE SPECIFIED WORLD POSITION.
        for (auto& exit_point : ExitPoints)
        {
            // CHECK IF THE CURRENT EXIT POINT INCLUDES THE WORLD POSITION.
            bool exit_point_contains_world_position = exit_point.BoundingBox.Contains(world_position.X, world_position.Y);
            if (exit_point_contains_world_position)
            {
                // CHECK IF THE EXIT POINT IS PART OF AN ARK PIECE.
                OBJECTS::ArkPiece* ark_piece_for_exit = GetArkPieceAtWorldPosition(world_position);
                if (ark_piece_for_exit)
                {
                    // MAKE SURE THE ARK PIECE HAS BEEN BUILT AND IS FOR A DOORWAY.
                    // If the ark piece hasn't been built yet or isn't a doorway,
                    // then the exit point shouldn't be usable yet.
                    bool ark_piece_is_built_external_doorway = (ark_piece_for_exit->Built && ark_piece_for_exit->IsExternalDoorway);
                    if (ark_piece_is_built_external_doorway)
                    {
                        // RETURN THE USABLE EXIT POINT.
                        return &exit_point;
                    }
                }
                else
                {
                    // RETURN THE EXIT POINT.
                    return &exit_point;
                }
            }
        }

        // INDICATE THAT NO EXIT POINT COULD BE FOUND.
        return nullptr;
    }

    /// Updates the tile map based on elapsed time, simulating interactions within the map.
    /// @param[in]  objects_can_move_across_space - True if objects are allowed to move across space;
    ///     false if not.  There are some situations where it can be useful to still have "stationary"
    ///     objects still animate but have other objects that would move across space in the tile map
    ///     not be updated.
    /// @param[in,out]  current_game_data - The game data to potentially update.
    /// @param[in,out]  gaming_hardware - The hardware for input and output.
    void TileMap::Update(
        const bool objects_can_move_across_space,
        STATES::SavedGameData& current_game_data,
        HARDWARE::GamingHardware& gaming_hardware)
    {
        // UPDATE THE CURRENT TILE MAP'S TILES.
        unsigned int map_height_in_tiles = Ground.Tiles.GetHeight();
        unsigned int map_width_in_tiles = Ground.Tiles.GetWidth();
        for (unsigned int tile_y = 0; tile_y < map_height_in_tiles; ++tile_y)
        {
            // UPDATE TILES ACROSS THE CURRENT ROW.
            for (unsigned int tile_x = 0; tile_x < map_width_in_tiles; ++tile_x)
            {
                // UPDATE THE CURRENT TILE.
                auto current_tile = Ground.Tiles(tile_x, tile_y);
                if (current_tile)
                {
                    current_tile->Sprite.Play();
                    current_tile->Sprite.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
                }
            }
        }

        // UPDATE THE CURRENT TILE MAP'S ROAMING ANIMALS.
        for (auto& animal : RoamingAnimals)
        {
            animal->Sprite.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
        }

        // ANIMATE ANIMALS IN PENS.
        for (auto& animal_pen : AnimalPens)
        {
            for (auto& animal : animal_pen.Animals)
            {
                animal->Sprite.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);
            }
        }

        // UPDATE THE CURRENT TILE MAP'S TREES.
        for (auto tree = Trees.begin(); tree != Trees.end(); ++tree)
        {
            // UPDATE THE TREE.
            tree->Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);

            // START PLAYING THE TREE SHAKING SOUND EFFECT IF APPROPRIATE.
            bool is_shaking = tree->IsShaking();
            if (is_shaking)
            {
                // ONLY START PLAYING THE SOUND IF IT ISN'T ALREADY PLAYING.
                // This results in a smoother sound experience.
                bool tree_shake_sound_playing = gaming_hardware.Speakers->SoundIsPlaying(RESOURCES::AssetId::TREE_SHAKE_SOUND);
                if (!tree_shake_sound_playing)
                {
                    gaming_hardware.Speakers->PlaySoundEffect(RESOURCES::AssetId::TREE_SHAKE_SOUND);
                }
            }
        }

        // UPDATE THE CURRENT TILE MAP'S DUST CLOUDS.
        for (auto dust_cloud = DustClouds.begin(); dust_cloud != DustClouds.end();)
        {
            // UPDATE THE CURRENT DUST CLOUD.
            dust_cloud->Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);

            // REMOVE THE DUST CLOUD IF IT HAS DISAPPEARED.
            bool dust_cloud_disappeared = dust_cloud->HasDisappeared();
            if (dust_cloud_disappeared)
            {
                // REMOVE THE DUST CLOUD.
                dust_cloud = DustClouds.erase(dust_cloud);
            }
            else
            {
                // MOVE TO UPDATING THE NEXT DUST CLOUD.
                ++dust_cloud;
            }
        }

        // CHECK IF MOVABLE OBJECTS CAN BE UPDATED.
        if (!objects_can_move_across_space)
        {
            // No more updates are needed.
            return;
        }

        // UPDATE ANY FALLING FOOD.
        for (auto food = FallingFood.begin(); food != FallingFood.end();)
        {
            // UPDATE THE CURRENT FOOD ITEM.
            food->Fall(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);

            // TRANSFER THE FOOD OVER IF IT HAS FINISHED FALLING.
            bool food_finished_falling = food->FinishedFalling();
            if (food_finished_falling)
            {
                FoodOnGround.push_back(food->FoodItem);
                food = FallingFood.erase(food);
            }
            else
            {
                // MOVE TO THE NEXT FALLING FOOD ITEM.
                ++food;
            }
        }

        // MOVE ANIMALS.
        MoveAnimals(current_game_data, gaming_hardware);
    }

    /// Moves animals in the tile map.
    /// @param[in,out]  current_game_data - The current game's save data.
    /// @param[in,out]  gaming_hardware - The gaming hardware for input and output.
    void TileMap::MoveAnimals(STATES::SavedGameData& current_game_data, HARDWARE::GamingHardware& gaming_hardware)
    {
        // MAKE SURE THIS TILE MAP IS PART OF A LARGER AREA.
        if (!MapGrid)
        {
            return;
        }

        // CHECK IF THE CURRENT TILE MAP HAS A VISIBLE EXTERNAL ARK DOORWAY.
        // This is how animals following Noah get transferred into the ark.
        bool inside_ark = (TileMapType::ARK_INTERIOR == Type);
        const OBJECTS::ArkPiece* doorway_into_ark = nullptr;
        for (const OBJECTS::ArkPiece& ark_piece : ArkPieces)
        {
            bool is_doorway_into_ark = !inside_ark && ark_piece.Built && ark_piece.IsExternalDoorway;
            if (is_doorway_into_ark)
            {
                doorway_into_ark = &ark_piece;
                break;
            }
        }
        if (doorway_into_ark)
        {
            // TRANSFER THE ANIMALS CURRENTLY FOLLOWING NOAH OVER TO MOVING INTO THE ARK.
            MapGrid->World->Overworld.AnimalsGoingIntoArk.insert(
                MapGrid->World->Overworld.AnimalsGoingIntoArk.cend(),
                MapGrid->World->NoahPlayer->Inventory.FollowingAnimals.Animals.cbegin(),
                MapGrid->World->NoahPlayer->Inventory.FollowingAnimals.Animals.cend());
            MapGrid->World->NoahPlayer->Inventory.FollowingAnimals.Animals.clear();

            // MOVE THE ANIMALS GOING INTO THE ARK CLOSER INTO THE ARK.
            MATH::FloatRectangle ark_doorway_bounding_box = doorway_into_ark->Sprite.GetWorldBoundingBox();
            MATH::Vector2f ark_doorway_world_position = doorway_into_ark->Sprite.WorldPosition;
            MAPS::ExitPoint* entry_point_into_ark = GetExitPointAtWorldPosition(ark_doorway_world_position);
            for (auto animal = MapGrid->World->Overworld.AnimalsGoingIntoArk.begin(); animal != MapGrid->World->Overworld.AnimalsGoingIntoArk.end(); )
            {
                // UPDATE THE ANIMAL'S ANIMATION.
                (*animal)->Sprite.Update(gaming_hardware.Clock.ElapsedTimeSinceLastFrame);

                // DETERMINE THE DIRECTION FROM THE ANIMAL TO THE DOORWAY.
                MATH::Vector2f animal_world_position = (*animal)->Sprite.GetWorldPosition();
                MATH::Vector2f animal_to_ark_doorway_vector = ark_doorway_world_position - animal_world_position;
                MATH::Vector2f animal_to_ark_doorway_direction = MATH::Vector2f::Normalize(animal_to_ark_doorway_vector);

                // CALCULATE THE DISTANCE THE ANIMAL NEEDS TO MOVE.
                const OBJECTS::AnimalType& current_animal_type = (*animal)->Type;
                float elapsed_time_in_seconds = gaming_hardware.Clock.ElapsedTimeSinceLastFrame.asSeconds();
                float animal_move_distance_in_pixels = current_animal_type.MoveSpeedInPixelsPerSecond * elapsed_time_in_seconds;
                MATH::Vector2f animal_move_vector = MATH::Vector2f::Scale(animal_move_distance_in_pixels, animal_to_ark_doorway_direction);

                // MOVE THE ANIMAL.
                MATH::Vector2f new_animal_world_position = animal_world_position + animal_move_vector;
                (*animal)->Sprite.SetWorldPosition(new_animal_world_position);

                // TRANSFER THE ANIMAL INTO THE ARK IF IT HAS REACHED THE DOORWAY.
                MATH::FloatRectangle animal_bounding_box = (*animal)->Sprite.GetWorldBoundingBox();
                bool animal_reached_doorway = ark_doorway_bounding_box.Intersects(animal_bounding_box);
                if (animal_reached_doorway)
                {
                    // UPDATE THE ANIMAL STATISTICS.
                    INVENTORY::AnimalCollectionStatistics& current_animal_collection_statistics = current_game_data.CollectedAnimalsBySpeciesThenGender[current_animal_type.Species][current_animal_type.Gender];
                    --current_animal_collection_statistics.FollowingPlayerCount;
                    ++current_animal_collection_statistics.InArkCount;

                    // ADD THE ANIMAL INTO THE APPROPRIATE TILE MAP OF THE ARK.
                    // This check is a precaution.  There should always be an entry point into the ark for the doorway.
                    assert(entry_point_into_ark);
                    if (entry_point_into_ark)
                    {
                        MAPS::TileMap* entry_room_inside_ark = entry_point_into_ark->NewTileMap;
                        assert(entry_room_inside_ark);
                        if (entry_room_inside_ark)
                        {
                            entry_room_inside_ark->MapGrid->World->Ark.AddAnimalToPen(*animal, current_game_data);
                        }
                    }

                    // REMOVE THE ANIMAL FROM ITS STATE OF TRYING TO ENTER THE ARK.
                    animal = MapGrid->World->Overworld.AnimalsGoingIntoArk.erase(animal);
                }
                else
                {
                    // MOVE TO UPDATING THE NEXT ANIMAL.
                    ++animal;
                }
            }
        }

        // MOVE EACH ANIMAL IN THE TILE MAP CLOSER TO NOAH IF THEY'RE OUTSIDE.
        for (auto& animal : RoamingAnimals)
        {
            // DETERMINE THE DIRECTION FROM THE ANIMAL TO THE PLAYER.
            // The animal should move closer to Noah based on Genesis 6:20.
            MATH::Vector2f noah_world_position = MapGrid->World->NoahPlayer->GetWorldPosition();
            MATH::Vector2f animal_world_position = animal->Sprite.GetWorldPosition();
            MATH::Vector2f animal_to_noah_vector = noah_world_position - animal_world_position;
            MATH::Vector2f animal_to_noah_direction = MATH::Vector2f::Normalize(animal_to_noah_vector);

            // CALCULATE THE DISTANCE THE ANIMAL NEEDS TO MOVE.
            float elapsed_time_in_seconds = gaming_hardware.Clock.ElapsedTimeSinceLastFrame.asSeconds();
            float animal_move_distance_in_pixels = animal->Type.MoveSpeedInPixelsPerSecond * elapsed_time_in_seconds;
            MATH::Vector2f animal_move_vector = MATH::Vector2f::Scale(animal_move_distance_in_pixels, animal_to_noah_direction);

            // DETERMINE THE TYPES OF TILES THE ANIMAL IS ALLOWED TO MOVE OVER.
            std::unordered_set<MAPS::TileType::Id> tile_types_allowed_to_move_over =
            {
                MAPS::TileType::SAND,
                MAPS::TileType::GRASS,
                MAPS::TileType::BROWN_DIRT,
                MAPS::TileType::GRAY_STONE
            };
            bool animal_can_fly = animal->Type.CanFly();
            bool animal_can_swim = animal->Type.CanSwim();
            bool animal_move_move_over_water = (animal_can_fly || animal_can_swim);
            if (animal_move_move_over_water)
            {
                // LET THE ANIMAL MOVE OVER WATER.
                tile_types_allowed_to_move_over.insert(MAPS::TileType::WATER_TYPES.cbegin(), MAPS::TileType::WATER_TYPES.cend());
            }

            // MOVE THE ANIMAL.
            MATH::FloatRectangle animal_world_bounding_box = animal->Sprite.GetWorldBoundingBox();
            bool allow_movement_over_solid_objects = animal_can_fly;
            MATH::Vector2f new_animal_world_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                animal_world_bounding_box,
                animal_move_vector,
                tile_types_allowed_to_move_over,
                allow_movement_over_solid_objects,
                *MapGrid);
            animal->Sprite.SetWorldPosition(new_animal_world_position);
        }

        // MOVE ANIMALS INSIDE PENS IN THE ARK.
        for (auto& animal_pen : AnimalPens)
        {
            for (auto& animal : animal_pen.Animals)
            {
                // RANDOMLY DETERMINE HOW MUCH THE ANIMAL SHOULD MOVE.
                // In general, movement this way is small enough to work well and not be a major problem.
                // However, it is a bit jittery, so we might want to tweak it a bit (possibly reduce amount
                // or reduce the randomness).  The random number generation is done to help reduce the amount
                // of overall movement by not moving sometimes.  The upper threshold for this movement is
                // largely arbitrary but has been tuned to have relatively frequent movement without too
                // much jitter.
                constexpr unsigned int MAX_PERCENTAGE = 100;
                constexpr unsigned int MOVEMENT_THRESHOLD = 8;
                unsigned int random_number_for_animal_movement = gaming_hardware.RandomNumberGenerator.RandomNumberLessThan(MAX_PERCENTAGE);
                bool move_animal = (random_number_for_animal_movement < MOVEMENT_THRESHOLD);
                if (!move_animal)
                {
                    continue;
                }

                float elapsed_time_in_seconds = gaming_hardware.Clock.ElapsedTimeSinceLastFrame.asSeconds();
                float normal_animal_move_speed_in_pixels_per_second = animal->Type.MoveSpeedInPixelsPerSecond;
                float animal_move_distance_in_pixels = normal_animal_move_speed_in_pixels_per_second * elapsed_time_in_seconds;
                constexpr float MIN_DIRECTION_VECTOR_COMPONENT = -1.0f;
                constexpr float MAX_DIRECTION_VECTOR_COMPONENT = 1.0f;
                MATH::Vector2f animal_direction_vector;
                animal_direction_vector.X = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(
                    MIN_DIRECTION_VECTOR_COMPONENT,
                    MAX_DIRECTION_VECTOR_COMPONENT);
                animal_direction_vector.Y = gaming_hardware.RandomNumberGenerator.RandomInRange<float>(
                    MIN_DIRECTION_VECTOR_COMPONENT,
                    MAX_DIRECTION_VECTOR_COMPONENT);

                MATH::Vector2f animal_move_vector = MATH::Vector2f::Scale(animal_move_distance_in_pixels, animal_direction_vector);

                // MOVE THE ANIMAL.
                std::unordered_set<MAPS::TileType::Id> tile_types_allowed_to_move_over =
                {
                    MAPS::TileType::ANIMAL_PEN_GROUND
                };
                MATH::FloatRectangle animal_world_bounding_box = animal->Sprite.GetWorldBoundingBox();
                constexpr bool NO_MOVEMENT_OVER_SOLID_OBJECTS = false;
                MATH::Vector2f new_animal_world_position = COLLISION::CollisionDetectionAlgorithms::MoveObject(
                    animal_world_bounding_box,
                    animal_move_vector,
                    tile_types_allowed_to_move_over,
                    NO_MOVEMENT_OVER_SOLID_OBJECTS,
                    *MapGrid);
                animal->Sprite.SetWorldPosition(new_animal_world_position);
            }
        }
    }
}