#include "Objects/DustCloud.h"

namespace OBJECTS
{
    bool DustCloud::HasDisappeared() const
    {
        // The cloud hasn't disappeared if it is still animating.
        bool cloud_animating = Sprite.IsAnimating();
        return !cloud_animating;
    }

    void DustCloud::Update(const float elapsed_time_in_seconds)
    {
        // UPDATE THE FRAME-BASED ANIMATION.
        Sprite.Update(elapsed_time_in_seconds);

        // TRACK THE TOTAL TIME THIS CLOUD THAT BEEN AROUND.
        TotalElapsedTimeInSeconds += elapsed_time_in_seconds;

        // ADD ADDITIONAL EFFECTS BASED ON THE CURRENT ANIMATION SEQUENCE.
        // The rotation, scaling, and opacity effects below
        // just make the cloud more visually appealing.
        std::shared_ptr<GRAPHICS::AnimationSequence> animation = Sprite.GetCurrentAnimationSequence();
        bool animation_exists = (nullptr != animation);
        if (animation_exists)
        {
            // DETERMINE HOW FAR THE ANIMATION HAS PROGRESSED.
            // This is needed to scale certain effects to the appropriate range based on elapsed time.
            float animation_duration_in_seconds = animation->TotalDuration.asSeconds();
            float animation_progress_ratio = (TotalElapsedTimeInSeconds / animation_duration_in_seconds);

            // ROTATE THE CLOUD BASED ON HOW LONG IT HAS BEEN ANIMATING.
            // It will rotate in about a full circle by the time the cloud disappears.
            const float DEGREES_IN_FULL_CIRCLE = 360.0f;
            float rotation_angle = animation_progress_ratio * DEGREES_IN_FULL_CIRCLE;
            Sprite.SetRotation(rotation_angle);

            // SCALE THE SPRITE BASED ON HOW FAR THE ANIMATION HAS PROGRESSED.
            // It should get smaller over time.
            const float MAX_SCALE = 1.5f;
            float scale = MAX_SCALE - animation_progress_ratio;
            Sprite.SetScale(scale);

            // FADE THE SPRITE OUT AS THE ANIMATION PROGRESSES.
            GRAPHICS::Color color = Sprite.GetColor();
            color.Alpha = GRAPHICS::Color::MAX_COLOR_COMPONENT - static_cast<uint8_t>(animation_progress_ratio * GRAPHICS::Color::MAX_COLOR_COMPONENT);
            Sprite.SetColor(color);
        }
    }
}