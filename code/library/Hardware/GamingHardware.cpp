#include "Hardware/GamingHardware.h"

namespace HARDWARE
{
    /// Attempts to initialize the gaming hardware.
    /// If an error occurs initializing some subset of the hardware,
    /// the partially initialized hardware will be returned to allow
    /// the game to still be played with fewer features.
    /// @return The gaming hardware.
    GamingHardware GamingHardware::Initialize()
    {
        GamingHardware gaming_hardware;

        gaming_hardware.Screen = GRAPHICS::Screen::Create();
        gaming_hardware.Speakers = std::make_unique<AUDIO::Speakers>();

        return gaming_hardware;
    }
}
