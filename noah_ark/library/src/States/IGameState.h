#pragma once

#include "Graphics/Camera.h"
#include "Graphics/RenderBatch.h"

/// Code for game states and their management.
namespace STATES
{
    /// An interface for any game states.
    class IGameState
    {
    public:
        /// Virtual destructor for this interface.
        virtual ~IGameState() {};

        /// Updates the state for a single frame of the game.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time since the last frame update, in seconds.
        /// @return True if the game should terminate and no longer update;
        ///     false if the game should continue updating.
        virtual bool Update(const float elapsed_time_in_seconds) = 0;
    };
}