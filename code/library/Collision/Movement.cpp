#include "Collision/Movement.h"

namespace COLLISION
{
    /// Constructor.  Parameters are optional to allow easy default
    /// construction with invalid values.
    /// @param[in]  direction - The direction of the movement.
    /// @param[in]  distance_in_pixels - The distance of the movement (in pixels).
    Movement::Movement(
        const CORE::Direction direction,
        const float distance_in_pixels) :
    Direction(direction),
    DistanceInPixels(distance_in_pixels)
    {}

    /// Converts the movement to a corresponding vector.
    /// @return A vector corresponding to this movement.
    MATH::Vector2f Movement::ToVector() const
    {
        // CONVERT THE MOVEMENT TO A VECTOR.
        switch (Direction)
        {
            case CORE::Direction::UP:
                // Y decreases going up.
                return MATH::Vector2f(0.0f, -1.0f * DistanceInPixels);
            case CORE::Direction::DOWN:
                // Y increases going down.
                return MATH::Vector2f(0.0f, DistanceInPixels);
            case CORE::Direction::LEFT:
                return MATH::Vector2f(-1.0f * DistanceInPixels, 0.0f);
            case CORE::Direction::RIGHT:
                return MATH::Vector2f(DistanceInPixels, 0.0f);
            case CORE::Direction::INVALID:
                // Intentionally fall through.
            default:
                // No valid movement vector can be determined.
                return MATH::Vector2f(0.0f, 0.0f);
        }
    }
}