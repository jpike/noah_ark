#pragma once

#include "Graphics/Sprite.h"

namespace OBJECTS
{
    /// A collection of logs that supply wood in the game.
    /// These logs appear after a tree has been chopped down
    /// and the dust cloud has cleared.
    class WoodLogs
    {
    public:
        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the wood logs.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();
    };
}
