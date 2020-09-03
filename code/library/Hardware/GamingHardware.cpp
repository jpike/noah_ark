#include "Hardware/GamingHardware.h"

namespace HARDWARE
{
    /// Attempts to initialize the gaming hardware.
    /// If an error occurs initializing some subset of the hardware,
    /// the partially initialized hardware will be returned to allow
    /// the game to still be played with fewer features.
    GamingHardware::GamingHardware()
    {
        Screen = GRAPHICS::Screen::Create();
        GraphicsDevice = std::make_shared<GRAPHICS::GraphicsDevice>();
        Speakers = std::make_unique<AUDIO::Speakers>();
    }
}
