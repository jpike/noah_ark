#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Bible/BibleVerse.h"
#include "Inventory/Inventory.h"
#include "Math/Vector2.h"
#include "Memory/Pointers.h"
#include "Objects/Noah.h"
#include "States/BuiltArkPieceTileMapData.h"

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

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The path to the file for this saved game.
        std::filesystem::path Filepath = "";
        /// True if the instructions that are displayed in a text box at the start
        /// of a new game are completed; false otherwise.
        bool NewGameInstructionsCompleted = false;
        /// The player.  Stored as a shared pointer since the player needs to be shared with the world.
        MEMORY::NonNullSharedPointer<OBJECTS::Noah> Player = MEMORY::NonNullSharedPointer<OBJECTS::Noah>(std::make_shared<OBJECTS::Noah>());
        /// Ark pieces built by the player.
        std::vector<BuiltArkPieceTileMapData> BuildArkPieces = {};
        /// Animals collected by the player, with types mapping to collected counts.
        std::unordered_map<OBJECTS::AnimalType, unsigned int> CollectedAnimals = {};
    };
}
