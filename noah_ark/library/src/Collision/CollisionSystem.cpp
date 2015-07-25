#include <algorithm>
#include <cassert>
#include <iostream>
#include "Collision/CollisionSystem.h"

namespace COLLISION
{
    CollisionSystem::CollisionSystem() :
    CollisionComponents(),
    AxeSwings(),
    OverworldMap()
    {}

    CollisionSystem::~CollisionSystem()
    {}

    void CollisionSystem::SetOverworldMap(const std::shared_ptr<MAPS::OverworldMap>& overworld_map)
    {
        OverworldMap = overworld_map;
    }

    void CollisionSystem::Add(const std::shared_ptr<EVENTS::AxeSwingEvent>& axe_swing_event)
    {
        // MAKE SURE THE AXE SWING EVENT EXISTS.
        bool event_exists = (nullptr != axe_swing_event);
        assert(event_exists);
        if (!event_exists)
        {
            // If no event was provided, then there isn't anything to add.
            return;
        }

        // ADD THE EVENT FOR PROCESSING.
        AxeSwings.push_back(axe_swing_event);
    }

    void CollisionSystem::Update()
    {
        SimulateMovement();
        HandleCollisions(AxeSwings);
    }

    void CollisionSystem::SimulateMovement()
    {
        // SIMULATE MOVEMENT FOR ALL COLLISION COMPONENTS.
        for (std::weak_ptr<ICollisionComponent>& collision_component : CollisionComponents)
        {
            // ATTEMPT TO OBTAIN A USABLE COLLISION COMPONENT.
            std::shared_ptr<ICollisionComponent> current_collision_component = collision_component.lock();
            bool current_collision_component_still_exists = (nullptr != current_collision_component);
            if (!current_collision_component_still_exists)
            {
                // Move on to the next collision component.
                continue;
            }

            // SEE IF ANY MOVEMENT WAS REQUEST FOR THE CURRENT COLLISION COMPONENT.
            std::unique_ptr<Movement> requested_movement = current_collision_component->GetRequestedMovement();
            if (!requested_movement)
            {
                // Move on to the next collision component.
                continue;
            }

            // SIMULATE MOVEMENT FOR THE COLLISION COMPONENT.
            SimulateMovement(*requested_movement, *current_collision_component);
        }

        // REMOVE ANY COLLISION COMPONENTS THAT ARE NO LONGER NEEDED.
        RemoveUnusedCollisionComponents();
    }

    std::shared_ptr<BoxCollider> CollisionSystem::CreateBoxCollider(
        const float center_x_world_position_in_pixels,
        const float center_y_world_position_in_pixels,
        const float width_in_pixels,
        const float height_in_pixels)
    {
        // VALIDATE THE BOX DIMENSIONS.
        // The dimensions are required to be positive to ensure that
        // the box has an actual area.
        bool valid_width = (width_in_pixels > 0);
        bool valid_height = (height_in_pixels > 0);
        bool valid_dimensions = (valid_width && valid_height);
        if (!valid_dimensions)
        {
            // No valid box collider could be created.
            return std::shared_ptr<BoxCollider>();
        }

        // CREATE THE BOX COLLIDER MANAGED BY THIS SYSTEM
        std::shared_ptr<BoxCollider> box_collider = std::make_shared<BoxCollider>(
            center_x_world_position_in_pixels,
            center_y_world_position_in_pixels,
            width_in_pixels,
            height_in_pixels);

        CollisionComponents.push_back(box_collider);

        return box_collider;
    }

    bool CollisionSystem::ManagesCollisionComponent(const std::shared_ptr<ICollisionComponent>& collision_component) const
    {
        // SEARCH FOR THE COLLISION COMPONENT IN THE CONTAINER.
        // We have to loop over the components manually because there isn't an equality
        // operator directly for weak pointers (as stored in the container).
        auto collision_component_iterator = CollisionComponents.cend();
        for (collision_component_iterator = CollisionComponents.cbegin();
            collision_component_iterator != CollisionComponents.cend();
            ++collision_component_iterator)
        {
            // Attempt to convert the current collision component to a shared_ptr for comparison.
            const std::shared_ptr<ICollisionComponent> current_collision_component = collision_component_iterator->lock();

            // Check if the current collision component matches the provided one.
            bool specified_collision_component_found = (current_collision_component == collision_component);
            if (specified_collision_component_found)
            {
                // Break out of the loop since the specified collision component was found.
                break;
            }
        }

        // DETERMINE IF THE COLLISION COMPONENT WAS FOUND.
        bool collision_component_found = (CollisionComponents.cend() != collision_component_iterator);
        return collision_component_found;
    }

