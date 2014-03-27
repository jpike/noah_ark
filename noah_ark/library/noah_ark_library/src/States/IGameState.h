#pragma once

/// Code for game states and their management.
namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// \brief  An interface for any game states.
    ///////////////////////////////////////////////////////////
    class IGameState
    {
    public:
        /// @brief  Virtual destructor for this interface.
        virtual ~IGameState() {};
    };
}