#pragma once

#include <SFML/System.hpp>

namespace HARDWARE
{
    /// A clock keeping track of time on the system.
    /// This class is specifically focused on tracking time
    /// for the purposes of frame updates.
    class Clock
    {
    public:
        // PUBLIC METHODS.
        void UpdateElapsedTime();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The total elapsed time since the game started.
        sf::Time TotalElapsedTime = sf::Time::Zero;
        /// The elapsed time just since the last frame.
        sf::Time ElapsedTimeSinceLastFrame = sf::Time::Zero;

    private:
        // PRIVATE MEMBER VARIABLES.
        /// The clock keeping track of time for the purposes of the game loop.
        sf::Clock GameLoopClock = {};
    };
}
