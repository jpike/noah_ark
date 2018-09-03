#include <algorithm>
#include "Collision/CollisionDetectionAlgorithms.h"

namespace COLLISION
{
    /// Moves an object in a map grid while performing collision detection
    /// to prevent the object from inappropriately overlapping objects.
    /// @param[in]  object_world_bounding_box - The world bounding box of the object being moved.
    /// @param[in]  move_vector - The movement vector for the object (in pixels).
    /// @param[in]  tile_types_allowed_to_move_over - The types of tiles the object is allowed to move over.
    /// @param[in]  allow_movement_over_solid_objects - True to allow movement over solid objects in the world;
    ///     false to prevent such movement.
    /// @param[in,out]  tile_map_grid - The map grid in which the object is being moved.
    /// @return The new center world position of the object.
    MATH::Vector2f CollisionDetectionAlgorithms::MoveObject(
        const MATH::FloatRectangle& object_world_bounding_box,
        const MATH::Vector2f& move_vector,
        const std::unordered_set<MAPS::TileType::Id>& tile_types_allowed_to_move_over,
        const bool allow_movement_over_solid_objects,
        MAPS::MultiTileMapGrid& tile_map_grid)
    {
        // GRADUALLY MOVE THE OBJECT UNTIL WE COLLIDE WITH SOMETHING.
        // The bounding box of the object will be modified through multiple iterations of movement
        // to calculate the final position.
        MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
        MATH::Vector2f unit_move_vector = MATH::Vector2f::Normalize(move_vector);
        float distance_left_to_move = move_vector.Length();
        while (distance_left_to_move > 0.0f)
        {
            // CHECK IF THE MOVEMENT DISTANCE IS WITHIN A SINGLE TILE.
            // The incremental move distance (per loop iteration) is hardcoded to
            // the tile dimensions of the game, but this should be fine since
            // we don't expect to change the tile dimensions for this game.
            const float INCREMENTAL_MOVE_DISTANCE_IN_PIXELS = 16.0f;
            float move_distance_this_iteration = std::min(distance_left_to_move, INCREMENTAL_MOVE_DISTANCE_IN_PIXELS);

            // CALCULATE THE NEW POSITION OF THE OBJECT FOR THIS ITERATION.
            MATH::Vector2f object_original_center_world_position = object_current_bounding_box.GetCenterPosition();
            MATH::Vector2f remaining_move_vector = MATH::Vector2f::Scale(move_distance_this_iteration, unit_move_vector);
            MATH::Vector2f object_center_world_position = object_original_center_world_position + remaining_move_vector;
            MATH::FloatRectangle object_new_bounding_box = object_current_bounding_box;
            object_new_bounding_box.SetCenterPosition(object_center_world_position.X, object_center_world_position.Y);

            // GET THE TILE AT THE OBJECT'S POTENTIAL NEW POSITION.
            std::shared_ptr<MAPS::Tile> tile_at_new_object_position = tile_map_grid.GetTileAtWorldPosition(
                object_center_world_position.X, object_center_world_position.Y);
            if (!tile_at_new_object_position)
            {
                // RETURN THE OBJECT'S CURRENT UPDATED WORLD POSITION.
                // It can't move any further if there isn't a tile underneath it.
                MATH::Vector2f new_center_world_position = object_current_bounding_box.GetCenterPosition();
                return new_center_world_position;
            }

            // CHECK IF THE CURRENT TILE CAN BE MOVED OVER.
            bool tile_can_be_moved_over = tile_types_allowed_to_move_over.count(tile_at_new_object_position->Type) > 0;
            if (!tile_can_be_moved_over)
            {
                // RETURN THE OBJECT'S CURRENT UPDATED WORLD POSITION.
                // It can't move any further if the tile can't be moved over.
                MATH::Vector2f new_center_world_position = object_current_bounding_box.GetCenterPosition();
                return new_center_world_position;
            }

            // CHECK IF MOVEMENT IS ALLOWED OVER SOLID OBJECTS IN THE WORLD.
            if (!allow_movement_over_solid_objects)
            {
                // CHECK IF THE OBJECT COLLIDED WITH A TREE.
                MATH::FloatRectangle tree_rectangle;
                bool collides_with_tree = CollidesWithTree(object_current_bounding_box, tile_map_grid, tree_rectangle);
                if (collides_with_tree)
                {
                    // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                    // A collision could have occurred with a tree that isn't being moved toward,
                    // in which case movement should not be stopped.
                    // If the object is closer to the tree, then the object was moving toward the tree.
                    // Otherwise, the object was moving away and should be allowed to move.
                    MATH::Vector2f tree_center_world_position = tree_rectangle.GetCenterPosition();
                    MATH::Vector2f original_tree_to_object_vector = tree_center_world_position - object_original_center_world_position;
                    float original_tree_to_object_distance = original_tree_to_object_vector.Length();
                    MATH::Vector2f new_tree_to_object_vector = tree_center_world_position - object_center_world_position;
                    float new_tree_to_object_distance = new_tree_to_object_vector.Length();
                    bool object_moved_toward_tree = (new_tree_to_object_distance < original_tree_to_object_distance);
                    if (object_moved_toward_tree)
                    {
                        // DETERMINE IF THE OBJECT CAN MOVE SOME ALONG ONE AXIS.
                        // The object may have primarily collided with one side of the tree.
                        // If not moving completely horizontally or vertically, the object may be able
                        // to move a little bit alongside the tree.  Doing this kind of check here
                        // helps prevent objects from getting stuck.
                        float tree_top_y_world_position = tree_rectangle.GetTopYPosition();
                        float tree_bottom_y_world_position = tree_rectangle.GetBottomYPosition();
                        float tree_left_x_world_position = tree_rectangle.GetLeftXPosition();
                        float tree_right_x_world_position = tree_rectangle.GetRightXPosition();
                        float object_new_top_y_world_position = object_new_bounding_box.GetTopYPosition();
                        float object_new_bottom_y_world_position = object_new_bounding_box.GetBottomYPosition();
                        float object_new_left_x_world_position = object_new_bounding_box.GetLeftXPosition();
                        float object_new_right_x_world_position = object_new_bounding_box.GetRightXPosition();
                        // 5 cases for the object colliding with the left or right side of the tree:
                        // - Object center y completely within tree bounds.
                        // - Object top y is above tree, object bottom y is below top of tree, and object center right of tree.
                        // - Object top y is above tree, object bottom y is below top of tree, and object center left of tree.
                        // - Object bottom y is below tree, object top y is above bottom of tree, and object center right of tree.
                        // - Object bottom y is below tree, object top y is above bottom of tree, and object center left of tree.
                        bool object_center_y_within_tree_bounds = (
                            (tree_top_y_world_position < object_center_world_position.Y) &&
                            (object_center_world_position.Y < tree_bottom_y_world_position));
                        bool object_bottom_hit_tree_from_right = (
                            (object_new_top_y_world_position < tree_top_y_world_position) &&
                            (object_new_bottom_y_world_position > tree_top_y_world_position) &&
                            (object_center_world_position.X > tree_right_x_world_position));
                        bool object_bottom_hit_tree_from_left = (
                            (object_new_top_y_world_position < tree_top_y_world_position) &&
                            (object_new_bottom_y_world_position > tree_top_y_world_position) &&
                            (object_center_world_position.X < tree_left_x_world_position));
                        bool object_top_hit_tree_from_right = (
                            (object_new_bottom_y_world_position > tree_bottom_y_world_position) &&
                            (object_new_top_y_world_position > tree_bottom_y_world_position) &&
                            (object_center_world_position.X > tree_right_x_world_position));
                        bool object_top_hit_tree_from_left = (
                            (object_new_bottom_y_world_position > tree_bottom_y_world_position) &&
                            (object_new_top_y_world_position > tree_bottom_y_world_position) &&
                            (object_center_world_position.X < tree_left_x_world_position));
                        bool object_collided_horizontally_with_tree = (
                            object_center_y_within_tree_bounds ||
                            object_bottom_hit_tree_from_right ||
                            object_bottom_hit_tree_from_left ||
                            object_top_hit_tree_from_right ||
                            object_top_hit_tree_from_left);
                        // 5 cases for the object colliding with the top or bottom side of the tree
                        // - Object center x is completely within tree bounds.
                        // - Object left x is left of tree, object right x is right of left of tree, and object center above tree.
                        // - Object left x is left of tree, object right x is right of left of tree, and object center below tree.
                        // - Object right x is right of tree, object left x is left of right of tree, and object center above tree.
                        // - Object right x is right of tree, object left x is left of right of tree, and object center below tree.
                        bool object_center_x_within_tree_bounds = (
                            (tree_left_x_world_position < object_center_world_position.X) &&
                            (object_center_world_position.X < tree_right_x_world_position));
                        bool object_right_hit_tree_from_top = (
                            (object_new_left_x_world_position < tree_left_x_world_position) &&
                            (object_new_right_x_world_position > tree_left_x_world_position) &&
                            (object_center_world_position.Y < tree_top_y_world_position));
                        bool object_right_hit_tree_from_bottom = (
                            (object_new_left_x_world_position < tree_left_x_world_position) &&
                            (object_new_right_x_world_position > tree_left_x_world_position) &&
                            (object_center_world_position.Y > tree_bottom_y_world_position));
                        bool object_left_hit_tree_from_top = (
                            (object_new_right_x_world_position > tree_right_x_world_position) &&
                            (object_new_left_x_world_position < tree_right_x_world_position) &&
                            (object_center_world_position.Y < tree_top_y_world_position));
                        bool object_left_hit_tree_from_bottom = (
                            (object_new_right_x_world_position > tree_right_x_world_position) &&
                            (object_new_left_x_world_position < tree_right_x_world_position) &&
                            (object_center_world_position.Y > tree_bottom_y_world_position));
                        bool object_collided_vertically_with_tree = (
                            object_center_x_within_tree_bounds ||
                            object_right_hit_tree_from_top ||
                            object_right_hit_tree_from_bottom ||
                            object_left_hit_tree_from_top ||
                            object_left_hit_tree_from_bottom);

                        // PERFORM ANY PARTIAL MOVEMENT BASED ON THE COLLISION.
                        // If there's both a horizontal and vertical collision, one is arbitrarily chosen
                        // first to keep the object moving.
                        if (object_collided_horizontally_with_tree)
                        {
                            // ONLY MOVE THE OBJECT VERTICALLY.
                            MATH::Vector2f new_center_world_position = object_current_bounding_box.GetCenterPosition();
                            new_center_world_position.Y = object_center_world_position.Y;
                            return new_center_world_position;
                        }
                        else if (object_collided_vertically_with_tree)
                        {
                            // ONLY MOVE THE OBJECT HORIZONTALLY.
                            MATH::Vector2f new_center_world_position = object_current_bounding_box.GetCenterPosition();
                            new_center_world_position.X = object_center_world_position.X;
                            return new_center_world_position;
                        }
                        else
                        {
                            // RETURN THE OBJECT'S CURRENT UPDATED WORLD POSITION.
                            // Assume the tree is completely blocking movement.
                            // It can't move any further if there a tree is in the way.
                            MATH::Vector2f new_center_world_position = object_current_bounding_box.GetCenterPosition();
                            return new_center_world_position;
                        }
                    }
                }
            }

            // UPDATE THE OBJECT'S BOUNDING BOX FOR THE MOVEMENT THIS ITERATION.
            object_current_bounding_box = object_new_bounding_box;

            // ACCOUNT FOR THE MOVEMENT THIS ITERATION.
            distance_left_to_move -= move_distance_this_iteration;
        }

        // RETURN THE OBJECT'S FINAL NEW WORLD POSITION.
        MATH::Vector2f final_center_world_position = object_current_bounding_box.GetCenterPosition();
        return final_center_world_position;
    }

