#pragma once

#include "Graphics/Sprite.h"
#include "Math/Vector2.h"

namespace OBJECTS
{
    /// An olive leaf that the dove brings back in Genesis 8:11.
    class OliveLeaf
    {
    public:
        // CONSTRUCTION.
        explicit OliveLeaf(const MATH::Vector2f& world_position);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The olive leaf's sprite.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();
    };
}
