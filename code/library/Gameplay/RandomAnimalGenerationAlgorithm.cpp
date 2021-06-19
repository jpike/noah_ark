#include "Debugging/DebugConsole.h"
#include "Gameplay/RandomAnimalGenerationAlgorithm.h"
#include "Resources/AnimalGraphics.h"
#include "Resources/AnimalSounds.h"

namespace GAMEPLAY
{
    /// Attempts to generate an animal that can be placed in the provided tile map.
    /// The randomly generated animal will be one that hasn't been fully collected
    /// (both genders, appropriate number of) in the player's inventory.
    /// @param[in]  noah_player - The Noah player in the overworld.
    /// @param[in]  tile_map - The tile map in which the animal needs to be generated.
    /// @param[in,out]  random_number_generator - The random number generator to use
    ///     for the algorithm.
    /// @return An animal, if one was successfully generated; null otherwise.
    std::shared_ptr<OBJECTS::Animal> RandomAnimalGenerationAlgorithm::GenerateAnimal(
        const STATES::SavedGameData& current_game_data,
        const OBJECTS::Noah& noah_player,
        const MAPS::TileMap& tile_map,
        MATH::RandomNumberGenerator& random_number_generator)
    {
        // DETERMINE THE SPECIES OF ANIMAL TO GENERATE.
        OBJECTS::AnimalSpecies::Value random_species = random_number_generator.RandomEnum<OBJECTS::AnimalSpecies::Value>();
        DEBUGGING::DebugConsole::WriteLine("Random species: ", random_species);

        // DETERMINE THE GENDER OF ANIMAL TO GENERATE.
        OBJECTS::AnimalGender::Value random_gender = random_number_generator.RandomEnum<OBJECTS::AnimalGender::Value>();
        DEBUGGING::DebugConsole::WriteLine("Random gender: ", random_gender);

        // CHECK IF THE ANIMAL TYPE HAS BEEN FULLY COLLECTED.
        OBJECTS::AnimalType animal_type(random_species, random_gender);
        bool animal_type_fully_collected = current_game_data.AnimalTypeFullyCollected(animal_type);
        if (animal_type_fully_collected)
        {
            DEBUGGING::DebugConsole::WriteLine("Initial animal type fully collected.");

            // CHECK IF THE ANIMAL WITH THE OPPOSITE GENDER CAN BE COLLECTED.
            OBJECTS::AnimalGender::Value opposite_gender = static_cast<OBJECTS::AnimalGender::Value>((random_gender + 1) % OBJECTS::AnimalGender::COUNT);
            OBJECTS::AnimalType opposite_gender_animal_type = OBJECTS::AnimalType(random_species, opposite_gender);
            animal_type_fully_collected = current_game_data.AnimalTypeFullyCollected(opposite_gender_animal_type);
            if (animal_type_fully_collected)
            {
                // SEARCH FOR THE NEXT AVAILABLE RANDOM SPECIES.
                // Pure random generation isn't enough to collect all animals within a reasonable
                // amount of time, so continuing to search for the next available animal type
                // if a way to allow an animal to likely still be generated.
                for (OBJECTS::AnimalSpecies::Value next_species = static_cast<OBJECTS::AnimalSpecies::Value>(random_species + 1);
                    next_species != random_species;
                    next_species = static_cast<OBJECTS::AnimalSpecies::Value>((next_species + 1) % OBJECTS::AnimalSpecies::COUNT))
                {
                    DEBUGGING::DebugConsole::WriteLine("Checking species: ", next_species);

                    OBJECTS::AnimalType next_male_animal_type(next_species, OBJECTS::AnimalGender::MALE);
                    bool next_male_animal_type_fully_collected = current_game_data.AnimalTypeFullyCollected(next_male_animal_type);
                    if (next_male_animal_type_fully_collected)
                    {
                        DEBUGGING::DebugConsole::WriteLine("Male animal fully collected.");

                        OBJECTS::AnimalType next_female_animal_type(next_species, OBJECTS::AnimalGender::FEMALE);
                        bool next_female_animal_type_fully_collected = current_game_data.AnimalTypeFullyCollected(next_female_animal_type);
                        if (!next_female_animal_type_fully_collected)
                        {
                            // USE THE ANIMAL TYPE.
                            DEBUGGING::DebugConsole::WriteLine("Using female animal type.");
                            animal_type = next_female_animal_type;
                            break;
                        }
                        else
                        {
                            DEBUGGING::DebugConsole::WriteLine("Female animal type fully collected.");
                        }
                    }
                    else
                    {
                        // USE THE ANIMAL TYPE.
                        DEBUGGING::DebugConsole::WriteLine("Using male animal type.");
                        animal_type = next_male_animal_type;
                        break;
                    }
                }
            }
            else
            {
                // USE THE ANIMAL TYPE.
                DEBUGGING::DebugConsole::WriteLine("Using opposite gender animal type.");
                animal_type = opposite_gender_animal_type;
            }

            // CHECK IF ANOTHER ANIMAL TYPE COULD BE FOUND FOR COLLECTION.
            bool different_animal_type_found = (animal_type.Species != random_species) || (animal_type.Gender != random_gender);
            if (!different_animal_type_found)
            {
                // DON'T GENERATE A RANDOM ANIMAL.
                // Since the randomly chosen animal type has already been fully collected,
                // there's no need to generate another instance of it.
                DEBUGGING::DebugConsole::WriteLine("Different animal type not found.");
                return nullptr;
            }
        }

        // DETERMINE THE LOCATION OF THE TILE MAP AT WHICH THE ANIMAL SHOULD BE PLACED.
        MATH::FloatRectangle tile_map_world_boundaries = tile_map.GetWorldBoundingBox();
        float tile_map_left_x_position = tile_map_world_boundaries.LeftTop.X;
        float tile_map_right_x_position = tile_map_world_boundaries.RightBottom.X;
        float tile_map_top_y_position = tile_map_world_boundaries.LeftTop.Y;
        float tile_map_bottom_y_position = tile_map_world_boundaries.RightBottom.Y;
        float random_x_position = random_number_generator.RandomInRange<float>(tile_map_left_x_position, tile_map_right_x_position);
        float random_y_position = random_number_generator.RandomInRange<float>(tile_map_top_y_position, tile_map_bottom_y_position);
        std::shared_ptr<MAPS::Tile> tile_at_animal_generation_point = tile_map.GetTileAtWorldPosition(
            random_x_position, 
            random_y_position);
        if (!tile_at_animal_generation_point)
        {
            // A tile might not be retrieved if the random position is at the very edge of the tile map.
            // In this case, it's fine for now to just not generate an animal.
            std::string debug_missing_tile_message =
                "At edge: Tile map world boundaries (LRTB): " +
                std::to_string(tile_map_left_x_position) + " " +
                std::to_string(tile_map_right_x_position) + " " +
                std::to_string(tile_map_top_y_position) + " " +
                std::to_string(tile_map_bottom_y_position) + " " +
                "Random position: " +
                std::to_string(random_x_position) + " " +
                std::to_string(random_y_position);
            DEBUGGING::DebugConsole::WriteLine(debug_missing_tile_message);
            return nullptr;
        }
        DEBUGGING::DebugConsole::WriteLine("Animal at: ", MATH::Vector2f(random_x_position, random_y_position));

        // MAKE SURE THE ANIMAL ISN'T ON TOP OF NOAH.
        // This is important to ensure that animals aren't accidentally collected without
        // a player noticing.
        MATH::FloatRectangle noah_world_bounding_box = noah_player.GetWorldBoundingBox();
        bool animal_colliding_with_noah = noah_world_bounding_box.Contains(random_x_position, random_y_position);
        if (animal_colliding_with_noah)
        {
            DEBUGGING::DebugConsole::WriteLine("Animal on top of Noah.");
            return nullptr;
        }

        // MAKE SURE THE ANIMAL ISN'T BEING GENERATED ON TOP OF THE ARK.
        // This is important because it doesn't make sense for an animal to be generated on top of the ark.
        // Special collision logic also needs to occur when animals collide with the ark, so avoiding
        // generating them on potential ark tiles avoids issues for that special logic.
        bool tile_is_for_ark = MAPS::TileType::IsForArk(tile_at_animal_generation_point->Type);
        if (tile_is_for_ark)
        {
            DEBUGGING::DebugConsole::WriteLine("Tile is for ark.");
            return nullptr;
        }

        // CHECK IF THE ANIMAL CAN FLY.
        // Flying animals can be generated on any tile.
        bool animal_can_fly = animal_type.CanFly();
        if (!animal_can_fly)
        {
            // CHECK IF THE ANIMAL CAN SWIM AND THIS IS A WATER FILE.
            bool animal_can_swim = animal_type.CanSwim();
            bool is_water_tile = MAPS::TileType::IsForWater(tile_at_animal_generation_point->Type);
            bool swimming_animal_on_water_tile = (animal_can_swim && is_water_tile);
            if (!swimming_animal_on_water_tile)
            {
                // MAKE SURE THE TILE IS WALKABLE IF THE ANIMAL CAN'T FLY OR SWIM.
                bool tile_is_walkable = tile_at_animal_generation_point->IsWalkable();
                if (!tile_is_walkable)
                {
                    // Non-flying animals can't be generated on unwalkable tiles.
                    DEBUGGING::DebugConsole::WriteLine("Tile isn't walkable.");
                    return nullptr;
                }
            }
        }

        // CREATE THE ANIMAL DETERMINED BY THE ALGORITHM.
        std::shared_ptr<OBJECTS::Animal> animal = MakeAnimal(
            animal_type,
            random_x_position,
            random_y_position);
        return animal;
    }

