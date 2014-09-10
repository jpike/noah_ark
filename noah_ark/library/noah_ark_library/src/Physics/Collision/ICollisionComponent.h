#pragma once

#include "Core/Direction.h"
#include "Math/FloatRectangle.h"

namespace PHYSICS
{
namespace COLLISION
{
    ///////////////////////////////////////////////////////////
    /// @brief  Describes movement by a particular amount in a particular direction.
    ///////////////////////////////////////////////////////////
    struct Movement
    {
        CORE::Direction Direction;  ///< The direction of the movement.
        float DistanceInPixels; ///< The distance of the movement (in pixels).

        /// @brief  Constructor.
        explicit Movement() :
            Direction(CORE::Direction::INVALID),
            DistanceInPixels(0.0f)
        {}
    };

    ///////////////////////////////////////////////////////////
    /// @brief  An interface for collision components.  Collision
    ///         components define details for how collisions on
    ///         particular objects may be detected and handled.
    ///////////////////////////////////////////////////////////
    class ICollisionComponent
    {
    public:
        /// @brief  Virtual destructor for this interface.
        virtual ~ICollisionComponent() {};

        /// @brief  Gets the bounding box (in world coordinates) for this component.
        /// @return The bounding box for this component.
        virtual MATH::FloatRectangle GetBoundingBox() const = 0;

        /// @brief      Requests that the collision component move a certan amount
        ///             in a specified direction.
        /// @parma[in]  moveDirection - The direction to move.
        /// @param[in]  moveDistanceInPixels - The distance to move (in pixels).
        virtual void RequestMovement(const CORE::Direction moveDirection, const float moveDistanceInPixels) = 0;
    };
}
}