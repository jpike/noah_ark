#include "Objects/Tree.h"

namespace OBJECTS
{
    /// Gets the bounding box of the tree, in world coordinates.
    /// @return The tree's bounding box.
    MATH::FloatRectangle Tree::GetWorldBoundingBox() const
    {
        return Sprite.GetWorldBoundingBox();
    }

    /// Gets the center world position of the tree's trunk.
    /// @return The center world position of the tree's trunk.
    MATH::Vector2f Tree::GetTrunkCenterWorldPosition() const
    {
        // GET THE BOUNDING BOX OF THE TREE.
        MATH::FloatRectangle world_bounding_box = GetWorldBoundingBox();

        // CALCULATE THE CENTER OF THE TREE'S TRUNK.
        // The center will be set so that it works for all current tree sizes.
        // It is vertically placed halfway up the pixel grid for the smallest
        // tree size, which also works well for other tree sizes.
        const float OFFSET_FROM_TREE_BOTTOM_TO_TRUNK_CENTER_IN_PIXELS = 8.0f;
        float tree_bottom_world_y_position = world_bounding_box.GetBottomYPosition();
        // Y gets smaller going up.
        float trunk_center_y_position = tree_bottom_world_y_position - OFFSET_FROM_TREE_BOTTOM_TO_TRUNK_CENTER_IN_PIXELS;
        float trunk_center_x_position = world_bounding_box.GetCenterXPosition();
        MATH::Vector2f trunk_center_world_position(
            trunk_center_x_position,
            trunk_center_y_position);
        return trunk_center_world_position;
    }

    /// Damages the tree by taking one hit.
    void Tree::TakeHit()
    {
        // CHECK IF THE TREE HAS ANY REMAINING HIT POINTS.
        // Damage should only be taken if hit points remain
        // to prevent numeric wraparound.
        bool hit_points_remain = (HitPoints > 0);
        if (hit_points_remain)
        {
            // TAKE A HIT.
            --HitPoints;
        }
    }

    /// Starts shaking the tree.  Intended
    /// for whenever the tree gets hit.
    void Tree::StartShaking()
    {
        // INITIALIZE THE SHAKING PARAMETERS FOR A NEW SHAKE.
        Shaking = true;
        CurrentShakeRotationInDegrees = 0.0f;
        CurrentShakeSubRotationIndex = 0;
        ShakeSubRotations.clear();

        // DEFINE THE SEQUENCE OF ROTATIONS FOR THE SHAKE.
        const float SHAKE_ROTATION_SPEED_IN_DEGREES_PER_SECOND = 30.0f;
        const float FULLY_SHAKED_OUT_ROTATION_ANGLE_MAGNITUDE = 10.0f;
        
        // Rotate out to the right.
        TreeShakeSubRotation first_rotation;
        first_rotation.RotationVelocityInDegreesPerSecond = SHAKE_ROTATION_SPEED_IN_DEGREES_PER_SECOND;
        first_rotation.DestinationRotationAngleInDegrees = FULLY_SHAKED_OUT_ROTATION_ANGLE_MAGNITUDE;
        ShakeSubRotations.push_back(first_rotation);

        // Rotate back to the left.
        TreeShakeSubRotation second_rotation;
        second_rotation.RotationVelocityInDegreesPerSecond = -SHAKE_ROTATION_SPEED_IN_DEGREES_PER_SECOND;
        second_rotation.DestinationRotationAngleInDegrees = -FULLY_SHAKED_OUT_ROTATION_ANGLE_MAGNITUDE;
        ShakeSubRotations.push_back(second_rotation);

        // Rotate back to the center.
        TreeShakeSubRotation third_rotation;
        third_rotation.RotationVelocityInDegreesPerSecond = SHAKE_ROTATION_SPEED_IN_DEGREES_PER_SECOND;
        second_rotation.DestinationRotationAngleInDegrees = 0.0f;
        ShakeSubRotations.push_back(third_rotation);
    }

    /// Updates the shaking of the tree, if the tree is currently shaking.
    /// @param[in]  elapsed_time - The elapsed time for which to
    ///     update the tree's shaking.
    void Tree::Update(const sf::Time& elapsed_time)
    {
        // ONLY UPDATE THE SHAKING IF THE TREE IS CURRENTLY SHAKING.
        if (!Shaking)
        {
            // There isn't a need to stop the tree shaking sound effect.
            // Doing so would cause a pop.  It is better to just let it
            // finish on its own.
            return;
        }

        // MAKE SURE THE TREE SHAKING SOUND IS PLAYING IF IT EXISTS.
        if (TreeShakeSound)
        {
            // ONLY START PLAYING THE SOUND IF IT ISN'T ALREADY PLAYING.
            // This results in a smoother sound experience.
            bool tree_shake_sound_playing = TreeShakeSound->IsPlaying();
            if (!tree_shake_sound_playing)
            {
                TreeShakeSound->Play();
            }
        }

        // UPDATE THE TREE'S ROTATION BASED ON THE CURRENT SHAKING PARAMETERS.
        const TreeShakeSubRotation& current_shake_rotation = ShakeSubRotations[CurrentShakeSubRotationIndex];
        float elapsed_time_in_seconds = elapsed_time.asSeconds();
        float shake_rotation_change_in_degrees = current_shake_rotation.RotationVelocityInDegreesPerSecond * elapsed_time_in_seconds;
        CurrentShakeRotationInDegrees += shake_rotation_change_in_degrees;

        // CHECK IF THE TREE'S CURRENT SUB-ROTATION IS COMPLETE.
        // Assume that the current rotation is complete by default (if a rotation change of 0 is detected)
        // to reduce the length of the if/else block handling.
        // This shouldn't ever happen (it indicates an error in constructing the sub-rotations)
        bool current_shake_sub_rotation_complete = true;
        bool positive_shake_rotation = (shake_rotation_change_in_degrees > 0.0f);
        bool negative_shake_rotation = (shake_rotation_change_in_degrees < 0.0f);
        if (positive_shake_rotation)
        {
            current_shake_sub_rotation_complete = (CurrentShakeRotationInDegrees >= current_shake_rotation.DestinationRotationAngleInDegrees);
            
        }
        else if (negative_shake_rotation)
        {
            current_shake_sub_rotation_complete = (CurrentShakeRotationInDegrees <= current_shake_rotation.DestinationRotationAngleInDegrees);
        }

        // MOVE TO THE NEXT SHAKE SUB-ROTATION IF THE CURRENT ONE IS COMPLETE.
        if (current_shake_sub_rotation_complete)
        {
            // MOVE TO THE NEXT SUB-ROTATION FOR THE TREE SHAKE.
            ++CurrentShakeSubRotationIndex;

            // STOP THE TREE FROM SHAKING IF THE LAST SUB-ROTATION HAS COMPLETED.
            Shaking = (CurrentShakeSubRotationIndex < ShakeSubRotations.size());
            if (!Shaking)
            {
                // When the tree finishes shaking, it should be set to its default state of no rotation.
                CurrentShakeRotationInDegrees = 0.0f;
            }
        }

        // UPDATE THE SPRITE'S ROTATION BASED ON THE SHAKING.
        Sprite.SetRotation(CurrentShakeRotationInDegrees);
    }
}
