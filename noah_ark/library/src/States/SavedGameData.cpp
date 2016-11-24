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
        default_saved_game_data.PlayerWorldPosition.X = 256.0f;
        default_saved_game_data.PlayerWorldPosition.Y = 192.0f;
        return default_saved_game_data;
    }
}
