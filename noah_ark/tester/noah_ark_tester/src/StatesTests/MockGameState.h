#pragma once

#include "States/IGameState.h"

namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// @brief  A mock game state for testing.
    ///////////////////////////////////////////////////////////
    class MockGameState : public IGameState
    {
    public:
        /// @brief  Destructor.
        virtual ~MockGameState() {};

        /// @brief      Mock update method.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time since the last frame update, in seconds.
        /// @return     Always false.  Maybe be updated later if needed.
        virtual bool Update(const float elapsedTimeInSeconds) { return false; };
    };
}