#pragma once

#include <hgerect.h>

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
        virtual hgeRect GetBoundingBox() const = 0;
    };
}
}