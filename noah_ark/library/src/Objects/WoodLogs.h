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
        // POSITIONING/BOUNDARIES.
        /// Gets the bounding box of the wood logs, in world coordinates.
        /// @return The wood logs' bounding box.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the wood logs.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();
    };
}
