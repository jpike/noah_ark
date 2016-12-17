#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Bible/BibleVerse.h"
#include "Math/Vector2.h"

namespace STATES
{
    /// The data for the saved state of a game.
    class SavedGameData
    {
    public:
        // STATIC CONSTANTS.
        /// The default filename for a saved game file.
        static const std::string DEFAULT_FILENAME;

        // CONSTRUCTION.
        static SavedGameData DefaultSavedGameData();
        static std::unique_ptr<SavedGameData> Load(const std::string& filepath);
        
        // WRITING.
        void Write(const std::string& filepath) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The player's world position.
        MATH::Vector2f PlayerWorldPosition = MATH::Vector2f();
        /// The amount of wood found by the player.
        unsigned int WoodCount = 0;
        /// Bible verses found by the player.
        std::vector<BIBLE::BibleVerse> FoundBibleVerses = {};
    };
}