    void CollisionSystem::RemoveUnusedCollisionComponents()
    {
        // Remove any collsion components that are no longer being used.
        CollisionComponents.remove_if(
            [](std::weak_ptr<ICollisionComponent>& collision_component) { return collision_component.expired(); });
    }

    void CollisionSystem::SimulateMovement(const Movement& movement, ICollisionComponent& collision_component) const
    {
        // SIMULATE MOVEMENT BASED ON THE PARTICULAR DIRECTION.
        switch (movement.Direction)
        {
            case CORE::Direction::UP:
                SimulateUpMovement(movement, collision_component);
                break;
            case CORE::Direction::DOWN:
                SimulateDownMovement(movement, collision_component);
                break;
            case CORE::Direction::LEFT:
                SimulateLeftMovement(movement, collision_component);
                break;
            case CORE::Direction::RIGHT:
                SimulateRightMovement(movement, collision_component);
                break;
            case CORE::Direction::INVALID:
                // Intentionally fall through.
            default:
                // No movement can be simulated for an invalid direction.
                return;
        }
    }

    void CollisionSystem::SimulateUpMovement(const Movement& movement, ICollisionComponent& collision_component) const
    {
        // VERIFY THAT THE MOVEMENT IS FOR THE 'UP' DIRECTION.
        bool movement_for_up_direction = (CORE::Direction::UP == movement.Direction);
        if (!movement_for_up_direction)
        {
            // An invalid movement was provided to this method since the direction was not up.
            return;
        }

        // GRADUALLY MOVE THE COLLISION COMPONENT UPWARD UNTIL WE COLLIDE WITH SOMETHING.
        float distance_left_to_move = movement.DistanceInPixels;
        //auto moreUpMovementToSimulate = [&] ()->bool { (); };
        while (distance_left_to_move > 0.0f)
        {
            // GET THE CURRENT TILES THAT THE TOP OF THE COLLISION COMPONENT IS ON.
            MATH::FloatRectangle collision_box = collision_component.GetBoundingBox();
            float collision_box_top_y_position = collision_box.GetTopYPosition();
            // Adjust the top position to be a little higher to avoid an infinite loop.
            collision_box_top_y_position -= 0.0f;

            // Get the tile for the top-left corner.
            float collision_box_left_x_position = collision_box.GetLeftXPosition();
            // The horizonal tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
            float collision_box_width = collision_box.GetWidth();
            float horizontal_corner_tiles_adjustment_amount = collision_box_width / 4.0f;
            collision_box_left_x_position += horizontal_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> top_left_tile = OverworldMap->GetTileAtWorldPosition(collision_box_left_x_position, collision_box_top_y_position);

            // Get the tile right above the collision box's center.
            float collison_box_center_x_position = collision_box.GetCenterXPosition();
            std::shared_ptr<MAPS::Tile> topCenterTile = OverworldMap->GetTileAtWorldPosition(collison_box_center_x_position, collision_box_top_y_position);

            // Get the tile for the top-right corner.
            float collision_box_right_x_position = collision_box.GetRightXPosition();
            collision_box_right_x_position -= horizontal_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> top_right_tile = OverworldMap->GetTileAtWorldPosition(collision_box_right_x_position, collision_box_top_y_position);

            // MAKE SURE THE TOP TILES EXIST.
            bool top_tiles_exist = (nullptr != top_left_tile) && (nullptr != topCenterTile) && (nullptr != top_right_tile);
            if (!top_tiles_exist)
            {
                // There aren't any tiles above the collision component to move to.
                return;
            }

            // CHECK IF THE TOP TILES ARE WALKABLE.
            bool top_tiles_walkable = (top_left_tile->IsWalkable()) && (topCenterTile->IsWalkable()) && (top_right_tile->IsWalkable());
            if (!top_tiles_walkable)
            {
                // At least one of the top tiles isn't walkable, so it is blocking any further upward movement.
                return;
            }

            // CHECK IF THE CURRENT COLLISION BOX COLLIDES WITH A TREE.
            MATH::FloatRectangle tree_rectangle;
            bool collides_with_tree = CollidesWithTree(collision_box, tree_rectangle);
            if (collides_with_tree)
            {
                // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                // A collision could have occurred with a tree that isn't being moved toward,
                // in which case movement should not be stopped.
                // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
                const float MIN_UP_MOVEMENT = -1.0f;
                float old_collision_box_bottom = collision_box.GetBottomYPosition();
                float old_collision_box_top = collision_box.GetTopYPosition();
                float new_collision_box_top = old_collision_box_top + MIN_UP_MOVEMENT;
                float tree_bottom = tree_rectangle.GetBottomYPosition();

                // A minimum distance between the collision box's center and the tree's
                // center is enforced so that moving objects don't get stuck on
                // parts of trees not directly related to their current movement.
                float min_object_tree_distance = tree_rectangle.GetWidth() / 2.0f;
                float object_center_x = collision_box.GetCenterXPosition();
                float tree_center_x = tree_rectangle.GetCenterXPosition();
                float object_to_tree_distance = fabs(object_center_x - tree_center_x);
                bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

                bool moving_up_collides_with_tree = (
                    collision_distance_met &&
                    (old_collision_box_bottom >= tree_bottom) &&
                    (new_collision_box_top <= tree_bottom) );
                if (moving_up_collides_with_tree)
                {
                    // A tree is blocking further movement.
                    return;
                }
            }

            // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
            float top_tile_top_y_position = topCenterTile->GetTopYPosition();
            float distance_from_top_of_collision_box_to_top_of_tile = fabs(collision_box_top_y_position - top_tile_top_y_position);
            bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_top_of_collision_box_to_top_of_tile);
            if (remaining_movement_confined_to_current_tile)
            {
                // Move the collision component the total movement amount since it is confined
                // to a walkable tile.
                // Upward movement is in the negative Y direction.
                collision_component.Move(MATH::Vector2f(0.0f, -1.0f * distance_left_to_move));

                // The entire movement has completed, so return.
                return;
            }
            else
            {
                // Ensure that we move up at least 1 pixel to prevent an infinite loop.
                float distance_to_move_for_current_tile = std::max(distance_from_top_of_collision_box_to_top_of_tile, 1.0f);

                // Move the collision component to the top of the current top tile.
                // Upward movement is in the negative Y direction.
                collision_component.Move(MATH::Vector2f(0.0f, -1.0f * distance_to_move_for_current_tile));

                // Track the distance we just moved.
                distance_left_to_move -= distance_to_move_for_current_tile;

                // At this point, the loop will continue another iteration.
                // New top tiles should be detected for the new position of the collision component,
                // allowing movement to be simulated for the next set of top tiles.
            }
        }
    }

    void CollisionSystem::SimulateDownMovement(const Movement& movement, ICollisionComponent& collision_component) const
    {
        // VERIFY THAT THE MOVEMENT IS FOR THE 'DOWN' DIRECTION.
        bool movement_for_down_direction = (CORE::Direction::DOWN == movement.Direction);
        if (!movement_for_down_direction)
        {
            // An invalid movement was provided to this method since the direction was not down.
            return;
        }

        // GRADUALLY MOVE THE COLLISION COMPONENT DOWNWARD UNTIL WE COLLIDE WITH SOMETHING.
        float distance_left_to_move = movement.DistanceInPixels;
        while (distance_left_to_move > 0.0f)
        {
            // GET THE CURRENT TILES THAT THE BOTTOM OF THE COLLISION COMPONENT IS ON.
            MATH::FloatRectangle collision_box = collision_component.GetBoundingBox();
            float collision_box_bottom_y_position = collision_box.GetBottomYPosition();

            // The horizonal tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
            // We will adjust the corners to move them closer to the center of the collision box.
            float collision_box_width = collision_box.GetWidth();
            float horizontal_corner_tiles_adjustment_amount = collision_box_width / 4.0f;

            // Get the tile for the bottom-left corner.
            float collision_box_left_x_position = collision_box.GetLeftXPosition();
            collision_box_left_x_position += horizontal_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> bottom_left_tile = OverworldMap->GetTileAtWorldPosition(collision_box_left_x_position, collision_box_bottom_y_position);

            // Get the tile right below the collision box's center.
            float collison_box_center_x_position = collision_box.GetCenterXPosition();
            std::shared_ptr<MAPS::Tile> bottom_center_tile = OverworldMap->GetTileAtWorldPosition(collison_box_center_x_position, collision_box_bottom_y_position);

            // Get the tile for the bottom-right corner.
            float collision_box_right_x_position = collision_box.GetRightXPosition();
            collision_box_right_x_position -= horizontal_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> bottom_right_tile = OverworldMap->GetTileAtWorldPosition(collision_box_right_x_position, collision_box_bottom_y_position);

            // MAKE SURE THE BOTTOM TILES EXIST.
            bool bottom_tiles_exist = (nullptr != bottom_left_tile) && (nullptr != bottom_center_tile) && (nullptr != bottom_right_tile);
            if (!bottom_tiles_exist)
            {
                // There aren't any tiles below the collision component to move to.
                return;
            }

            // CHECK IF THE BOTTOM TILES ARE WALKABLE.
            bool bottom_tiles_walkable = (bottom_left_tile->IsWalkable()) && (bottom_center_tile->IsWalkable()) && (bottom_right_tile->IsWalkable());
            if (!bottom_tiles_walkable)
            {
                // At least one of the bottom tiles isn't walkable, so it is blocking any further downward movement.
                return;
            }

            // CHECK IF THE CURRENT COLLISION BOX COLLIDES WITH A TREE.
            MATH::FloatRectangle tree_rectangle;
            bool collides_with_tree = CollidesWithTree(collision_box, tree_rectangle);
            if (collides_with_tree)
            {
                // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                // A collision could have occurred with a tree that isn't being moved toward,
                // in which case movement should not be stopped.
                // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
                const float MIN_DOWN_MOVEMENT = 1.0f;
                float old_collision_box_top = collision_box.GetTopYPosition();
                float old_collision_box_bottom = collision_box.GetBottomYPosition();
                float new_collision_box_bottom = old_collision_box_bottom + MIN_DOWN_MOVEMENT;
                float tree_top = tree_rectangle.GetTopYPosition();

                // A minimum distance between the collision box's center and the tree's
                // center is enforced so that moving objects don't get stuck on
                // parts of trees not directly related to their current movement.
                float min_object_tree_distance = tree_rectangle.GetWidth() / 2.0f;
                float object_center_x = collision_box.GetCenterXPosition();
                float tree_center_x = tree_rectangle.GetCenterXPosition();
                float object_to_tree_distance = fabs(object_center_x - tree_center_x);
                bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

                bool moving_down_collides_with_tree = (
                    collision_distance_met &&
                    (old_collision_box_top <= tree_top) &&
                    (new_collision_box_bottom >= tree_top) );
                if (moving_down_collides_with_tree)
                {
                    // A tree is blocking further movement.
                    return;
                }
            }

            // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
            float bottom_tile_bottom_y_position = bottom_center_tile->GetBottomYPosition();
            float distance_from_bottom_of_collision_box_to_bottom_of_tile = fabs(bottom_tile_bottom_y_position - collision_box_bottom_y_position);
            bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_bottom_of_collision_box_to_bottom_of_tile);
            if (remaining_movement_confined_to_current_tile)
            {
                // Move the collision component the total movement amount since it is confined
                // to a walkable tile.
                // Downward movement is in the positive Y direction.
                collision_component.Move(MATH::Vector2f(0.0f, distance_left_to_move));

                // The entire movement has completed, so return.
                return;
            }
            else
            {
                // Ensure that we move down at least 1 pixel to prevent an infinite loop.
                float distance_to_move_for_current_til = std::max(distance_from_bottom_of_collision_box_to_bottom_of_tile, 1.0f);

                // Move the collision component to the bottom of the current bottom tile.
                // Downward movement is in the positive Y direction.
                collision_component.Move(MATH::Vector2f(0.0f, distance_to_move_for_current_til));

                // Track the distance we just moved.
                distance_left_to_move -= distance_to_move_for_current_til;

                // At this point, the loop will continue another iteration.
                // New bottom tiles should be detected for the new position of the collision component,
                // allowing movement to be simulated for the next set of bottom tiles.
            }
        }
    }

    void CollisionSystem::SimulateLeftMovement(const Movement& movement, ICollisionComponent& collision_component) const
    {
        // VERIFY THAT THE MOVEMENT IS FOR THE 'LEFT' DIRECTION.
        bool movement_for_left_direction = (CORE::Direction::LEFT == movement.Direction);
        if (!movement_for_left_direction)
        {
            // An invalid movement was provided to this method since the direction was not left.
            return;
        }

        // GRADUALLY MOVE THE COLLISION COMPONENT LEFTWARD UNTIL WE COLLIDE WITH SOMETHING.
        float distance_left_to_move = movement.DistanceInPixels;
        while (distance_left_to_move > 0.0f)
        {
            // GET THE CURRENT TILES THAT THE LEFT OF THE COLLISION COMPONENT IS ON.
            MATH::FloatRectangle collision_box = collision_component.GetBoundingBox();
            float collision_box_left_x_position = collision_box.GetLeftXPosition();

            // The vertical tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
            // We will adjust the corners to move them closer to the center of the collision box.
            float collision_box_height = collision_box.GetHeight();
            float vertical_corner_tiles_adjustment_amount = collision_box_height / 4.0f;

            // Get the tile for the top-left corner.
            float collision_box_top_y_position = collision_box.GetTopYPosition();
            collision_box_top_y_position += vertical_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> top_left_tile = OverworldMap->GetTileAtWorldPosition(collision_box_left_x_position, collision_box_top_y_position);

            // Get the tile to the direct left of the collision box's center.
            float collision_box_center_y_position = collision_box.GetCenterYPosition();
            std::shared_ptr<MAPS::Tile> centerLeftTile = OverworldMap->GetTileAtWorldPosition(collision_box_left_x_position, collision_box_center_y_position);

            // Get the tile for the bottom-left corner.
            float collision_box_bottom_y_position = collision_box.GetBottomYPosition();
            collision_box_bottom_y_position -= vertical_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> bottom_left_tile = OverworldMap->GetTileAtWorldPosition(collision_box_left_x_position, collision_box_bottom_y_position);

            // MAKE SURE THE LEFT TILES EXIST.
            bool left_tiles_exist = (nullptr != top_left_tile) && (nullptr != centerLeftTile) && (nullptr != bottom_left_tile);
            if (!left_tiles_exist)
            {
                // There aren't any tiles left of the collision component to move to.
                return;
            }

            // CHECK IF THE LEFT TILES ARE WALKABLE.
            bool left_tiles_walkable = (top_left_tile->IsWalkable()) && (centerLeftTile->IsWalkable()) && (bottom_left_tile->IsWalkable());
            if (!left_tiles_walkable)
            {
                // At least one of the left tiles isn't walkable, so it is blocking any further leftward movement.
                return;
            }

            // CHECK IF THE CURRENT COLLISION BOX COLLIDES WITH A TREE.
            MATH::FloatRectangle tree_rectangle;
            bool collides_with_tree = CollidesWithTree(collision_box, tree_rectangle);
            if (collides_with_tree)
            {
                // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                // A collision could have occurred with a tree that isn't being moved toward,
                // in which case movement should not be stopped.
                // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
                const float MIN_LEFT_MOVEMENT = -1.0f;
                float old_collision_box_right = collision_box.GetRightXPosition();
                float old_collision_box_left = collision_box.GetLeftXPosition();
                float new_collision_box_left = old_collision_box_left + MIN_LEFT_MOVEMENT;
                float tree_right = tree_rectangle.GetRightXPosition();

                // A minimum distance between the collision box's center and the tree's
                // center is enforced so that moving objects don't get stuck on
                // parts of trees not directly related to their current movement.
                float min_object_tree_distance = tree_rectangle.GetHeight() / 2.0f;
                float object_center_y = collision_box.GetCenterYPosition();
                float tree_center_y = tree_rectangle.GetCenterYPosition();
                float object_to_tree_distance = fabs(object_center_y - tree_center_y);
                bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

                bool moving_left_collides_with_tree = (
                    collision_distance_met &&
                    (old_collision_box_right >= tree_right) &&
                    (new_collision_box_left <= tree_right) );
                if (moving_left_collides_with_tree)
                {
                    // A tree is blocking further movement.
                    return;
                }
            }

            // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
            float left_tile_left_x_position = centerLeftTile->GetLeftXPosition();
            float distance_from_left_of_collision_box_to_left_of_tile = fabs(collision_box_left_x_position - left_tile_left_x_position);
            bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_left_of_collision_box_to_left_of_tile);
            if (remaining_movement_confined_to_current_tile)
            {
                // Move the collision component the total movement amount since it is confined
                // to a walkable tile.
                collision_component.Move(MATH::Vector2f(-1.0f * distance_left_to_move, 0.0f));

                // The entire movement has completed, so return.
                return;
            }
            else
            {
                // Ensure that we move left at least 1 pixel to prevent an infinite loop.
                float distance_to_move_for_current_til = std::max(distance_from_left_of_collision_box_to_left_of_tile, 1.0f);

                // Move the collision component to the left of the current left tile.
                collision_component.Move(MATH::Vector2f(-1.0f * distance_to_move_for_current_til, 0.0f));

                // Track the distance we just moved.
                distance_left_to_move -= distance_to_move_for_current_til;

                // At this point, the loop will continue another iteration.
                // New left tiles should be detected for the new position of the collision component,
                // allowing movement to be simulated for the next set of left tiles.
            }
        }
    }

    void CollisionSystem::SimulateRightMovement(const Movement& movement, ICollisionComponent& collision_component) const
    {
        // VERIFY THAT THE MOVEMENT IS FOR THE 'RIGHT' DIRECTION.
        bool movement_for_right_direction = (CORE::Direction::RIGHT == movement.Direction);
        if (!movement_for_right_direction)
        {
            // An invalid movement was provided to this method since the direction was not right.
            return;
        }

        // GRADUALLY MOVE THE COLLISION COMPONENT RIGHTWARD UNTIL WE COLLIDE WITH SOMETHING.
        float distance_left_to_move = movement.DistanceInPixels;
        while (distance_left_to_move > 0.0f)
        {
            // GET THE CURRENT TILES THAT THE RIGHT OF THE COLLISION COMPONENT IS ON.
            MATH::FloatRectangle collision_box = collision_component.GetBoundingBox();
            float collision_box_right_x_position = collision_box.GetRightXPosition();

            // The vertical tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
            // We will adjust the corners to move them closer to the center of the collision box.
            float collision_box_height = collision_box.GetHeight();
            float vertical_corner_tiles_adjustment_amount = collision_box_height / 4.0f;

            // Get the tile for the top-right corner.
            float collision_box_top_y_position = collision_box.GetTopYPosition();
            collision_box_top_y_position += vertical_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> top_right_tile = OverworldMap->GetTileAtWorldPosition(collision_box_right_x_position, collision_box_top_y_position);

            // Get the tile to the direct right of the collision box's center.
            float collision_box_center_y_position = collision_box.GetCenterYPosition();
            std::shared_ptr<MAPS::Tile> centerRightTile = OverworldMap->GetTileAtWorldPosition(collision_box_right_x_position, collision_box_center_y_position);

            // Get the tile for the bottom-right corner.
            float collision_box_bottom_y_position = collision_box.GetBottomYPosition();
            collision_box_bottom_y_position -= vertical_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> bottom_right_tile = OverworldMap->GetTileAtWorldPosition(collision_box_right_x_position, collision_box_bottom_y_position);

            // MAKE SURE THE RIGHT TILES EXIST.
            bool right_tiles_exist = (nullptr != top_right_tile) && (nullptr != centerRightTile) && (nullptr != bottom_right_tile);
            if (!right_tiles_exist)
            {
                // There aren't any tiles right of the collision component to move to.
                return;
            }

            // CHECK IF THE RIGHT TILES ARE WALKABLE.
            bool right_tiles_walkable = (top_right_tile->IsWalkable()) && (centerRightTile->IsWalkable()) && (bottom_right_tile->IsWalkable());
            if (!right_tiles_walkable)
            {
                // At least one of the right tiles isn't walkable, so it is blocking any further rightward movement.
                return;
            }

            // CHECK IF THE CURRENT COLLISION BOX COLLIDES WITH A TREE.
            MATH::FloatRectangle tree_rectangle;
            bool collides_with_tree = CollidesWithTree(collision_box, tree_rectangle);
            if (collides_with_tree)
            {
                // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                // A collision could have occurred with a tree that isn't being moved toward,
                // in which case movement should not be stopped.
                // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
                const float MIN_RIGHT_MOVEMENT = 1.0f;
                float old_collision_box_left = collision_box.GetLeftXPosition();
                float old_collision_box_right = collision_box.GetRightXPosition();
                float new_collision_box_right = old_collision_box_right + MIN_RIGHT_MOVEMENT;
                float tree_left = tree_rectangle.GetLeftXPosition();

                // A minimum distance between the collision box's center and the tree's
                // center is enforced so that moving objects don't get stuck on
                // parts of trees not directly related to their current movement.
                float min_object_tree_distance = tree_rectangle.GetHeight() / 2.0f;
                float object_center_y = collision_box.GetCenterYPosition();
                float tree_center_y = tree_rectangle.GetCenterYPosition();
                float object_to_tree_distance = fabs(object_center_y - tree_center_y);
                bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

                bool moving_right_collides_with_tree = (
                    collision_distance_met &&
                    (old_collision_box_left <= tree_left) &&
                    (new_collision_box_right >= tree_left) );
                if (moving_right_collides_with_tree)
                {
                    // A tree is blocking further movement.
                    return;
                }
            }

            // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
            float right_tile_right_x_position = centerRightTile->GetRightXPosition();
            float distance_from_right_of_collision_box_to_right_of_tile = fabs(right_tile_right_x_position - collision_box_right_x_position);
            bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_right_of_collision_box_to_right_of_tile);
            if (remaining_movement_confined_to_current_tile)
            {
                // Move the collision component the total movement amount since it is confined
                // to a walkable tile.
                collision_component.Move(MATH::Vector2f(distance_left_to_move, 0.0f));

                // The entire movement has completed, so return.
                return;
            }
            else
            {
                // Ensure that we move right at least 1 pixel to prevent an infinite loop.
                float distance_to_move_for_current_tile = std::max(distance_from_right_of_collision_box_to_right_of_tile, 1.0f);

                // Move the collision component to the right of the current right tile.
                collision_component.Move(MATH::Vector2f(distance_from_right_of_collision_box_to_right_of_tile, 0.0f));

                // Track the distance we just moved.
                distance_left_to_move -= distance_to_move_for_current_tile;

                // At this point, the loop will continue another iteration.
                // New right tiles should be detected for the new position of the collision component,
                // allowing movement to be simulated for the next set of right tiles.
            }
        }
    }

    bool CollisionSystem::CollidesWithTree(const MATH::FloatRectangle& rectangle, MATH::FloatRectangle& tree_rectangle) const
    {
        // CLEAR THE OUT PARAMETER.
        tree_rectangle = MATH::FloatRectangle();

        // GET THE TREES NEAR THE RECTANGLE.
        std::vector< std::shared_ptr<OBJECTS::Tree> >* trees = OverworldMap->GetTreesInArea(rectangle.GetCenterPosition());
        bool trees_exist = (nullptr != trees);
        if (!trees_exist)
        {
            // No trees exist to collide with.
            return false;
        }

        // CHECK IF ANY OF THE TREES COLLIDE WITH THE RECTANGLE.
        for (auto tree = trees->cbegin(); trees->cend() != tree; ++tree)
        {
            // SHRINK THE TREE'S BOUNDING BOX SO THAT OBJECTS DON'T GET CAUGHT ON EDGES.
            const float TREE_DIMENSION_SHRINK_AMOUNT = 2.0f;
            MATH::FloatRectangle tree_bounds = (*tree)->GetBoundingBox();
            float new_tree_width = tree_bounds.GetWidth() - TREE_DIMENSION_SHRINK_AMOUNT;
            float new_tree_height = tree_bounds.GetHeight() - TREE_DIMENSION_SHRINK_AMOUNT;
            MATH::FloatRectangle new_tree_bounds = MATH::FloatRectangle::FromCenterAndDimensions(
                tree_bounds.GetCenterXPosition(),
                tree_bounds.GetCenterYPosition(),
                new_tree_width,
                new_tree_height);

            // CHECK IF A COLLISION OCCURS WITH THE CURRENT TREE.
            bool collides_with_tree = rectangle.Intersects(new_tree_bounds);
            if (collides_with_tree)
            {
                // RETURN THAT A COLLISION OCCURRED WITH THE CURRENT TREE.
                tree_rectangle = new_tree_bounds;
                return true;
            }
        }

        // No trees were found to collide with the rectangle.
        return false;
    }

    void CollisionSystem::HandleCollisions(std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> >& axe_swings)
    {
        // HANDLE COLLISIONS FOR ALL AXE SWINGS.
        for (auto axe_swing_event = axe_swings.cbegin(); axe_swings.cend() != axe_swing_event;)
        {
            // MAKE SURE THE CURRENT AXE SWING EXISTS.
            bool axe_swing_exists = (nullptr != axe_swing_event->get());
            if (!axe_swing_exists)
            {
                // Remove the invalid event so that it no longer takes up space.
                axe_swing_event = axe_swings.erase(axe_swing_event);

                // Silently skip over this missing axe swing to allow
                // the game to continue running.
                continue;
            }

            const EVENTS::AxeSwingEvent axe_swing = (**axe_swing_event);

            // CHECK IF THE AXE SWING EVENT HAS REACHED ITS MAXIMUM EXTENSION POINT.
            // The axe swing event should only be processed once it has finish
            // being fully swung out to its maximum point.
            bool axe_swing_at_max_extension_point = axe_swing.FullySwungOut();
            if (!axe_swing_at_max_extension_point)
            {
                // The current axe swing event is not ready to be processed,
                // so skip to the next event.
                ++axe_swing_event;
                continue;
            }

            // HANDLE COLLISIONS OF THE AXE WITH TREES.
            MATH::FloatRectangle axe_blade_bounds = axe_swing.GetBladeBounds();
            HandleAxeCollisionsWithTrees(axe_blade_bounds);

            // REMOVE THE PROCESSED AXE SWING EVENT.
            axe_swing_event = axe_swings.erase(axe_swing_event);
        }
    }
    
    void CollisionSystem::HandleAxeCollisionsWithTrees(const MATH::FloatRectangle& axe_blade_bounds)
    {
        // GET THE WORLD AREA CONTAING THE AXE BLADE.
        // While it is technically possible for the axe to intersect multiple world areas,
        // the axe is relatively small, and the player shouldn't be able to see more than
        // one world area.  Therefore, it should be safe to get just the area for the
        // axe blade's center.
        /// @todo   Maybe the world should just encapsulate the above?
        /// @todo   Change overworld map to world area?
        //WorldArea world_area = OverworldMap->GetArea(axe_blade_bounds.GetCenterXPosition(), axe_blade_bounds.GetCenterYPosition());
        std::vector< std::shared_ptr<OBJECTS::Tree> >* trees = OverworldMap->GetTreesInArea(axe_blade_bounds.GetCenterPosition());
        for (auto tree = trees->cbegin(); trees->cend() != tree;)
        {
            MATH::FloatRectangle tree_bounds = (*tree)->GetBoundingBox();
            bool axe_hit_tree = axe_blade_bounds.Intersects(tree_bounds);
            if (axe_hit_tree)
            {
                tree = trees->erase(tree);
            }
            else
            {
                ++tree;
            }
        }
    }
}
