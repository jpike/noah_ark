#pragma once

#include <memory>
#include "Math/Rectangle.h"
#include "Physics/Collision/Movement.h"

namespace PHYSICS
{
namespace COLLISION
{
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
        /// @param[in]  moveDirection - The direction to move.
        /// @param[in]  moveDistanceInPixels - The distance to move (in pixels).
        virtual void RequestMovement(const CORE::Direction moveDirection, const float moveDistanceInPixels) = 0;

        /// @brief  Gets any movement requested of the collision component.
        ///         Since the movement is returned as a unique pointer,
        ///         calling this method also clears the requested movement
        ///         from the actual collision component.
        /// @return The current movement requested for the collision component, if any.
        virtual std::unique_ptr<Movement> GetRequestedMovement() = 0;

        /// @brief      Moves the collision component the amount specified by the provided vector.
        /// @param[in]  movementVector - The vector to move by.
        virtual void Move(const MATH::Vector2f& movementVector) = 0;
    };
}
}