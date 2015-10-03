#pragma once

#include <list>
#include <memory>
#include <vector>
#include "Collision/BoxCollider.h"
#include "Collision/ICollisionComponent.h"
#include "Events/AxeSwingEvent.h"
#include "Math/Rectangle.h"

/// Holds code related to collisions.
namespace COLLISION
{
    /// Responsible for detecting and handling collisions.
    ///
    /// Collision components created from this class
    /// are managed by this class as long as they are
    /// referenced elsewhere in the game.
    ///
    /// Calling SimulateMovement will handle movement
    /// and collision detection/handling for a specified
    /// amount of time for any managed collision components.
    /*class CollisionSystem
    {
    public:
        /// Constructor.
        explicit CollisionSystem();
        /// Destructor.
        ~CollisionSystem();

        /// Sets the overworld map that provides basic tile map
        /// collision data for the collision system.
        /// @param[in]  overworld_map - The overworld map to set as a tile map
        ///     collision data source for the collision system.
        void SetOverworldMap(const std::shared_ptr<MAPS::OverworldMap>& overworld_map);
        
        /// Adds an axe swing event to the collision system for processing.
        /// @param[in]  axe_swing_event - The axe swing event to add.
        void Add(const std::shared_ptr<EVENTS::AxeSwingEvent>& axe_swing_event);

        /// Updates the collision system by a single iteration of processing.
        void Update();

        /// Simulates movement for all collision components managed by this system.
        void SimulateMovement();

        /// Creates a box collider based on the specified parameters.
        /// @param[in]  center_x_world_position_in_pixels - The center X world position of the box (in pixels).
        /// @param[in]  center_y_world_position_in_pixels - The center Y world position of the box (in pixels).
        /// @param[in]  width_in_pixels - The width of the box (in pixels).
        /// @param[in]  height_in_pixels - The height of the box (in pixels).
        /// @return The box collider, if successfully created.  Otherwise, an unset pointer type.
        std::shared_ptr<BoxCollider> CreateBoxCollider(
            const float center_x_world_position_in_pixels,
            const float center_y_world_position_in_pixels,
            const float width_in_pixels,
            const float height_in_pixels);

        /// Determines if a collision component is managed by this system.
        /// @param[in]  collision_component - The collision component to check.
        /// @return True if the provided collision component is managed by this system; false otherwise.
        bool ManagesCollisionComponent(const std::shared_ptr<ICollisionComponent>& collision_component) const;

    private:
        CollisionSystem(const CollisionSystem&);  ///< Private to disallow copying.
        CollisionSystem& operator= (const CollisionSystem&); ///< Private to disallow assignment.

        /// Removes unusued collision components from the system.  Allows freeing
        /// memory for collision components that are no longer being used elsewhere
        /// in the game.
        void RemoveUnusedCollisionComponents();

        /// Simulates the provided movement for the specified collision component.
        /// Any collisions that occur are handled by this method.
        /// @param[in]  movement - The movement to simulate.
        /// @param[in,out]  collision_component - The collison component for which to simulate movement.
        void SimulateMovement(const Movement& movement, ICollisionComponent& collision_component) const;

        /// Simulates the provided upward movement for the specified collision component.
        /// Any collisions that occur are handled by this method.
        /// @param[in]  movement - The movement to simulate.  It must be in the up direction.
        /// @param[in,out]  collision_component - The collision component for which to simulate movement.
        void SimulateUpMovement(const Movement& movement, ICollisionComponent& collision_component) const;
        /// Simulates the provided downward movement for the specified collision component.
        /// Any collisions that occur are handled by this method.
        /// @param[in]  movement - The movement to simulate.  It must be in the down direction.
        /// @param[in,out]  collision_component - The collision component for which to simulate movement.
        void SimulateDownMovement(const Movement& movement, ICollisionComponent& collision_component) const;
        /// Simulates the provided leftward movement for the specified collision component.
        /// Any collisions that occur are handled by this method.
        /// @param[in]  movement - The movement to simulate.  It must be in the left direction.
        /// @param[in,out]  collision_component - The collision component for which to simulate movement.
        void SimulateLeftMovement(const Movement& movement, ICollisionComponent& collision_component) const;
        /// Simulates the provided rightward movement for the specified collision component.
        /// Any collisions that occur are handled by this method.
        /// @param[in]  movement - The movement to simulate.  It must be in the right direction.
        /// @param[in,out]  collision_component - The collision component for which to simulate movement.
        void SimulateRightMovement(const Movement& movement, ICollisionComponent& collision_component) const;

        /// Determines if the provided rectangle collides with a tree.
        /// @param[in]  rectangle - The rectangle (in world coordinates) to check
        ///     for collision with any tree.
        /// @param[out] tree_rectangle - The rectangle (in world coordinates) of the tree
        ///     that was collided with, if a collision occured.
        /// @return True if the provided rectangle collides with a tree; false otherwise.
        bool CollidesWithTree(const MATH::FloatRectangle& rectangle, MATH::FloatRectangle& tree_rectangle) const;

        /// Processes the axe swings to handle any collisions.
        /// @param[in,out]  axe_swings - The axe swings to process.  They may be removed from
        ///     the container if their processing if complete.
        void HandleCollisions(std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> >& axe_swings);

        /// Handles collisions of an axe with trees.
        /// @param[in]  axe_blade_bounds - The world bounding rectangle for the axe's blade.
        void HandleAxeCollisionsWithTrees(const MATH::FloatRectangle& axe_blade_bounds);

        /// The collision components managed by this system.  The methods for creating
        /// these components return shared_ptrs, but they are stored as weak_ptrs
        /// to allow their memory to be properly released once the objects holding
        /// the shared_ptrs are deleted.
        std::list< std::weak_ptr<ICollisionComponent> > CollisionComponents;

        /// Axe swing events currently being processed.
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> > AxeSwings;

        std::shared_ptr<MAPS::OverworldMap> OverworldMap; ///< The overworld map supplying tile map collision data.
    };*/
}