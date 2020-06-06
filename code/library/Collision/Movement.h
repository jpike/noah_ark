#pragma once

#include "Gameplay/Direction.h"
#include "Math/Vector2.h"

namespace COLLISION
{
    /// Describes movement by a particular amount in a particular direction.
    class Movement
    {
    public:
        // CONSTRUCTION.
        explicit Movement(
            const GAMEPLAY::Direction direction = GAMEPLAY::Direction::INVALID,
            const float distance_in_pixels = 0.0f);

        // CONVERSION.
        MATH::Vector2f ToVector() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The direction of the movement.
        GAMEPLAY::Direction Direction;
        /// The distance of the movement (in pixels).
        float DistanceInPixels;
    };
}