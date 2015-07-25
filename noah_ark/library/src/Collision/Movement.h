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

        /// Constructor.
        explicit Movement() :
            Direction(CORE::Direction::INVALID),
            DistanceInPixels(0.0f)
        {}

        /// Converts the movement to a corresponding vector.
        /// @return A vector corresponding to this movement.
        MATH::Vector2f ToVector() const;
    };
}