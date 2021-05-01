#pragma once

#include "Graphics/Sprite.h"
#include "Math/Vector2.h"

namespace OBJECTS
{
    /// The altar that Noah can build.
    class Altar
    {
    public:
        // CONSTRUCTION.
        explicit Altar(const MATH::Vector2f& center_world_position);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the altar.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();
    };
}