    /// Attempts to make the specified animal.
    /// @param[in]  animal_type - The type of animal to make.
    /// @param[in]  world_x_position - The world x position of the center of the animal.
    /// @param[in]  world_y_position - The world y position of the center of the animal.
    /// @return The specified animal, if successfully created; null otherwise.
    std::shared_ptr<OBJECTS::Animal> RandomAnimalGenerationAlgorithm::MakeAnimal(
        const OBJECTS::AnimalType& animal_type,
        const float world_x_position,
        const float world_y_position)
    {
        // CREATE THE ANIMAL'S SPRITE.
        std::shared_ptr<GRAPHICS::AnimatedSprite> animal_sprite = RESOURCES::AnimalGraphics::GetSprite(animal_type);
        if (!animal_sprite)
        {
            return nullptr;
        }

        // SET THE SPRITE'S WORLD POSITION.
        animal_sprite->SetWorldPosition(world_x_position, world_y_position);

        // GET THE ANIMAL'S SOUND EFFECT.
        RESOURCES::AssetId animal_sound_id = RESOURCES::AnimalSounds::GetSound(animal_type.Species);

        // CREATE THE ANIMAL.
        std::shared_ptr<OBJECTS::Animal> animal = std::make_shared<OBJECTS::Animal>(
            animal_type, 
            GRAPHICS::AnimatedSprite(*animal_sprite),
            animal_sound_id);
        return animal;
    }
}