    /// Moves an object in the map grid while performing collision detection
    /// to prevent the object from inappropriately overlapping objects.
    /// @param[in]  object_world_bounding_box - The world bounding box of the object being moved.
    /// @param[in]  direction - The direction to move the object.
    /// @param[in]  move_speed_in_pixels_per_second - The movement speed of the object.
    /// @param[in]  elapsed_time - The elapsed time over which to move the object.
    /// @param[in,out]  tile_map_grid - The map grid in which the object is being moved.
    /// @return The new center world position of the object.
    MATH::Vector2f CollisionDetectionAlgorithms::MoveObject(
        const MATH::FloatRectangle& object_world_bounding_box,
        const CORE::Direction direction,
        const float move_speed_in_pixels_per_second,
        const sf::Time& elapsed_time,
        MAPS::MultiTileMapGrid& tile_map_grid)
    {
        // DESCRIBE THE MOVEMENT TO BE PERFORMED.
        float elapsed_time_in_seconds = elapsed_time.asSeconds();
        float movement_distance_in_pixels = move_speed_in_pixels_per_second * elapsed_time_in_seconds;
        COLLISION::Movement movement(direction, movement_distance_in_pixels);

        // SIMULATE MOVEMENT BASED ON THE PARTICULAR DIRECTION.
        MATH::Vector2f new_world_position = object_world_bounding_box.GetCenterPosition();
        switch (direction)
        {
            case CORE::Direction::UP:
                new_world_position = MoveObjectUp(object_world_bounding_box, movement, tile_map_grid);
                break;
            case CORE::Direction::DOWN:
                new_world_position = MoveObjectDown(object_world_bounding_box, movement, tile_map_grid);
                break;
            case CORE::Direction::LEFT:
                new_world_position = MoveObjectLeft(object_world_bounding_box, movement, tile_map_grid);
                break;
            case CORE::Direction::RIGHT:
                new_world_position = MoveObjectRight(object_world_bounding_box, movement, tile_map_grid);
                break;
            case CORE::Direction::INVALID:
                // Intentionally fall through.
            default:
                // No movement can be simulated for an invalid direction.
                break;
        }

        return new_world_position;
    }

