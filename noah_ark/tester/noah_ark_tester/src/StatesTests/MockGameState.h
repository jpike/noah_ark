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

        /// @brief  Mock update method.
        /// @return Always false.  Maybe be updated later if needed.
        virtual bool Update() { return false; };
    };
}