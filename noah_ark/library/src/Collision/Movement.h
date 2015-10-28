#pragma once

#include "Core/Direction.h"
#include "Math/Vector2.h"

namespace COLLISION
{
    /// Describes movement by a particular amount in a particular direction.
    struct Movement
    {
        CORE::Direction Direction;  ///< The direction of the movement.
        float DistanceInPixels; ///< The distance of the movement (in pixels).

        /// Constructor.  Parameters are optional to allow easy default
        /// construction with invalid values.
        /// @param[in]  direction - The direction of the movement.
        /// @param[in]  distance_in_pixels - The distance of the movement (in pixels).
        explicit Movement(
            const CORE::Direction direction = CORE::Direction::INVALID, 
            const float distance_in_pixels = 0.0f) :
        Direction(direction),
        DistanceInPixels(distance_in_pixels)
        {}

        /// Converts the movement to a corresponding vector.
        /// @return A vector corresponding to this movement.
        MATH::Vector2f ToVector() const;
    };
}