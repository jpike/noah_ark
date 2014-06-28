#pragma once

/// Code for game states and their management.
namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// @brief  An interface for any game states.
    ///////////////////////////////////////////////////////////
    class IGameState
    {
    public:
        /// @brief  Virtual destructor for this interface.
        virtual ~IGameState() {};

        /// @brief      Updates the state for a single frame of the game.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time since the last frame update, in seconds.
        /// @return     True if the game should terminate and no longer update;
        ///             false if the game should continue updating.
        virtual bool Update(const float elapsedTimeInSeconds) = 0;
    };
}