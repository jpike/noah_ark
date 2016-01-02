#include "Objects/Tree.h"

namespace OBJECTS
{
    MATH::FloatRectangle Tree::GetWorldBoundingBox() const
    {
        return Sprite.GetWorldBoundingBox();
    }

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

        /// @todo   Consider if randomness should be used here to make things
        /// more interesting or if we could have some additional direction
        /// parameter to have the tree shake in a more realistic way
        /// depending on how it was hit.
        
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

    void Tree::Update(const float elapsed_time_in_seconds)
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
