#pragma once

#include "Math/Vector2.h"

namespace STATES
{
    /// The data for the saved state of a game.
    class SavedGameData
    {
    public:
        // CONSTRUCTION.
        static SavedGameData DefaultSavedGameData();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The player's world position.
        MATH::Vector2f PlayerWorldPosition = MATH::Vector2f();
    };
}
