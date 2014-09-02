#pragma once

#include <list>
#include <memory>
#include "Physics/Collision/BoxCollider.h"
#include "Physics/Collision/ICollisionComponent.h"

/// Holds physics-related functionality.
namespace PHYSICS
{
/// Holds code related to collisions.
namespace COLLISION
{
    ///////////////////////////////////////////////////////////
    /// @brief  Responsible for detecting and handling collisions.
    ///
    ///         Collision components created from this class
    ///         are managed by this class as long as they are
    ///         referenced elsewhere in the game.
    ///
    ///         Calling SimulateMovement will handle movement
    ///         and collision detection/handling for a specified
    ///         amount of time for any managed collision components.
    ///////////////////////////////////////////////////////////
    class CollisionSystem
    {
    public:
        /// @brief  Constructor.
        explicit CollisionSystem();
        /// @brief  Destructor.
        ~CollisionSystem();

        /// @brief      Creates a box collider based on the specified parameters.
        /// @param[in]  centerXWorldPositionInPixels - The center X world position of the box (in pixels).
        /// @param[in]  centerYWorldPositionInPixels - The center Y world position of the box (in pixels).
        /// @param[in]  widthInPixels - The width of the box (in pixels).
        /// @param[in]  heightInPixels - The height of the box (in pixels).
        /// @return     The box collider, if successfully created.  Otherwise, an unset pointer type.
        std::shared_ptr<BoxCollider> CreateBoxCollider(
            const float centerXWorldPositionInPixels,
            const float centerYWorldPositionInPixels,
            const float widthInPixels,
            const float heightInPixels);

        /// @brief      Determines if a collision component is managed by this system.
        /// @param[in]  collisionComponent - The collision component to check.
        /// @return     True if the provided collision component is managed by this system; false otherwise.
        bool ManagesCollisionComponent(const std::shared_ptr<ICollisionComponent>& collisionComponent) const;

    private:
        CollisionSystem(const CollisionSystem& systemToCopy);  ///< Private to disallow copying.
        CollisionSystem& operator= (const CollisionSystem& rhsSystem); ///< Private to disallow assignment.

        /// @brief  The collision components managed by this system.  The methods for creating
        ///         these components return shared_ptrs, but they are stored as weak_ptrs
        ///         to allow their memory to be properly released once the objects holding
        ///         the shared_ptrs are deleted.
        std::list< std::weak_ptr<ICollisionComponent> > m_collisionComponents;
    };
}
}