#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Bible/BibleVerse.h"
#include "Math/Vector2.h"
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

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The path to the file for this saved game.
        std::filesystem::path Filepath = "";
        /// True if the instructions that are displayed in a text box at the start
        /// of a new game are completed; false otherwise.
        bool NewGameInstructionsCompleted = false;
        /// The player's world position.
        MATH::Vector2f PlayerWorldPosition = MATH::Vector2f();
        /// The amount of wood found by the player.
        unsigned int WoodCount = 0;
        /// Bible verses found by the player.
        std::vector<BIBLE::BibleVerse> FoundBibleVerses = {};
        /// Ark pieces built by the player.
        std::vector<BuiltArkPieceTileMapData> BuildArkPieces = {};
        /// Animals collected by the player, with types mapping to collected counts.
        std::unordered_map<OBJECTS::AnimalType, unsigned int> CollectedAnimals = {};
        /// Food collected by the player, with types mapping to collected counts.
        std::unordered_map<OBJECTS::FoodType, unsigned int> CollectedFood = {};
    };
}
