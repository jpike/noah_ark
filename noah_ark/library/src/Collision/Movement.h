#pragma once

#include "Core/Direction.h"
#include "Math/Vector2.h"

namespace COLLISION
{
    /// Describes movement by a particular amount in a particular direction.
    class Movement
    {
    public:
        // CONSTRUCTION.
        explicit Movement(
            const CORE::Direction direction = CORE::Direction::INVALID,
            const float distance_in_pixels = 0.0f);

        // CONVERSION.
        MATH::Vector2f ToVector() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        CORE::Direction Direction;  ///< The direction of the movement.
        float DistanceInPixels; ///< The distance of the movement (in pixels).
    };
}