    /// Handles collisions of axe swings with objects in the map grid.
    /// @param[in,out]  tile_map_grid - The map grid in which axes are being swung.
    /// @param[in,out]  axe_swings - The axe swings to process and update.
    /// @param[in,out]  speakers - The speakers for which to play sound effects.
    /// @param[in,out]  assets - Assets that might be needed.
    void CollisionDetectionAlgorithms::HandleAxeSwings(
        MAPS::MultiTileMapGrid& tile_map_grid,
        std::vector< std::shared_ptr<EVENTS::AxeSwingEvent> >& axe_swings,
        AUDIO::Speakers& speakers,
        RESOURCES::Assets& assets)
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
            HandleAxeCollisionsWithTrees(*axe_swing.Axe, tile_map_grid, speakers, assets);

            // REMOVE THE PROCESSED AXE SWING EVENT.
            axe_swing_event = axe_swings.erase(axe_swing_event);
        }
    }

    /// Moves an object up in the map grid while performing collision detection
    /// to prevent the object from inappropriately overlapping objects.
    /// @param[in]  object_world_bounding_box - The world bounding box of the object being moved.
    /// @param[in]  movement - The movement to simulate for the object.
    /// @param[in,out]  tile_map_grid - The map grid in which the object is being moved.
    /// @return The new center world position of the object.
    MATH::Vector2f CollisionDetectionAlgorithms::MoveObjectUp(
        const MATH::FloatRectangle& object_world_bounding_box,
        const COLLISION::Movement& movement,
        MAPS::MultiTileMapGrid& tile_map_grid)
    {
        // INITIALIZE THE NEW POSITION FOR THE OBJECT.
        // The object should remain at its current position if no movement occurs.
        MATH::Vector2f object_new_world_position = object_world_bounding_box.GetCenterPosition();

        // VERIFY THAT THE MOVEMENT IS FOR THE 'UP' DIRECTION.
        bool movement_for_up_direction = (CORE::Direction::UP == movement.Direction);
        if (!movement_for_up_direction)
        {
            // An invalid movement was provided to this method since the direction was not up.
            return object_new_world_position;
        }

        // GRADUALLY MOVE THE OBJECT UPWARD UNTIL WE COLLIDE WITH SOMETHING.
        // The bounding box of the object will be modified through multiple iterations of movement
        // to calculate the final position.
        MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
        float distance_left_to_move = movement.DistanceInPixels;
        while (distance_left_to_move > 0.0f)
        {
            // GET THE CURRENT TILES THAT THE TOP OF THE OBJECT'S BOUNDING BOX IS TOUCHING.
            float collision_box_top_y_position = object_current_bounding_box.GetTopYPosition();

            // Get the tile for the top-left corner.
            float collision_box_left_x_position = object_current_bounding_box.GetLeftXPosition();
            // The horizonal tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
            float collision_box_width = object_current_bounding_box.GetWidth();
            float horizontal_corner_tiles_adjustment_amount = collision_box_width / 4.0f;
            collision_box_left_x_position += horizontal_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> top_left_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_top_y_position);

            // Get the tile right above the collision box's center.
            float collison_box_center_x_position = object_current_bounding_box.GetCenterXPosition();
            std::shared_ptr<MAPS::Tile> top_center_tile = tile_map_grid.GetTileAtWorldPosition(collison_box_center_x_position, collision_box_top_y_position);

            // Get the tile for the top-right corner.
            float collision_box_right_x_position = object_current_bounding_box.GetRightXPosition();
            collision_box_right_x_position -= horizontal_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> top_right_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_top_y_position);

            // MAKE SURE THE TOP TILES EXIST.
            bool top_tiles_exist = (nullptr != top_left_tile) && (nullptr != top_center_tile) && (nullptr != top_right_tile);
            if (!top_tiles_exist)
            {
                // There aren't any tiles above the collision box to move to.
                break;
            }

            // CHECK IF THE TOP TILES ARE WALKABLE.
            bool top_tiles_walkable = (top_left_tile->IsWalkable()) && (top_center_tile->IsWalkable()) && (top_right_tile->IsWalkable());
            if (!top_tiles_walkable)
            {
                // At least one of the top tiles isn't walkable, so it is blocking any further upward movement.
                break;
            }

            // CHECK IF THE OBJECT COLLIDES WITH A TREE.
            MATH::FloatRectangle tree_rectangle;
            bool collides_with_tree = CollidesWithTree(object_current_bounding_box, tile_map_grid, tree_rectangle);
            if (collides_with_tree)
            {
                // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                // A collision could have occurred with a tree that isn't being moved toward,
                // in which case movement should not be stopped.
                // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
                const float MIN_UP_MOVEMENT = -1.0f;
                float old_collision_box_bottom = object_current_bounding_box.GetBottomYPosition();
                float old_collision_box_top = object_current_bounding_box.GetTopYPosition();
                float new_collision_box_top = old_collision_box_top + MIN_UP_MOVEMENT;
                float tree_bottom = tree_rectangle.GetBottomYPosition();

                // A minimum distance between the collision box's center and the tree's
                // center is enforced so that moving objects don't get stuck on
                // parts of trees not directly related to their current movement.
                float min_object_tree_distance = tree_rectangle.GetWidth() / 2.0f;
                float object_center_x = object_current_bounding_box.GetCenterXPosition();
                float tree_center_x = tree_rectangle.GetCenterXPosition();
                float object_to_tree_distance = fabs(object_center_x - tree_center_x);
                bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

                bool moving_up_collides_with_tree = (
                    collision_distance_met &&
                    (old_collision_box_bottom >= tree_bottom) &&
                    (new_collision_box_top <= tree_bottom));
                if (moving_up_collides_with_tree)
                {
                    // A tree is blocking further movement.
                    break;
                }
            }

            // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
            float top_tile_top_y_position = top_center_tile->GetTopYPosition();
            float distance_from_top_of_collision_box_to_top_of_tile = fabs(collision_box_top_y_position - top_tile_top_y_position);
            bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_top_of_collision_box_to_top_of_tile);
            if (remaining_movement_confined_to_current_tile)
            {
                // Move the collision box the total movement amount since it is confined to a walkable tile.
                // Upward movement is in the negative Y direction.
                object_current_bounding_box.Move(MATH::Vector2f(0.0f, -1.0f * distance_left_to_move));

                // The entire movement has completed, so exit the movement loop.
                break;
            }
            else
            {
                // Ensure that we move up at least 1 pixel to prevent an infinite loop.
                float distance_to_move_for_current_tile = std::max(distance_from_top_of_collision_box_to_top_of_tile, 1.0f);

                // Move the collision box to the top of the current top tile.
                // Upward movement is in the negative Y direction.
                object_current_bounding_box.Move(MATH::Vector2f(0.0f, -1.0f * distance_to_move_for_current_tile));

                // Track the distance we just moved.
                distance_left_to_move -= distance_to_move_for_current_tile;

                // At this point, the loop will continue another iteration.
                // New top tiles should be detected for the new position of the collision box,
                // allowing movement to be simulated for the next set of top tiles.
            }
        }

        // RETURN THE OBJECT'S NEW POSITION.
        object_new_world_position = object_current_bounding_box.GetCenterPosition();
        return object_new_world_position;
    }

    /// Moves an object down in the map grid while performing collision detection
    /// to prevent the object from inappropriately overlapping objects.
    /// @param[in]  object_world_bounding_box - The world bounding box of the object being moved.
    /// @param[in]  movement - The movement to simulate for the object.
    /// @param[in,out]  tile_map_grid - The map grid in which the object is being moved.
    /// @return The new center world position of the object.
    MATH::Vector2f CollisionDetectionAlgorithms::MoveObjectDown(
        const MATH::FloatRectangle& object_world_bounding_box,
        const COLLISION::Movement& movement,
        MAPS::MultiTileMapGrid& tile_map_grid)
    {
        // INITIALIZE THE NEW POSITION OF THE OBJECT.
        // The object should remain at its current position if no movement occurs.
        MATH::Vector2f object_new_world_position = object_world_bounding_box.GetCenterPosition();

        // VERIFY THAT THE MOVEMENT IS FOR THE 'DOWN' DIRECTION.
        bool movement_for_down_direction = (CORE::Direction::DOWN == movement.Direction);
        if (!movement_for_down_direction)
        {
            // An invalid movement was provided to this method since the direction was not down.
            return object_new_world_position;
        }

        // GRADUALLY MOVE THE OBJECT DOWNWARD UNTIL WE COLLIDE WITH SOMETHING.
        // The bounding box of the object will be modified through multiple iterations of movement
        // to calculate the final position.
        MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
        float distance_left_to_move = movement.DistanceInPixels;
        while (distance_left_to_move > 0.0f)
        {
            // GET THE CURRENT TILES THAT THE BOTTOM OF THE OBJECT'S BOUNDING BOX IS TOUCHING.
            float collision_box_bottom_y_position = object_current_bounding_box.GetBottomYPosition();

            // The horizonal tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
            // We will adjust the corners to move them closer to the center of the collision box.
            float collision_box_width = object_current_bounding_box.GetWidth();
            float horizontal_corner_tiles_adjustment_amount = collision_box_width / 4.0f;

            // Get the tile for the bottom-left corner.
            float collision_box_left_x_position = object_current_bounding_box.GetLeftXPosition();
            collision_box_left_x_position += horizontal_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> bottom_left_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_bottom_y_position);

            // Get the tile right below the collision box's center.
            float collison_box_center_x_position = object_current_bounding_box.GetCenterXPosition();
            std::shared_ptr<MAPS::Tile> bottom_center_tile = tile_map_grid.GetTileAtWorldPosition(collison_box_center_x_position, collision_box_bottom_y_position);

            // Get the tile for the bottom-right corner.
            float collision_box_right_x_position = object_current_bounding_box.GetRightXPosition();
            collision_box_right_x_position -= horizontal_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> bottom_right_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_bottom_y_position);

            // MAKE SURE THE BOTTOM TILES EXIST.
            bool bottom_tiles_exist = (nullptr != bottom_left_tile) && (nullptr != bottom_center_tile) && (nullptr != bottom_right_tile);
            if (!bottom_tiles_exist)
            {
                // There aren't any tiles below the collision box to move to.
                break;
            }

            // CHECK IF THE BOTTOM TILES ARE WALKABLE.
            bool bottom_tiles_walkable = (bottom_left_tile->IsWalkable()) && (bottom_center_tile->IsWalkable()) && (bottom_right_tile->IsWalkable());
            if (!bottom_tiles_walkable)
            {
                // At least one of the bottom tiles isn't walkable, so it is blocking any further downward movement.
                break;
            }

            // CHECK IF THE OBJECT COLLIDES WITH A TREE.
            MATH::FloatRectangle tree_rectangle;
            bool collides_with_tree = CollidesWithTree(object_current_bounding_box, tile_map_grid, tree_rectangle);
            if (collides_with_tree)
            {
                // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                // A collision could have occurred with a tree that isn't being moved toward,
                // in which case movement should not be stopped.
                // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
                const float MIN_DOWN_MOVEMENT = 1.0f;
                float old_collision_box_top = object_current_bounding_box.GetTopYPosition();
                float old_collision_box_bottom = object_current_bounding_box.GetBottomYPosition();
                float new_collision_box_bottom = old_collision_box_bottom + MIN_DOWN_MOVEMENT;
                float tree_top = tree_rectangle.GetTopYPosition();

                // A minimum distance between the collision box's center and the tree's
                // center is enforced so that moving objects don't get stuck on
                // parts of trees not directly related to their current movement.
                float min_object_tree_distance = tree_rectangle.GetWidth() / 2.0f;
                float object_center_x = object_current_bounding_box.GetCenterXPosition();
                float tree_center_x = tree_rectangle.GetCenterXPosition();
                float object_to_tree_distance = fabs(object_center_x - tree_center_x);
                bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

                bool moving_down_collides_with_tree = (
                    collision_distance_met &&
                    (old_collision_box_top <= tree_top) &&
                    (new_collision_box_bottom >= tree_top));
                if (moving_down_collides_with_tree)
                {
                    // A tree is blocking further movement.
                    break;
                }
            }

            // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
            float bottom_tile_bottom_y_position = bottom_center_tile->GetBottomYPosition();
            float distance_from_bottom_of_collision_box_to_bottom_of_tile = fabs(bottom_tile_bottom_y_position - collision_box_bottom_y_position);
            bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_bottom_of_collision_box_to_bottom_of_tile);
            if (remaining_movement_confined_to_current_tile)
            {
                // Move the collision box the total movement amount since it is confined to a walkable tile.
                // Downward movement is in the positive Y direction.
                object_current_bounding_box.Move(MATH::Vector2f(0.0f, distance_left_to_move));

                // The entire movement has completed, so exit the movement loop.
                break;
            }
            else
            {
                // Ensure that we move down at least 1 pixel to prevent an infinite loop.
                float distance_to_move_for_current_tile = std::max(distance_from_bottom_of_collision_box_to_bottom_of_tile, 1.0f);

                // Move the collision box to the bottom of the current bottom tile.
                // Downward movement is in the positive Y direction.
                object_current_bounding_box.Move(MATH::Vector2f(0.0f, distance_to_move_for_current_tile));

                // Track the distance we just moved.
                distance_left_to_move -= distance_to_move_for_current_tile;

                // At this point, the loop will continue another iteration.
                // New bottom tiles should be detected for the new position of the collision box,
                // allowing movement to be simulated for the next set of bottom tiles.
            }
        }

        // RETURN THE OBJECT'S NEW POSITION.
        object_new_world_position = object_current_bounding_box.GetCenterPosition();
        return object_new_world_position;
    }

    /// Moves an object left in the map grid while performing collision detection
    /// to prevent the object from inappropriately overlapping objects.
    /// @param[in]  object_world_bounding_box - The world bounding box of the object being moved.
    /// @param[in]  movement - The movement to simulate for the object.
    /// @param[in,out]  tile_map_grid - The map grid in which the object is being moved.
    /// @return The new center world position of the object.
    MATH::Vector2f CollisionDetectionAlgorithms::MoveObjectLeft(
        const MATH::FloatRectangle& object_world_bounding_box,
        const COLLISION::Movement& movement,
        MAPS::MultiTileMapGrid& tile_map_grid)
    {
        // INITIALIZE THE NEW POSITION FOR THE OBJECT.
        // The object should remain at its current position if no movement occurs.
        MATH::Vector2f object_new_world_position = object_world_bounding_box.GetCenterPosition();

        // VERIFY THAT THE MOVEMENT IS FOR THE 'LEFT' DIRECTION.
        bool movement_for_left_direction = (CORE::Direction::LEFT == movement.Direction);
        if (!movement_for_left_direction)
        {
            // An invalid movement was provided to this method since the direction was not left.
            return object_new_world_position;
        }

        // GRADUALLY MOVE THE OBJECT LEFTWARD UNTIL WE COLLIDE WITH SOMETHING.
        // The bounding box of the object will be modified through multiple iterations of movement
        // to calculate the final position.
        MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
        float distance_left_to_move = movement.DistanceInPixels;
        while (distance_left_to_move > 0.0f)
        {
            // GET THE CURRENT TILES THAT THE LEFT OF THE OBJECT'S BOUNDING BOX IS TOUCHING.
            float collision_box_left_x_position = object_current_bounding_box.GetLeftXPosition();

            // The vertical tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
            // We will adjust the corners to move them closer to the center of the collision box.
            float collision_box_height = object_current_bounding_box.GetHeight();
            float vertical_corner_tiles_adjustment_amount = collision_box_height / 4.0f;

            // Get the tile for the top-left corner.
            float collision_box_top_y_position = object_current_bounding_box.GetTopYPosition();
            collision_box_top_y_position += vertical_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> top_left_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_top_y_position);

            // Get the tile to the direct left of the collision box's center.
            float collision_box_center_y_position = object_current_bounding_box.GetCenterYPosition();
            std::shared_ptr<MAPS::Tile> center_left_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_center_y_position);

            // Get the tile for the bottom-left corner.
            float collision_box_bottom_y_position = object_current_bounding_box.GetBottomYPosition();
            collision_box_bottom_y_position -= vertical_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> bottom_left_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_left_x_position, collision_box_bottom_y_position);

            // MAKE SURE THE LEFT TILES EXIST.
            bool left_tiles_exist = (nullptr != top_left_tile) && (nullptr != center_left_tile) && (nullptr != bottom_left_tile);
            if (!left_tiles_exist)
            {
                // There aren't any tiles left of the collision box to move to.
                break;
            }

            // CHECK IF THE LEFT TILES ARE WALKABLE.
            bool left_tiles_walkable = (top_left_tile->IsWalkable()) && (center_left_tile->IsWalkable()) && (bottom_left_tile->IsWalkable());
            if (!left_tiles_walkable)
            {
                // At least one of the left tiles isn't walkable, so it is blocking any further leftward movement.
                break;
            }

            // CHECK IF THE OBJECT COLLIDES WITH A TREE.
            MATH::FloatRectangle tree_rectangle;
            bool collides_with_tree = CollidesWithTree(object_current_bounding_box, tile_map_grid, tree_rectangle);
            if (collides_with_tree)
            {
                // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                // A collision could have occurred with a tree that isn't being moved toward,
                // in which case movement should not be stopped.
                // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
                const float MIN_LEFT_MOVEMENT = -1.0f;
                float old_collision_box_right = object_current_bounding_box.GetRightXPosition();
                float old_collision_box_left = object_current_bounding_box.GetLeftXPosition();
                float new_collision_box_left = old_collision_box_left + MIN_LEFT_MOVEMENT;
                float tree_right = tree_rectangle.GetRightXPosition();

                // A minimum distance between the collision box's center and the tree's
                // center is enforced so that moving objects don't get stuck on
                // parts of trees not directly related to their current movement.
                float min_object_tree_distance = tree_rectangle.GetHeight() / 2.0f;
                float object_center_y = object_current_bounding_box.GetCenterYPosition();
                float tree_center_y = tree_rectangle.GetCenterYPosition();
                float object_to_tree_distance = fabs(object_center_y - tree_center_y);
                bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

                bool moving_left_collides_with_tree = (
                    collision_distance_met &&
                    (old_collision_box_right >= tree_right) &&
                    (new_collision_box_left <= tree_right));
                if (moving_left_collides_with_tree)
                {
                    // A tree is blocking further movement.
                    break;
                }
            }

            // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
            float left_tile_left_x_position = center_left_tile->GetLeftXPosition();
            float distance_from_left_of_collision_box_to_left_of_tile = fabs(collision_box_left_x_position - left_tile_left_x_position);
            bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_left_of_collision_box_to_left_of_tile);
            if (remaining_movement_confined_to_current_tile)
            {
                // Move the collision box the total movement amount since it is confined to a walkable tile.
                object_current_bounding_box.Move(MATH::Vector2f(-1.0f * distance_left_to_move, 0.0f));

                // The entire movement has completed, so exit the movement lop.
                break;
            }
            else
            {
                // Ensure that we move left at least 1 pixel to prevent an infinite loop.
                float distance_to_move_for_current_tile = std::max(distance_from_left_of_collision_box_to_left_of_tile, 1.0f);

                // Move the collision box to the left of the current left tile.
                object_current_bounding_box.Move(MATH::Vector2f(-1.0f * distance_to_move_for_current_tile, 0.0f));

                // Track the distance we just moved.
                distance_left_to_move -= distance_to_move_for_current_tile;

                // At this point, the loop will continue another iteration.
                // New left tiles should be detected for the new position of the collision box,
                // allowing movement to be simulated for the next set of left tiles.
            }
        }

        // RETURN THE OBJECT'S NEW POSITION.
        object_new_world_position = object_current_bounding_box.GetCenterPosition();
        return object_new_world_position;
    }

    /// Moves an object right in the map grid while performing collision detection
    /// to prevent the object from inappropriately overlapping objects.
    /// @param[in]  object_world_bounding_box - The world bounding box of the object being moved.
    /// @param[in]  movement - The movement to simulate for the object.
    /// @param[in,out]  tile_map_grid - The map grid in which the object is being moved.
    /// @return The new center world position of the object.
    MATH::Vector2f CollisionDetectionAlgorithms::MoveObjectRight(
        const MATH::FloatRectangle& object_world_bounding_box,
        const COLLISION::Movement& movement,
        MAPS::MultiTileMapGrid& tile_map_grid)
    {
        // INITIALIZE THE NEW POSITION FOR THE OBJECT.
        // The object should remain at its current position if no movement occurs.
        MATH::Vector2f object_new_world_position = object_world_bounding_box.GetCenterPosition();

        // VERIFY THAT THE MOVEMENT IS FOR THE 'RIGHT' DIRECTION.
        bool movement_for_right_direction = (CORE::Direction::RIGHT == movement.Direction);
        if (!movement_for_right_direction)
        {
            // An invalid movement was provided to this method since the direction was not right.
            return object_new_world_position;
        }

        // GRADUALLY MOVE THE OBJECT RIGHTWARD UNTIL WE COLLIDE WITH SOMETHING.
        // The bounding box of the object will be modified through multiple iterations of movement
        // to calculate the final position.
        MATH::FloatRectangle object_current_bounding_box = object_world_bounding_box;
        float distance_left_to_move = movement.DistanceInPixels;
        while (distance_left_to_move > 0.0f)
        {
            // GET THE CURRENT TILES THAT THE RIGHT OF THE OBJECT'S BOUNDING BOX IS TOUCHING.
            float collision_box_right_x_position = object_current_bounding_box.GetRightXPosition();

            // The vertical tiles retrieved will be adjusted so that small corners touching won't be detected as collisions.
            // We will adjust the corners to move them closer to the center of the collision box.
            float collision_box_height = object_current_bounding_box.GetHeight();
            float vertical_corner_tiles_adjustment_amount = collision_box_height / 4.0f;

            // Get the tile for the top-right corner.
            float collision_box_top_y_position = object_current_bounding_box.GetTopYPosition();
            collision_box_top_y_position += vertical_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> top_right_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_top_y_position);

            // Get the tile to the direct right of the collision box's center.
            float collision_box_center_y_position = object_current_bounding_box.GetCenterYPosition();
            std::shared_ptr<MAPS::Tile> center_right_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_center_y_position);

            // Get the tile for the bottom-right corner.
            float collision_box_bottom_y_position = object_current_bounding_box.GetBottomYPosition();
            collision_box_bottom_y_position -= vertical_corner_tiles_adjustment_amount;
            std::shared_ptr<MAPS::Tile> bottom_right_tile = tile_map_grid.GetTileAtWorldPosition(collision_box_right_x_position, collision_box_bottom_y_position);

            // MAKE SURE THE RIGHT TILES EXIST.
            bool right_tiles_exist = (nullptr != top_right_tile) && (nullptr != center_right_tile) && (nullptr != bottom_right_tile);
            if (!right_tiles_exist)
            {
                // There aren't any tiles right of the collision box to move to.
                break;
            }

            // CHECK IF THE RIGHT TILES ARE WALKABLE.
            bool right_tiles_walkable = (top_right_tile->IsWalkable()) && (center_right_tile->IsWalkable()) && (bottom_right_tile->IsWalkable());
            if (!right_tiles_walkable)
            {
                // At least one of the right tiles isn't walkable, so it is blocking any further rightward movement.
                break;
            }

            // CHECK IF THE OBJECT COLLIDES WITH A TREE.
            MATH::FloatRectangle tree_rectangle;
            bool collides_with_tree = CollidesWithTree(object_current_bounding_box, tile_map_grid, tree_rectangle);
            if (collides_with_tree)
            {
                // CHECK IF THE TREE IS IN THE PATH OF MOVEMENT.
                // A collision could have occurred with a tree that isn't being moved toward,
                // in which case movement should not be stopped.
                // Since pixels correspond to world coordinates, movement must occur by at least 1 pixel.
                const float MIN_RIGHT_MOVEMENT = 1.0f;
                float old_collision_box_left = object_current_bounding_box.GetLeftXPosition();
                float old_collision_box_right = object_current_bounding_box.GetRightXPosition();
                float new_collision_box_right = old_collision_box_right + MIN_RIGHT_MOVEMENT;
                float tree_left = tree_rectangle.GetLeftXPosition();

                // A minimum distance between the collision box's center and the tree's
                // center is enforced so that moving objects don't get stuck on
                // parts of trees not directly related to their current movement.
                float min_object_tree_distance = tree_rectangle.GetHeight() / 2.0f;
                float object_center_y = object_current_bounding_box.GetCenterYPosition();
                float tree_center_y = tree_rectangle.GetCenterYPosition();
                float object_to_tree_distance = fabs(object_center_y - tree_center_y);
                bool collision_distance_met = (object_to_tree_distance <= min_object_tree_distance);

                bool moving_right_collides_with_tree = (
                    collision_distance_met &&
                    (old_collision_box_left <= tree_left) &&
                    (new_collision_box_right >= tree_left));
                if (moving_right_collides_with_tree)
                {
                    // A tree is blocking further movement.
                    break;
                }
            }

            // CHECK IF THE TOTAL MOVEMENT IS CONFINED TO THE CURRENT TILE.
            float right_tile_right_x_position = center_right_tile->GetRightXPosition();
            float distance_from_right_of_collision_box_to_right_of_tile = fabs(right_tile_right_x_position - collision_box_right_x_position);
            bool remaining_movement_confined_to_current_tile = (distance_left_to_move <= distance_from_right_of_collision_box_to_right_of_tile);
            if (remaining_movement_confined_to_current_tile)
            {
                // Move the collision box the total movement amount since it is confined
                // to a walkable tile.
                object_current_bounding_box.Move(MATH::Vector2f(distance_left_to_move, 0.0f));

                // The entire movement has completed, so exit the movement loop.
                break;
            }
            else
            {
                // Ensure that we move right at least 1 pixel to prevent an infinite loop.
                float distance_to_move_for_current_tile = std::max(distance_from_right_of_collision_box_to_right_of_tile, 1.0f);

                // Move the collision box to the right of the current right tile.
                object_current_bounding_box.Move(MATH::Vector2f(distance_from_right_of_collision_box_to_right_of_tile, 0.0f));

                // Track the distance we just moved.
                distance_left_to_move -= distance_to_move_for_current_tile;

                // At this point, the loop will continue another iteration.
                // New right tiles should be detected for the new position of the collision box,
                // allowing movement to be simulated for the next set of right tiles.
            }
        }

        // RETURN THE OBJECT'S NEW POSITION.
        object_new_world_position = object_current_bounding_box.GetCenterPosition();
        return object_new_world_position;
    }

    /// Handles collisions of axes with trees in the map grid.
    /// @param[in]  axe - The axe to process for collision detection with trees.
    /// @param[in,out]  tile_map_grid - The map grid in which the axe and trees exist.
    /// @param[in,out]  speakers - The speakers for which to play sound effects.
    /// @param[in,out]  assets - Assets that might be needed.
    void CollisionDetectionAlgorithms::HandleAxeCollisionsWithTrees(
        const OBJECTS::Axe& axe, 
        MAPS::MultiTileMapGrid& tile_map_grid,
        AUDIO::Speakers& speakers,
        RESOURCES::Assets& assets)
    {
        // GET THE WORLD AREA CONTAING THE AXE BLADE.
        // While it is technically possible for the axe to intersect multiple tile maps,
        // the axe is relatively small, and the player shouldn't be able to see more than
        // one tile map.  Therefore, it should be safe to get just the tile map for the
        // axe blade's center.
        MATH::FloatRectangle axe_blade_bounds = axe.GetBladeBounds();
        MATH::Vector2f axe_center_position = axe_blade_bounds.GetCenterPosition();
        MAPS::TileMap* tile_map = tile_map_grid.GetTileMap(axe_center_position.X, axe_center_position.Y);
        if (!tile_map)
        {
            // A tile map must exist in order to deal with collisions in it.
            return;
        }

        // HANDLE AXE COLLISIONS WITH TREES.
        for (auto tree = tile_map->Trees.begin(); tile_map->Trees.end() != tree;)
        {
            MATH::FloatRectangle tree_bounds = tree->GetWorldBoundingBox();
            bool axe_hit_tree = axe_blade_bounds.Intersects(tree_bounds);
            if (axe_hit_tree)
            {
                // PLAY THE SOUND EFFECT FOR THE AXE HITTING A TREE.
                speakers.PlaySound(RESOURCES::AssetId::AXE_HIT_SOUND);

                // DAMAGE THE TREE.
                tree->TakeHit();

                // SHAKE THE TREE.
                tree->StartShaking();

                // ADD FOOD TO THE TILE MAP IF THE TREE HAD FOOD.
                if (tree->Food)
                {
                    // CALCULATE THE DESTINATION POSITION FOR THE FALLING FOOD.
                    // It should fall to directly below the tree.
                    float tree_bottom_y_position = tree->Sprite.GetWorldBoundingBox().GetBottomYPosition();
                    float food_height_in_pixels = tree->Food->Sprite.GetHeightInPixels();
                    float food_half_height_in_pixels = food_height_in_pixels / 2.0f;
                    MATH::Vector2f food_destination_world_position = tree->Food->Sprite.GetWorldPosition();
                    food_destination_world_position.Y = tree_bottom_y_position + food_half_height_in_pixels;

                    // START MAKING THE FOOD FALL OFF THE TREE.
                    OBJECTS::FallingFood falling_food;
                    falling_food.FoodItem = *tree->Food;
                    falling_food.DestinationWorldPosition = food_destination_world_position;
                    
                    // TRANSFER THE FOOD FROM THE TREE TO THE TILE MAP.
                    tile_map->FallingFood.push_back(falling_food);
                    tree->Food.reset();
                }

                // CHECK IF THE TREE STILL HAS ANY HIT POINTS.
                bool tree_still_has_hit_points = (tree->HitPoints > 0);
                if (tree_still_has_hit_points)
                {
                    // MOVE TO THE NEXT TREE FOR DETECTING COLLISIONS.
                    ++tree;
                }
                else
                {
                    // PLACE WOOD LOGS WHERE THE TREE WAS
                    // If the texture can't be loaded, then wood logs simply won't be added.
                    // The game will just continue as if no wood could be obtained from the tree.
                    std::shared_ptr<GRAPHICS::Texture> wood_log_texture = assets.GetTexture(RESOURCES::AssetId::WOOD_LOG_TEXTURE);
                    bool wood_log_texture_retrieved = (nullptr != wood_log_texture);
                    if (wood_log_texture_retrieved)
                    {
                        // ADD A GROUP OF WOOD LOGS AT THE DUST CLOUD'S POSITION.
                        OBJECTS::WoodLogs wood_logs;

                        const MATH::FloatRectangle WOOD_LOG_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(
                            32.0f, 32.0f, 16.0f, 16.0f);
                        wood_logs.Sprite = GRAPHICS::Sprite(wood_log_texture, WOOD_LOG_TEXTURE_SUB_RECTANGLE);

                        MATH::Vector2f wood_log_center_world_position = tree->GetTrunkCenterWorldPosition();
                        wood_logs.Sprite.SetWorldPosition(wood_log_center_world_position);

                        tile_map->WoodLogs.push_back(wood_logs);
                    }

                    // PLACE A DUST CLOUD WHERE THE TREE WAS.
                    // This signifies the tree being chopped down and turning into wood.
                    // If the resources can't be loaded, then a dust cloud simply won't be added.
                    // The game will just continue as if no wood could be obtained from the tree.
                    std::shared_ptr<GRAPHICS::Texture> dust_cloud_texture = assets.GetTexture(RESOURCES::AssetId::DUST_CLOUD_TEXTURE);
                    bool dust_cloud_resources_retrieved = (dust_cloud_texture != nullptr);
                    if (dust_cloud_resources_retrieved)
                    {
                        OBJECTS::DustCloud dust_cloud(dust_cloud_texture);

                        // The dust cloud should be positioned to cover the base of the tree.
                        MATH::Vector2f dust_cloud_center_world_position = tree->GetTrunkCenterWorldPosition();
                        dust_cloud.Sprite.SetWorldPosition(dust_cloud_center_world_position);

                        // The dust cloud should start animating immediately.
                        dust_cloud.Sprite.Play();

                        // The dust cloud needs to be added to the tile map so that it gets updated.
                        tile_map->DustClouds.push_back(dust_cloud);
                    }

                    // REMOVE THE TREE SINCE IT NO LONGER HAS ANY HIT POINTS.
                    tree = tile_map->Trees.erase(tree);
                }
            }
            else
            {
                // MOVE TO THE NEXT TREE FOR DETECTING COLLISIONS.
                ++tree;
            }
        }
    }

    /// Determines if an object collides with a tree in the map grid.
    /// @param[in]  rectangle - The bounding world rectangle of the object.
    /// @param[in,out]  tile_map_grid - The map grid in which the object and trees exist.
    /// @param[out] tree_rectangle - The bounding world rectangle of the tree, if a collision occurred.
    /// @return True if the object collided with a tree; false otherwise.
    bool CollisionDetectionAlgorithms::CollidesWithTree(
        const MATH::FloatRectangle& rectangle, 
        MAPS::MultiTileMapGrid& tile_map_grid,
        MATH::FloatRectangle& tree_rectangle)
    {
        // CLEAR THE OUT PARAMETER.
        tree_rectangle = MATH::FloatRectangle();

        // GET THE TREES NEAR THE RECTANGLE.
        MATH::Vector2f object_center_position = rectangle.GetCenterPosition();
        MAPS::TileMap* current_tile_map = tile_map_grid.GetTileMap(object_center_position.X, object_center_position.Y);
        bool tile_map_exists = (nullptr != current_tile_map);
        if (!tile_map_exists)
        {
            // No tile map exists with trees to collide with.
            return false;
        }

        // CHECK IF ANY OF THE TREES COLLIDE WITH THE RECTANGLE.
        for (auto tree = current_tile_map->Trees.cbegin(); current_tile_map->Trees.cend() != tree; ++tree)
        {
            // SHRINK THE TREE'S BOUNDING BOX SO THAT OBJECTS DON'T GET CAUGHT ON EDGES.
            const float TREE_DIMENSION_SHRINK_AMOUNT = 2.0f;
            MATH::FloatRectangle tree_bounds = tree->GetWorldBoundingBox();
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
}
