#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Bible/BibleVerse.h"
#include "Inventory/AnimalCollectionStatistics.h"
#include "Inventory/Inventory.h"
#include "Math/Vector2.h"
#include "Memory/Pointers.h"
#include "Objects/Animal.h"
#include "Objects/ArkPiece.h"
#include "Objects/FamilyMember.h"
#include "Objects/Noah.h"
#include "States/GameState.h"

namespace STATES
{
    /// The data for the saved state of a game.
    class SavedGameData
    {
    public:
        // STATIC CONSTANTS.
        /// The default filepath for a saved game file.
        static const std::string DEFAULT_FILEPATH;

        // CONSTRUCTION.
        static SavedGameData DefaultSavedGameData();
        static std::unique_ptr<SavedGameData> Load(const std::filesystem::path& filepath);

        // WRITING.
        void Write(const std::filesystem::path& filepath) const;

        // OTHER PUBLIC METHODS.
        bool AnimalTypeFullyCollected(const OBJECTS::AnimalType& animal_type) const;
        INVENTORY::AnimalCollectionStatistics GetAnimalCollectionStatistics(const OBJECTS::AnimalSpecies::Value species) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The path to the file for this saved game.
        std::filesystem::path Filepath = "";
        /// The state the game is currently in.
        GameState CurrentGameState = GameState::INTRO_SEQUENCE;
        /// The player.  Stored as a shared pointer since the player needs to be shared with the world.
        MEMORY::NonNullSharedPointer<OBJECTS::Noah> Player = MEMORY::NonNullSharedPointer<OBJECTS::Noah>(std::make_shared<OBJECTS::Noah>());
        /// Ark pieces built by the player.
        std::vector<OBJECTS::ArkPiece> BuiltArkPieces = {};
        /// Statistics for animals collected by the player.
        /// Animal species act as indices.
        std::array<INVENTORY::AnimalCollectionStatistics, OBJECTS::AnimalSpecies::COUNT> CollectedAnimals = {};
        /// Bible verses that still need to be found by the player.
        std::vector<BIBLE::BibleVerse> BibleVersesLeftToFind = {};
        /// Whether or not certain family members have been gathered so far.
        /// Index matches enum values for family member types.
        std::array<bool, OBJECTS::FamilyMember::Type::COUNT> FamilyMembersGathered = {};
        /// The number of days of the flood elapsed so far.
        /// 150 days (Genesis 8:3) are required for the flood to subside.
        unsigned int FloodDayCount = 0;
    };
}
