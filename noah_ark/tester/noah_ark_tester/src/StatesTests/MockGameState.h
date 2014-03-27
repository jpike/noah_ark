#pragma once

#include "States/IGameState.h"

namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// \brief  A mock game state for testing.
    ///////////////////////////////////////////////////////////
    class MockGameState : public IGameState
    {
    public:
        /// @brief  Destructor.
        virtual ~MockGameState() {};
    };
}