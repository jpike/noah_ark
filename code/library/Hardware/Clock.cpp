#include "Hardware/Clock.h"

namespace HARDWARE
{
    /// Updates the clock's elapsed time, updating internal member variables.
    void Clock::UpdateElapsedTime()
    {
        ElapsedTimeSinceLastFrame = GameLoopClock.restart();
        TotalElapsedTime += ElapsedTimeSinceLastFrame;
    }
}
