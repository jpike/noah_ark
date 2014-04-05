#pragma once

#include <memory>
#include "States/IGameState.h"

namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// @brief  Responsible for managing game states.
    ///////////////////////////////////////////////////////////
    class StateManager
    {
    public:
        /// @brief  Constructor.
        explicit StateManager();
        /// @brief  Destructor.
        ~StateManager();

        /// @brief  Gets the current state being managed.
        /// @return The current state.
        std::shared_ptr<IGameState> GetCurrentState() const;

        /// @brief      Sets the current state being managed.
        /// @param[in]  newState - The new state to set.
        void SetCurrentState(const std::shared_ptr<IGameState>& newState);

        /// @brief      Updates the current state for a single frame of the game.
        /// @return     True if the game should terminate and no longer update;
        ///             false if the game should continue updating.
        bool Update();

    private:
        StateManager(const StateManager& managerToCopy);  ///< Private to disallow copying.
        StateManager& operator= (const StateManager& rhsManager); ///< Private to disallow assignment.

        std::shared_ptr<IGameState> m_currentState;   ///< The current state being managed.
    };
}