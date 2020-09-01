#pragma once

#include <memory>
#include <SFML/System.hpp>
#include "Audio/Speakers.h"
#include "Graphics/Screen.h"
#include "Input/InputController.h"

/// Holds code related to generic computing hardware.
/// Computing hardware specific to another subsystem may exist in a separate namespace.
namespace HARDWARE
{
    /// The entire set of gaming/computing hardware the game is being played on.
    class GamingHardware
    {
    public:
        // CONSTRUCTION.
        static GamingHardware Initialize();

        // OTHER METHODS.
        void TickClockForFrame();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The clock keeping track of time for the purposes of the game loop.
        sf::Clock GameLoopClock = {};
        /// The total elapsed time since the game started.
        sf::Time TotalElapsedTime = sf::Time::Zero;
        /// The elapsed time just since the last frame.
        sf::Time ElapsedTimeSinceLastFrame = sf::Time::Zero;
        /// The controller supplying user input for the game.
        INPUT_CONTROL::InputController InputController = {};
        /// The screen to which the game is rendered.
        /// @todo   See about not making this a shared pointer!
        std::shared_ptr<GRAPHICS::Screen> Screen = nullptr;
        /// The speakers out of which audio can be played.
        /// Dynamically allocated since the speakers have a mutex that prevents copying/moving.
        std::unique_ptr<AUDIO::Speakers> Speakers = nullptr;
    };
}
