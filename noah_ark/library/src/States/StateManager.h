#pragma once

#include <memory>
#include "States/IGameState.h"

namespace STATES
{
    /// Responsible for managing game states.
    class StateManager
    {
    public:
        /// Constructor.
        explicit StateManager();
        /// Destructor.
        ~StateManager();

        /// Gets the current state being managed.
        /// @return The current state.
        std::shared_ptr<IGameState> GetCurrentState() const;

        /// Sets the current state being managed.
        /// @param[in]  newState - The new state to set.
        void SetCurrentState(const std::shared_ptr<IGameState>& newState);

        /// Updates the current state for a single frame of the game.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time since the last frame update, in seconds.
        /// @return     True if the game should terminate and no longer update;
        ///             false if the game should continue updating.
        bool Update(const float elapsedTimeInSeconds);

    private:
        StateManager(const StateManager&);  ///< Private to disallow copying.
        StateManager& operator= (const StateManager&); ///< Private to disallow assignment.

        std::shared_ptr<IGameState> m_currentState;   ///< The current state being managed.
    };
}