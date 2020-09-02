#pragma once

#include <memory>
#include "Audio/Speakers.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Screen.h"
#include "Hardware/Clock.h"
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
        explicit GamingHardware();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The clock keeping track of time.
        Clock Clock = {};
        /// The controller supplying user input for the game.
        INPUT_CONTROL::InputController InputController = {};
        /// The screen to which the game is rendered.
        /// @todo   See about not making this a shared pointer!
        std::shared_ptr<GRAPHICS::Screen> Screen = nullptr;
        /// The device for rendering graphics on the system.
        /// Dynamically allocated since the device has a mutex that prevents copying/moving.
        std::unique_ptr<GRAPHICS::GraphicsDevice> GraphicsDevice = nullptr;
        /// The speakers out of which audio can be played.
        /// Dynamically allocated since the speakers have a mutex that prevents copying/moving.
        std::unique_ptr<AUDIO::Speakers> Speakers = nullptr;
    };
}
