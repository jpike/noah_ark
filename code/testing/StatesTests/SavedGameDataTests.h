#pragma once

#include "Bible/BibleVerses.h"
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
        original_saved_game_data.Player->Inventory.FollowingAnimals.Animals =
        {

        };

        original_saved_game_data.CollectedAnimals =
        {

        };
        original_saved_game_data.BibleVersesLeftToFind =
        {

        };
        original_saved_game_data.BuiltArkPieces =
        {

        };
        original_saved_game_data.FamilyMembersGathered =
        {

        };
        original_saved_game_data.FloodDayCount =
        {

        };

        // WRITE AND READ THE SAVED GAME DATA TO FILE.
        const std::filesystem::path SAVED_GAME_DATA_FILEPATH = "TestSavedGameData.dat";
        std::unique_ptr<STATES::SavedGameData> reread_saved_game_data = STATES::SavedGameData::Load(SAVED_GAME_DATA_FILEPATH);

        // VERIFY THAT THE CORRECT SAVED DATA WAS LOADED.
        REQUIRE(original_saved_game_data.CurrentGameState == reread_saved_game_data->CurrentGameState);
        REQUIRE(original_saved_game_data.Player->GetWorldPosition().X == reread_saved_game_data->Player->GetWorldPosition().X);
        REQUIRE(original_saved_game_data.Player->GetWorldPosition().Y == reread_saved_game_data->Player->GetWorldPosition().Y);
        REQUIRE(original_saved_game_data.Player->Inventory.WoodCount == reread_saved_game_data->Player->Inventory.WoodCount);
        REQUIRE(original_saved_game_data.Player->Inventory.FoodCounts == reread_saved_game_data->Player->Inventory.FoodCounts);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.Noah == &*original_saved_game_data.Player);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X == reread_saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.X);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y == reread_saved_game_data->Player->Inventory.FollowingAnimals.CurrentCenterWorldPosition.Y);
        REQUIRE(original_saved_game_data.Player->Inventory.FollowingAnimals.Animals == reread_saved_game_data->Player->Inventory.FollowingAnimals.Animals);
        //REQUIRE(original_saved_game_data.CollectedAnimals == reread_saved_game_data->CollectedAnimals);
        //REQUIRE(original_saved_game_data.BibleVersesLeftToFind == reread_saved_game_data->BibleVersesLeftToFind);
        //REQUIRE(original_saved_game_data.BuiltArkPieces == reread_saved_game_data->BuiltArkPieces);
        REQUIRE(original_saved_game_data.FamilyMembersGathered == reread_saved_game_data->FamilyMembersGathered);
        REQUIRE(original_saved_game_data.FloodDayCount == reread_saved_game_data->FloodDayCount);
    }
}
