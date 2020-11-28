#pragma once

#include <algorithm>
#include <filesystem>
#include "Bible/BibleVerses.h"
#include "Resources/AnimalGraphics.h"
#include "Resources/AnimalSounds.h"
#include "States/GameSelectionScreen.h"
#include "States/SavedGameData.h"

namespace SAVED_GAME_DATA_TESTS
{
    TEST_CASE("Can properly save and restore saved game data.", "[SavedGameData]")
    {
        // CREATE THE ORIGINAL SAVED GAME DATA.
        STATES::SavedGameData original_saved_game_data;

        original_saved_game_data.CurrentGameState = STATES::GameState::PRE_FLOOD_GAMEPLAY;
        original_saved_game_data.Player->SetWorldPosition(MATH::Vector2f(1.0f, 2.0f));
        original_saved_game_data.Player->Inventory.WoodCount = 3;
        original_saved_game_data.Player->Inventory.BibleVerses =
        {
            BIBLE::BIBLE_VERSES[0],
            BIBLE::BIBLE_VERSES[50],
            BIBLE::BIBLE_VERSES[100],
        };
        original_saved_game_data.Player->Inventory.FoodCounts[OBJECTS::Food::TypeId::ALMOND] = 4;
        original_saved_game_data.Player->Inventory.FoodCounts[OBJECTS::Food::TypeId::BANANA] = 5;
        original_saved_game_data.Player->Inventory.FoodCounts[OBJECTS::Food::TypeId::WALNUT] = 6;
        original_saved_game_data.Player->Inventory.FollowingAnimals.Noah = &*original_saved_game_data.Player;
        original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition = MATH::Vector2f(7.0f, 8.0f);

        OBJECTS::AnimalType first_following_animal_type(OBJECTS::AnimalSpecies::ALLIGATOR, OBJECTS::AnimalGender::MALE);
        auto first_following_animal = MEMORY::NonNullSharedPointer<OBJECTS::Animal>(std::make_shared<OBJECTS::Animal>(
            first_following_animal_type,
            *RESOURCES::AnimalGraphics::GetSprite(first_following_animal_type),
            RESOURCES::AnimalSounds::GetSound(first_following_animal_type.Species)));
        original_saved_game_data.Player->Inventory.FollowingAnimals.Add(first_following_animal);

        OBJECTS::AnimalType second_following_animal_type(OBJECTS::AnimalSpecies::WOLF, OBJECTS::AnimalGender::FEMALE);
        auto second_following_animal = MEMORY::NonNullSharedPointer<OBJECTS::Animal>(std::make_shared<OBJECTS::Animal>(
            second_following_animal_type,
            *RESOURCES::AnimalGraphics::GetSprite(second_following_animal_type),
            RESOURCES::AnimalSounds::GetSound(second_following_animal_type.Species)));
        original_saved_game_data.Player->Inventory.FollowingAnimals.Add(second_following_animal);

        original_saved_game_data.CollectedAnimalsBySpeciesThenGender[OBJECTS::AnimalSpecies::ALLIGATOR][OBJECTS::AnimalGender::MALE] = INVENTORY::AnimalCollectionStatistics
        {
            .FollowingPlayerCount = 1,
            .InArkCount = 2,
            .AnimalPenTileMapLayerIndex = 0,
            .AnimalPenTileMapColumnIndex = 0,
            .AnimalPenTileMapRowIndex = 0,
            .AnimalPenTileMapPenIndex = 0,
        };
        original_saved_game_data.CollectedAnimalsBySpeciesThenGender[OBJECTS::AnimalSpecies::ALLIGATOR][OBJECTS::AnimalGender::FEMALE] = INVENTORY::AnimalCollectionStatistics
        {
            .FollowingPlayerCount = 0,
            .InArkCount = 3,
            .AnimalPenTileMapLayerIndex = 0,
            .AnimalPenTileMapColumnIndex = 0,
            .AnimalPenTileMapRowIndex = 0,
            .AnimalPenTileMapPenIndex = 0,
        };
        original_saved_game_data.CollectedAnimalsBySpeciesThenGender[OBJECTS::AnimalSpecies::WOLF][OBJECTS::AnimalGender::MALE] = INVENTORY::AnimalCollectionStatistics
        {
            .FollowingPlayerCount = 4,
            .InArkCount = 5,
            .AnimalPenTileMapLayerIndex = 0,
            .AnimalPenTileMapColumnIndex = 0,
            .AnimalPenTileMapRowIndex = 0,
            .AnimalPenTileMapPenIndex = 1,
        };
        original_saved_game_data.CollectedAnimalsBySpeciesThenGender[OBJECTS::AnimalSpecies::WOLF][OBJECTS::AnimalGender::FEMALE] = INVENTORY::AnimalCollectionStatistics
        {
            .FollowingPlayerCount = 6,
            .InArkCount = 7,
            .AnimalPenTileMapLayerIndex = 0,
            .AnimalPenTileMapColumnIndex = 0,
            .AnimalPenTileMapRowIndex = 0,
            .AnimalPenTileMapPenIndex = 1,
        };

        std::set_difference(
            BIBLE::BIBLE_VERSES.cbegin(),
            BIBLE::BIBLE_VERSES.cend(),
            original_saved_game_data.Player->Inventory.BibleVerses.cbegin(),
            original_saved_game_data.Player->Inventory.BibleVerses.cend(),
            std::inserter(original_saved_game_data.BibleVersesLeftToFind, original_saved_game_data.BibleVersesLeftToFind.begin()));

        original_saved_game_data.BuiltArkPieces =
        {
            OBJECTS::ArkPiece(23),
            OBJECTS::ArkPiece(24),
            OBJECTS::ArkPiece(25),
        };
        original_saved_game_data.FamilyMembersGathered[OBJECTS::FamilyMember::NOAH_WIFE] = true;
        original_saved_game_data.FamilyMembersGathered[OBJECTS::FamilyMember::HAM] = true;
        original_saved_game_data.FamilyMembersGathered[OBJECTS::FamilyMember::SHEM_WIFE] = false;
        
        original_saved_game_data.FloodDayCount = 23;

        // WRITE AND READ THE SAVED GAME DATA TO FILE.
        // The file is written to the same default "saved games" folder to better support re-using this data for actual playthoughs.
        std::filesystem::create_directory(STATES::GameSelectionScreen::SAVED_GAMES_FOLDER_PATH);
        const std::filesystem::path SAVED_GAME_DATA_FILENAME = "TestSavedGameData.dat";
        const std::filesystem::path SAVED_GAME_DATA_FILEPATH = STATES::GameSelectionScreen::SAVED_GAMES_FOLDER_PATH / SAVED_GAME_DATA_FILENAME;
        original_saved_game_data.Write(SAVED_GAME_DATA_FILEPATH);
        std::unique_ptr<STATES::SavedGameData> reread_saved_game_data = STATES::SavedGameData::Load(SAVED_GAME_DATA_FILEPATH);

        // VERIFY THAT THE CORRECT SAVED DATA WAS LOADED.
        REQUIRE(original_saved_game_data.CurrentGameState == reread_saved_game_data->CurrentGameState);
        REQUIRE(original_saved_game_data.Player->GetWorldPosition().X == reread_saved_game_data->Player->GetWorldPosition().X);
        REQUIRE(original_saved_game_data.Player->GetWorldPosition().Y == reread_saved_game_data->Player->GetWorldPosition().Y);
        REQUIRE(original_saved_game_data.Player->Inventory.WoodCount == reread_saved_game_data->Player->Inventory.WoodCount);
        REQUIRE(original_saved_game_data.Player->Inventory.BibleVerses == reread_saved_game_data->Player->Inventory.BibleVerses);
        REQUIRE(original_saved_game_data.Player->Inventory.FoodCounts == reread_saved_game_data->Player->Inventory.FoodCounts);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.Noah == &*original_saved_game_data.Player);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X == reread_saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y == reread_saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y);

        // The animal types are the most important data in the following animal groups and the only data that can be easily verified.
        std::vector<OBJECTS::AnimalType> actual_following_animal_types(reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.size());
        std::transform(
            reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.cbegin(),
            reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.cend(),
            actual_following_animal_types.begin(),
            [](const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal) -> OBJECTS::AnimalType
            {
                return animal->Type;
            });
        std::vector<OBJECTS::AnimalType> expected_following_animal_types(original_saved_game_data.Player->Inventory.FollowingAnimals.Animals.size());
        std::transform(
            original_saved_game_data.Player->Inventory.FollowingAnimals.Animals.cbegin(),
            original_saved_game_data.Player->Inventory.FollowingAnimals.Animals.cend(),
            expected_following_animal_types.begin(),
            [](const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal) -> OBJECTS::AnimalType
            {
                return animal->Type;
            });
        REQUIRE(expected_following_animal_types == actual_following_animal_types);

        REQUIRE(original_saved_game_data.CollectedAnimalsBySpeciesThenGender == reread_saved_game_data->CollectedAnimalsBySpeciesThenGender);
        REQUIRE(original_saved_game_data.BibleVersesLeftToFind == reread_saved_game_data->BibleVersesLeftToFind);

        std::size_t max_built_ark_piece_count = std::max(original_saved_game_data.BuiltArkPieces.size(), reread_saved_game_data->BuiltArkPieces.size());
        for (std::size_t ark_piece_index = 0; ark_piece_index < max_built_ark_piece_count; ++ark_piece_index)
        {
            INFO("Checking ark piece: " + std::to_string(ark_piece_index));

            const OBJECTS::ArkPiece& actual_ark_piece = reread_saved_game_data->BuiltArkPieces[ark_piece_index];
            const OBJECTS::ArkPiece& expected_ark_piece = original_saved_game_data.BuiltArkPieces[ark_piece_index];
            REQUIRE(expected_ark_piece.Id == actual_ark_piece.Id);
            REQUIRE(expected_ark_piece.Sprite.WorldPosition.X == actual_ark_piece.Sprite.WorldPosition.X);
            REQUIRE(expected_ark_piece.Sprite.WorldPosition.Y == actual_ark_piece.Sprite.WorldPosition.Y);
            REQUIRE(expected_ark_piece.Built == actual_ark_piece.Built);
            REQUIRE(expected_ark_piece.IsExternalDoorway == actual_ark_piece.IsExternalDoorway);
        }
        
        REQUIRE(original_saved_game_data.FamilyMembersGathered == reread_saved_game_data->FamilyMembersGathered);
        REQUIRE(original_saved_game_data.FloodDayCount == reread_saved_game_data->FloodDayCount);
    }

    TEST_CASE("Can properly save and restore nearly full saved game data pre-flood.", "[SavedGameData]")
    {
        // CREATE THE ORIGINAL SAVED GAME DATA.
        // Nearly "full" saved game data (prior to the flood) is created here to enable easier testing of completion.
        STATES::SavedGameData original_saved_game_data;

        original_saved_game_data.CurrentGameState = STATES::GameState::PRE_FLOOD_GAMEPLAY;
        original_saved_game_data.Player->SetWorldPosition(MATH::Vector2f(300.0f, 100.0f));
        original_saved_game_data.Player->Inventory.WoodCount = 9999;
        original_saved_game_data.Player->Inventory.BibleVerses.insert(
            BIBLE::BIBLE_VERSES.cbegin(),
            BIBLE::BIBLE_VERSES.cend());
        for (unsigned int food_id = OBJECTS::Food::TypeId::NONE; food_id < OBJECTS::Food::TypeId::COUNT; ++food_id)
        {
            original_saved_game_data.Player->Inventory.FoodCounts[food_id] = 99;
        }
        original_saved_game_data.Player->Inventory.FollowingAnimals.Noah = &*original_saved_game_data.Player;
        original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition = MATH::Vector2f(300.0f, 64.0f);

        unsigned int animal_pen_tile_map_layer_index = 0;
        unsigned int animal_pen_tile_map_column_index = 0;
        unsigned int animal_pen_tile_map_row_index = 0;
        unsigned int animal_pen_tile_map_pen_index = 0;
        for (unsigned int animal_species_id = 0; animal_species_id < OBJECTS::AnimalSpecies::COUNT; ++animal_species_id)
        {
            for (unsigned int animal_gender_id = 0; animal_gender_id < OBJECTS::AnimalGender::COUNT; ++animal_gender_id)
            {
                OBJECTS::AnimalSpecies::Value animal_species = static_cast<OBJECTS::AnimalSpecies::Value>(animal_species_id);
                OBJECTS::AnimalGender::Value animal_gender = static_cast<OBJECTS::AnimalGender::Value>(animal_gender_id);
                OBJECTS::AnimalType animal_type(animal_species, animal_gender);
                auto animal = MEMORY::NonNullSharedPointer<OBJECTS::Animal>(std::make_shared<OBJECTS::Animal>(
                    animal_type,
                    *RESOURCES::AnimalGraphics::GetSprite(animal_type),
                    RESOURCES::AnimalSounds::GetSound(animal_type.Species)));
                original_saved_game_data.Player->Inventory.FollowingAnimals.Add(animal);

                // For simplicity, all but one of the animals is considered as being in the ark.
                unsigned int in_ark_count = 1;
                if (animal_type.Clean())
                {
                    in_ark_count = 7 - 1;
                }
                original_saved_game_data.CollectedAnimalsBySpeciesThenGender[animal_species][animal_gender] = INVENTORY::AnimalCollectionStatistics
                {
                    .FollowingPlayerCount = 1,
                    .InArkCount = in_ark_count,
                    .AnimalPenTileMapLayerIndex = animal_pen_tile_map_layer_index,
                    .AnimalPenTileMapColumnIndex = animal_pen_tile_map_column_index,
                    .AnimalPenTileMapRowIndex = animal_pen_tile_map_row_index,
                    .AnimalPenTileMapPenIndex = animal_pen_tile_map_pen_index,
                };
            }

            // UPDATE INDICES FOR ANIMAL PENS.
            constexpr unsigned int MAX_ANIMAL_PENS_PER_ROW_PER_TILE_MAP = 4;
            constexpr unsigned int MAX_ANIMAL_PENS_PER_COLUMN_PER_TILE_MAP = 2;
            constexpr unsigned int MAX_ANIMAL_PENS_PER_TILE_MAP = MAX_ANIMAL_PENS_PER_ROW_PER_TILE_MAP * MAX_ANIMAL_PENS_PER_COLUMN_PER_TILE_MAP;
            animal_pen_tile_map_pen_index = (animal_pen_tile_map_pen_index + 1) % MAX_ANIMAL_PENS_PER_TILE_MAP;

            constexpr unsigned int MAX_ARK_LAYERS = 3;
            constexpr unsigned int MAX_TILE_MAPS_PER_ROW_PER_LAYER = 3;
            constexpr unsigned int MAX_ANIMAL_PENS_PER_LAYER = MAX_ANIMAL_PENS_PER_TILE_MAP * MAX_TILE_MAPS_PER_ROW_PER_LAYER;
            unsigned int next_animal_species_id = animal_species_id + 1;
            animal_pen_tile_map_layer_index = (next_animal_species_id / MAX_ANIMAL_PENS_PER_LAYER) % MAX_ARK_LAYERS;

            constexpr unsigned int MAX_TILE_MAPS_PER_COLUMN_PER_LAYER = 1;
            animal_pen_tile_map_row_index = next_animal_species_id % MAX_TILE_MAPS_PER_COLUMN_PER_LAYER;

            animal_pen_tile_map_column_index = (next_animal_species_id / MAX_ANIMAL_PENS_PER_TILE_MAP) % MAX_TILE_MAPS_PER_ROW_PER_LAYER;
        }

        std::set_difference(
            BIBLE::BIBLE_VERSES.cbegin(),
            BIBLE::BIBLE_VERSES.cend(),
            original_saved_game_data.Player->Inventory.BibleVerses.cbegin(),
            original_saved_game_data.Player->Inventory.BibleVerses.cend(),
            std::inserter(original_saved_game_data.BibleVersesLeftToFind, original_saved_game_data.BibleVersesLeftToFind.begin()));

        for (unsigned int ark_piece_id = 1; ark_piece_id <= 31; ++ark_piece_id)
        {
            OBJECTS::ArkPiece ark_piece(ark_piece_id);
            ark_piece.Built = true;
            original_saved_game_data.BuiltArkPieces.emplace_back(ark_piece);
        }

        for (unsigned int family_member_id = 0; family_member_id < OBJECTS::FamilyMember::COUNT; ++family_member_id)
        {
            original_saved_game_data.FamilyMembersGathered[family_member_id] = true;
        }
        
        original_saved_game_data.FloodDayCount = 150;

        // WRITE AND READ THE SAVED GAME DATA TO FILE.
        // The file is written to the same default "saved games" folder to better support re-using this data for actual playthoughs.
        std::filesystem::create_directory(STATES::GameSelectionScreen::SAVED_GAMES_FOLDER_PATH);
        const std::filesystem::path SAVED_GAME_DATA_FILENAME = "TestNearlyFullSavedGameDataPreFlood.dat";
        const std::filesystem::path SAVED_GAME_DATA_FILEPATH = STATES::GameSelectionScreen::SAVED_GAMES_FOLDER_PATH / SAVED_GAME_DATA_FILENAME;
        original_saved_game_data.Write(SAVED_GAME_DATA_FILEPATH);
        std::unique_ptr<STATES::SavedGameData> reread_saved_game_data = STATES::SavedGameData::Load(SAVED_GAME_DATA_FILEPATH);

        // VERIFY THAT THE CORRECT SAVED DATA WAS LOADED.
        REQUIRE(original_saved_game_data.CurrentGameState == reread_saved_game_data->CurrentGameState);
        REQUIRE(original_saved_game_data.Player->GetWorldPosition().X == reread_saved_game_data->Player->GetWorldPosition().X);
        REQUIRE(original_saved_game_data.Player->GetWorldPosition().Y == reread_saved_game_data->Player->GetWorldPosition().Y);
        REQUIRE(original_saved_game_data.Player->Inventory.WoodCount == reread_saved_game_data->Player->Inventory.WoodCount);
        REQUIRE(original_saved_game_data.Player->Inventory.BibleVerses == reread_saved_game_data->Player->Inventory.BibleVerses);
        REQUIRE(original_saved_game_data.Player->Inventory.FoodCounts == reread_saved_game_data->Player->Inventory.FoodCounts);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.Noah == &*original_saved_game_data.Player);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X == reread_saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y == reread_saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y);

        // The animal types are the most important data in the following animal groups and the only data that can be easily verified.
        std::vector<OBJECTS::AnimalType> actual_following_animal_types(reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.size());
        std::transform(
            reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.cbegin(),
            reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.cend(),
            actual_following_animal_types.begin(),
            [](const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal) -> OBJECTS::AnimalType
            {
                return animal->Type;
            });
        std::vector<OBJECTS::AnimalType> expected_following_animal_types(original_saved_game_data.Player->Inventory.FollowingAnimals.Animals.size());
        std::transform(
            original_saved_game_data.Player->Inventory.FollowingAnimals.Animals.cbegin(),
            original_saved_game_data.Player->Inventory.FollowingAnimals.Animals.cend(),
            expected_following_animal_types.begin(),
            [](const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal) -> OBJECTS::AnimalType
            {
                return animal->Type;
            });
        REQUIRE(expected_following_animal_types == actual_following_animal_types);

        REQUIRE(original_saved_game_data.CollectedAnimalsBySpeciesThenGender == reread_saved_game_data->CollectedAnimalsBySpeciesThenGender);
        REQUIRE(original_saved_game_data.BibleVersesLeftToFind == reread_saved_game_data->BibleVersesLeftToFind);

        std::size_t max_built_ark_piece_count = std::max(original_saved_game_data.BuiltArkPieces.size(), reread_saved_game_data->BuiltArkPieces.size());
        for (std::size_t ark_piece_index = 0; ark_piece_index < max_built_ark_piece_count; ++ark_piece_index)
        {
            INFO("Checking ark piece: " + std::to_string(ark_piece_index));

            const OBJECTS::ArkPiece& actual_ark_piece = reread_saved_game_data->BuiltArkPieces[ark_piece_index];
            const OBJECTS::ArkPiece& expected_ark_piece = original_saved_game_data.BuiltArkPieces[ark_piece_index];
            REQUIRE(expected_ark_piece.Id == actual_ark_piece.Id);
            REQUIRE(expected_ark_piece.Sprite.WorldPosition.X == actual_ark_piece.Sprite.WorldPosition.X);
            REQUIRE(expected_ark_piece.Sprite.WorldPosition.Y == actual_ark_piece.Sprite.WorldPosition.Y);
            REQUIRE(expected_ark_piece.Built == actual_ark_piece.Built);
            REQUIRE(expected_ark_piece.IsExternalDoorway == actual_ark_piece.IsExternalDoorway);
        }
        
        REQUIRE(original_saved_game_data.FamilyMembersGathered == reread_saved_game_data->FamilyMembersGathered);
        REQUIRE(original_saved_game_data.FloodDayCount == reread_saved_game_data->FloodDayCount);
    }

    TEST_CASE("Can properly save and restore nearly full saved game data during flood.", "[SavedGameData]")
    {
        // CREATE THE ORIGINAL SAVED GAME DATA.
        // Nearly "full" saved game data (during the flood) is created here to enable easier testing of completion.
        STATES::SavedGameData original_saved_game_data;

        original_saved_game_data.CurrentGameState = STATES::GameState::DURING_FLOOD_GAMEPLAY;
        original_saved_game_data.Player->SetWorldPosition(MATH::Vector2f(300.0f, 100.0f));
        original_saved_game_data.Player->Inventory.BibleVerses.insert(
            BIBLE::BIBLE_VERSES.cbegin(),
            BIBLE::BIBLE_VERSES.cend());
        for (unsigned int food_id = OBJECTS::Food::TypeId::NONE; food_id < OBJECTS::Food::TypeId::COUNT; ++food_id)
        {
            original_saved_game_data.Player->Inventory.FoodCounts[food_id] = 99;
        }

        unsigned int animal_pen_tile_map_layer_index = 0;
        unsigned int animal_pen_tile_map_column_index = 0;
        unsigned int animal_pen_tile_map_row_index = 0;
        unsigned int animal_pen_tile_map_pen_index = 0;
        for (unsigned int animal_species_id = 0; animal_species_id < OBJECTS::AnimalSpecies::COUNT; ++animal_species_id, ++animal_pen_tile_map_pen_index)
        {
            for (unsigned int animal_gender_id = 0; animal_gender_id < OBJECTS::AnimalGender::COUNT; ++animal_gender_id)
            {
                OBJECTS::AnimalSpecies::Value animal_species = static_cast<OBJECTS::AnimalSpecies::Value>(animal_species_id);
                OBJECTS::AnimalGender::Value animal_gender = static_cast<OBJECTS::AnimalGender::Value>(animal_gender_id);
                OBJECTS::AnimalType animal_type(animal_species, animal_gender);

                // For simplicity, all but one of the animals is considered as being in the ark.
                unsigned int in_ark_count = 2;
                if (animal_type.Clean())
                {
                    in_ark_count = 7;
                }
                original_saved_game_data.CollectedAnimalsBySpeciesThenGender[animal_species][animal_gender] = INVENTORY::AnimalCollectionStatistics
                {
                    .FollowingPlayerCount = 0,
                    .InArkCount = in_ark_count,
                    .AnimalPenTileMapLayerIndex = animal_pen_tile_map_layer_index,
                    .AnimalPenTileMapColumnIndex = animal_pen_tile_map_column_index,
                    .AnimalPenTileMapRowIndex = animal_pen_tile_map_row_index,
                    .AnimalPenTileMapPenIndex = animal_pen_tile_map_pen_index,
                };

                // UPDATE INDICES FOR ANIMAL PENS.
                constexpr unsigned int MAX_ANIMAL_PENS_PER_ROW_PER_TILE_MAP = 4;
                constexpr unsigned int MAX_ANIMAL_PENS_PER_COLUMN_PER_TILE_MAP = 2;
                constexpr unsigned int MAX_ANIMAL_PENS_PER_TILE_MAP = MAX_ANIMAL_PENS_PER_ROW_PER_TILE_MAP * MAX_ANIMAL_PENS_PER_COLUMN_PER_TILE_MAP;
                animal_pen_tile_map_pen_index = (animal_pen_tile_map_pen_index + 1) % MAX_ANIMAL_PENS_PER_TILE_MAP;

                constexpr unsigned int MAX_ARK_LAYERS = 3;
                constexpr unsigned int MAX_TILE_MAPS_PER_ROW_PER_LAYER = 3;
                constexpr unsigned int MAX_ANIMAL_PENS_PER_LAYER = MAX_ANIMAL_PENS_PER_TILE_MAP * MAX_TILE_MAPS_PER_ROW_PER_LAYER;
                unsigned int next_animal_species_id = animal_species_id + 1;
                animal_pen_tile_map_layer_index = (next_animal_species_id / MAX_ANIMAL_PENS_PER_LAYER) % MAX_ARK_LAYERS;

                constexpr unsigned int MAX_TILE_MAPS_PER_COLUMN_PER_LAYER = 1;
                animal_pen_tile_map_row_index = next_animal_species_id % MAX_TILE_MAPS_PER_COLUMN_PER_LAYER;

                animal_pen_tile_map_column_index = (next_animal_species_id / MAX_ANIMAL_PENS_PER_TILE_MAP) % MAX_TILE_MAPS_PER_ROW_PER_LAYER;
            }
        }

        std::set_difference(
            BIBLE::BIBLE_VERSES.cbegin(),
            BIBLE::BIBLE_VERSES.cend(),
            original_saved_game_data.Player->Inventory.BibleVerses.cbegin(),
            original_saved_game_data.Player->Inventory.BibleVerses.cend(),
            std::inserter(original_saved_game_data.BibleVersesLeftToFind, original_saved_game_data.BibleVersesLeftToFind.begin()));

        for (unsigned int ark_piece_id = 1; ark_piece_id <= 31; ++ark_piece_id)
        {
            OBJECTS::ArkPiece ark_piece(ark_piece_id);
            ark_piece.Built = true;
            original_saved_game_data.BuiltArkPieces.emplace_back(ark_piece);
        }

        for (unsigned int family_member_id = 0; family_member_id < OBJECTS::FamilyMember::COUNT; ++family_member_id)
        {
            original_saved_game_data.FamilyMembersGathered[family_member_id] = true;
        }
        
        // Set to 1 less than the maximum to easily be close to the end.
        original_saved_game_data.FloodDayCount = 149;

        // WRITE AND READ THE SAVED GAME DATA TO FILE.
        // The file is written to the same default "saved games" folder to better support re-using this data for actual playthoughs.
        std::filesystem::create_directory(STATES::GameSelectionScreen::SAVED_GAMES_FOLDER_PATH);
        const std::filesystem::path SAVED_GAME_DATA_FILENAME = "TestNearlyFullSavedGameDataDuringFlood.dat";
        const std::filesystem::path SAVED_GAME_DATA_FILEPATH = STATES::GameSelectionScreen::SAVED_GAMES_FOLDER_PATH / SAVED_GAME_DATA_FILENAME;
        original_saved_game_data.Write(SAVED_GAME_DATA_FILEPATH);
        std::unique_ptr<STATES::SavedGameData> reread_saved_game_data = STATES::SavedGameData::Load(SAVED_GAME_DATA_FILEPATH);

        // VERIFY THAT THE CORRECT SAVED DATA WAS LOADED.
        REQUIRE(original_saved_game_data.CurrentGameState == reread_saved_game_data->CurrentGameState);
        REQUIRE(original_saved_game_data.Player->GetWorldPosition().X == reread_saved_game_data->Player->GetWorldPosition().X);
        REQUIRE(original_saved_game_data.Player->GetWorldPosition().Y == reread_saved_game_data->Player->GetWorldPosition().Y);
        REQUIRE(original_saved_game_data.Player->Inventory.WoodCount == reread_saved_game_data->Player->Inventory.WoodCount);
        REQUIRE(original_saved_game_data.Player->Inventory.BibleVerses == reread_saved_game_data->Player->Inventory.BibleVerses);
        REQUIRE(original_saved_game_data.Player->Inventory.FoodCounts == reread_saved_game_data->Player->Inventory.FoodCounts);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.Noah == &*original_saved_game_data.Player);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X == reread_saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y == reread_saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y);

        // The animal types are the most important data in the following animal groups and the only data that can be easily verified.
        std::vector<OBJECTS::AnimalType> actual_following_animal_types(reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.size());
        std::transform(
            reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.cbegin(),
            reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals.cend(),
            actual_following_animal_types.begin(),
            [](const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal) -> OBJECTS::AnimalType
            {
                return animal->Type;
            });
        std::vector<OBJECTS::AnimalType> expected_following_animal_types;
        REQUIRE(expected_following_animal_types == actual_following_animal_types);

        REQUIRE(original_saved_game_data.CollectedAnimalsBySpeciesThenGender == reread_saved_game_data->CollectedAnimalsBySpeciesThenGender);
        REQUIRE(original_saved_game_data.BibleVersesLeftToFind == reread_saved_game_data->BibleVersesLeftToFind);

        std::size_t max_built_ark_piece_count = std::max(original_saved_game_data.BuiltArkPieces.size(), reread_saved_game_data->BuiltArkPieces.size());
        for (std::size_t ark_piece_index = 0; ark_piece_index < max_built_ark_piece_count; ++ark_piece_index)
        {
            INFO("Checking ark piece: " + std::to_string(ark_piece_index));

            const OBJECTS::ArkPiece& actual_ark_piece = reread_saved_game_data->BuiltArkPieces[ark_piece_index];
            const OBJECTS::ArkPiece& expected_ark_piece = original_saved_game_data.BuiltArkPieces[ark_piece_index];
            REQUIRE(expected_ark_piece.Id == actual_ark_piece.Id);
            REQUIRE(expected_ark_piece.Sprite.WorldPosition.X == actual_ark_piece.Sprite.WorldPosition.X);
            REQUIRE(expected_ark_piece.Sprite.WorldPosition.Y == actual_ark_piece.Sprite.WorldPosition.Y);
            REQUIRE(expected_ark_piece.Built == actual_ark_piece.Built);
            REQUIRE(expected_ark_piece.IsExternalDoorway == actual_ark_piece.IsExternalDoorway);
        }
        
        REQUIRE(original_saved_game_data.FamilyMembersGathered == reread_saved_game_data->FamilyMembersGathered);
        REQUIRE(original_saved_game_data.FloodDayCount == reread_saved_game_data->FloodDayCount);
    }
}
