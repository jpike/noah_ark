#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <SFML/System.hpp>
#include "Bible/BibleVerse.h"
#include "Containers/NestedArray.h"
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
        unsigned int GetCollectedAnimalCount(const OBJECTS::AnimalSpecies::Value species) const;
        bool AnimalSpeciesCollectedAtAll(const OBJECTS::AnimalSpecies::Value species) const;
        bool AnimalTypeFullyCollected(const OBJECTS::AnimalType& animal_type) const;

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
        /// Animal species act as outer indices, followed by gender for inner indices.
        CONTAINERS::NestedEnumArray<INVENTORY::AnimalCollectionStatistics, OBJECTS::AnimalSpecies, OBJECTS::AnimalGender>
            CollectedAnimalsBySpeciesThenGender = {};
        /// Bible verses that still need to be found by the player.
        std::vector<BIBLE::BibleVerse> BibleVersesLeftToFind = {};
        /// Whether or not certain family members have been gathered so far.
        /// Index matches enum values for family member types.
        std::array<bool, OBJECTS::FamilyMember::COUNT> FamilyMembersGathered = {};
        /// The amount of time that's elapsed during the flood, in terms of gameplay time.
        /// 150 days (Genesis 8:3) are required for the flood to subside,
        /// though gameplay time is "sped up".
        sf::Time FloodElapsedGameplayTime = sf::Time::Zero;
    };
}
