#pragma once

#include <memory>
#include "States/IGameState.h"

namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// \brief  Responsible for managing game states.
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

    private:
        StateManager(const StateManager& managerToCopy);  ///< Private to disallow copying.
        StateManager& operator= (const StateManager& rhsManager); ///< Private to disallow assignment.

        std::shared_ptr<IGameState> m_currentState;   ///< The current state being managed.
    };
}