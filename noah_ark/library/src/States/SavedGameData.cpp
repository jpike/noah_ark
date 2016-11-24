#include "States/IntroSequence.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// Creates an instance of the default saved game data
    /// for a newly started game.
    /// @return The default saved game data.
    SavedGameData SavedGameData::DefaultSavedGameData()
    {
        // The exact values are set based on experimentation
        // for what seems best for a new game.
        SavedGameData default_saved_game_data;

        // INITIALIZE THE DEFAULT PLAYER POSITION.
        default_saved_game_data.PlayerWorldPosition.X = 256.0f;
        default_saved_game_data.PlayerWorldPosition.Y = 192.0f;

        // INITIALIZE THE DEFAULT FOUND BIBLE VERSES.
        // Verses from the intro sequence are found by default.
        default_saved_game_data.FoundBibleVerses = IntroSequence::IntroBibleVerses();

        return default_saved_game_data;
    }
}
