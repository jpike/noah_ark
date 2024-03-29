#include "ErrorHandling/NullChecking.h"
#include "Objects/DustCloud.h"

namespace OBJECTS
{
    /// Constructs a dust cloud using the provided texture resource.
    /// @param[in]  texture_id - The ID of the texture with the dust cloud graphics.
    DustCloud::DustCloud(const RESOURCES::AssetId texture_id) :
        Sprite(),
        TotalElapsedTimeInSeconds(0.0f)
    {
        // CREATE THE ANIMATED SPRITE FOR THE DUST CLOUD.
        const std::string DUST_CLOUD_ANIMATION_ID = "dust_cloud";
        const bool IS_LOOPING = false;
        const sf::Time TOTAL_DURATION = sf::seconds(2.0f);
        const std::vector<MATH::IntRectangle> FRAMES =
        {
            // Frames are duplicated some so that things appear to loop a few times.
            MATH::IntRectangle::FromLeftTopAndDimensions(48, 0, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(48, 16, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(48, 0, 16, 16),
            MATH::IntRectangle::FromLeftTopAndDimensions(48, 16, 16, 16)
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> DUST_CLOUD_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            DUST_CLOUD_ANIMATION_ID,
            IS_LOOPING,
            TOTAL_DURATION,
            FRAMES);
        Sprite = GRAPHICS::AnimatedSprite(texture_id, DUST_CLOUD_ANIMATION);
    }

    /// Determines if the cloud has disappeared or faded away.
    /// @return True if the cloud has disappeared; false otherwise.
    bool DustCloud::HasDisappeared() const
    {
        // The cloud hasn't disappeared if it is still animating.
        bool cloud_animating = Sprite.IsAnimating();
        return !cloud_animating;
    }

    /// Updates the state of the dust cloud based on the elapsed time.
    /// Dust clouds animate and disappear/fade away over time.
    /// @param[in]  elapsed_time - The elapsed time for which
    ///     to update the dust cloud.
    void DustCloud::Update(const sf::Time& elapsed_time)
    {
        // UPDATE THE FRAME-BASED ANIMATION.
        Sprite.Update(elapsed_time);

        // TRACK THE TOTAL TIME THIS CLOUD THAT BEEN AROUND.
        float elapsed_time_in_seconds = elapsed_time.asSeconds();
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
