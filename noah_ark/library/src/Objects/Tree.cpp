#include <cassert>
#include <random>
#include "Objects/Tree.h"

namespace OBJECTS
{
    Tree::Tree(const GRAPHICS::Sprite& sprite) :
    Sprite(sprite),
    HitPoints(Tree::INITIAL_HIT_POINTS),
    Shaking(false),
    ShakingOut(false),
    CurrentShakeRotationInDegrees(0.0f),
    CurrentShakeOutElapsedTimeInSeconds(0.0f),
    CurrentShakeInElapsedTimeInSeconds(0.0f),
    ShakeOutDirection(CORE::Direction::INVALID),
    InitialShakeOutDirection(CORE::Direction::INVALID)
    {}

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
        ShakingOut = true;
        CurrentShakeRotationInDegrees = 0.0f;
        CurrentShakeOutElapsedTimeInSeconds = 0.0f;
        CurrentShakeInElapsedTimeInSeconds = 0.0f;

        // DETERMINE A RANDOM DIRECTION.
        std::random_device random_number_generator;
        unsigned int random_number = random_number_generator();
        bool even_random_number = (0 == (random_number % 2));
        if (even_random_number)
        {
            ShakeOutDirection = CORE::Direction::LEFT;
        }
        else
        {
            ShakeOutDirection = CORE::Direction::RIGHT;
        }

        // To have the tree shake out in both directions,
        // the initial shake direction also needs to be preserved.
        InitialShakeOutDirection = ShakeOutDirection;
    }

    void Tree::Update(const float elapsed_time_in_seconds)
    {
        // ONLY UPDATE THE SHAKING IF THE TREE IS CURRENTLY SHAKING.
        if (!Shaking)
        {
            return;
        }

        // DETERMINE THE AMOUNT OF ROTATION FOR THE ELAPSED TIME.
        const float SHAKE_ROTATION_SPEED_IN_DEGREES_PER_SECOND = 30.0f;
        float shake_rotation_increase_in_degrees = SHAKE_ROTATION_SPEED_IN_DEGREES_PER_SECOND * elapsed_time_in_seconds;

        // DETERMINE IF THE TREE IS SHAKING OUT OR IN.
        if (ShakingOut)
        {
            // UPDATE THE SHAKE ROTATION BASED ON THE DIRECTION.
            switch (ShakeOutDirection)
            {
                case CORE::Direction::LEFT:
                    // Rotating out to the left is negative.
                    CurrentShakeRotationInDegrees -= shake_rotation_increase_in_degrees;
                    break;
                case CORE::Direction::RIGHT:
                    // Rotating out to the right is positive.
                    CurrentShakeRotationInDegrees += shake_rotation_increase_in_degrees;
                    break;
                default:
                    // Don't shake at all since an invalid direction was specified.
                    assert(false);
                    break;
            }

            // CHECK IF SHAKING OUT HAS FINISHED.
            const float MAX_SHAKE_OUT_TIME_IN_SECONDS = 0.25f;
            CurrentShakeOutElapsedTimeInSeconds += elapsed_time_in_seconds;
            bool shaking_out_finished = (CurrentShakeOutElapsedTimeInSeconds >= MAX_SHAKE_OUT_TIME_IN_SECONDS);
            if (shaking_out_finished)
            {
                // STOP SHAKING OUT.
                ShakingOut = false;

                // START SHAKING BACK IN.
                CurrentShakeInElapsedTimeInSeconds = 0.0f;
            }
        }
        else
        {
            // UPDATE THE SHAKE ROTATION BASED ON THE DIRECTION.
            switch (ShakeOutDirection)
            {
                case CORE::Direction::LEFT:
                    // Rotating back from the left is positive.
                    CurrentShakeRotationInDegrees += shake_rotation_increase_in_degrees;
                    break;
                case CORE::Direction::RIGHT:
                    // Rotating back from the right is negative.
                    CurrentShakeRotationInDegrees -= shake_rotation_increase_in_degrees;
                    break;
                default:
                    // Don't shake at all since an invalid direction was specified.
                    assert(false);
                    break;
            }

            // CHECK IF SHAKING BACK IN HAS FINISHED.
            const float MAX_SHAKE_IN_TIME_IN_SECONDS = 0.25f;
            CurrentShakeInElapsedTimeInSeconds += elapsed_time_in_seconds;
            bool shaking_in_finished = (CurrentShakeInElapsedTimeInSeconds >= MAX_SHAKE_IN_TIME_IN_SECONDS);
            if (shaking_in_finished)
            {
                // START SHAKING BACK OUT IN THE OPPOSITE DIRECTION IF NEEDED.
                // When shaking, the tree should shake out in both directions.
                // If this is only the first time shaking back in (the shake out
                // direction hasn't changed from its initial direction), then
                // we need to switch to swinging out in the opposite direction.
                bool only_first_shake_in_finished = (InitialShakeOutDirection == ShakeOutDirection);
                if (only_first_shake_in_finished)
                {
                    // START SHAKING BACK OUT.
                    ShakingOut = true;
                    CurrentShakeOutElapsedTimeInSeconds = 0.0f;

                    // Switch directions.
                    switch (ShakeOutDirection)
                    {
                        case CORE::Direction::LEFT:
                            ShakeOutDirection = CORE::Direction::RIGHT;
                            break;
                        case CORE::Direction::RIGHT:
                            ShakeOutDirection = CORE::Direction::LEFT;
                            break;
                        default:
                            // Don't doing anything since an invalid direction was specified.
                            assert(false);
                            break;
                    }
                }
                else
                {
                    // STOP SHAKING.
                    // The tree has finished shaking in both directions.
                    Shaking = false;
                    CurrentShakeRotationInDegrees = 0.0f;
                }
            }
        }

        // UPDATE THE SPRITE'S ROTATION BASED ON THE SHAKING.
        Sprite.SetRotation(CurrentShakeRotationInDegrees);
    }
}
