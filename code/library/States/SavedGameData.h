#pragma once

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
#include "Objects/Noah.h"
#include "States/BuiltArkPieceTileMapData.h"
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
        static std::unique_ptr<SavedGameData> Load(const std::string& filepath);

        // WRITING.
        void Write(const std::filesystem::path& filepath) const;

        // OTHER PUBLIC METHODS.
        bool AnimalTypeFullyCollected(const OBJECTS::AnimalType& animal_type) const;
        INVENTORY::AnimalCollectionStatistics GetAnimalCollectionStatistics(const OBJECTS::AnimalSpecies& species) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The path to the file for this saved game.
        std::filesystem::path Filepath = "";
        /// The state the game is currently in.
        GameState CurrentGameState = GameState::INTRO_SEQUENCE;
        /// True if the instructions that are displayed in a text box at the start
        /// of a new game are completed; false otherwise.
        bool NewGameInstructionsCompleted = false;
        /// The player.  Stored as a shared pointer since the player needs to be shared with the world.
        MEMORY::NonNullSharedPointer<OBJECTS::Noah> Player = MEMORY::NonNullSharedPointer<OBJECTS::Noah>(std::make_shared<OBJECTS::Noah>());
        /// Ark pieces built by the player.
        std::vector<BuiltArkPieceTileMapData> BuiltArkPieces = {};
        /// All animals collected by the player, with types mapping to collected counts.
        std::unordered_map<OBJECTS::AnimalType, unsigned int> AllCollectedAnimals = {};
        /// Animals collected by the player that are currently following the player,
        /// with types mapping to collected counts.
        std::unordered_map<OBJECTS::AnimalType, unsigned int> FollowingAnimals = {};
        /// Animals collected by the player that are currently in the ark,
        /// with types mapping to collected counts.
        std::unordered_map<OBJECTS::AnimalType, unsigned int> AnimalsInArk = {};
    };
}
