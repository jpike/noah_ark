#include "Objects/OfferingSmoke.h"
#include "Resources/AssetId.h"

namespace OBJECTS
{
    /// Creates offering smoke.
    /// @param[in]  altar_center_world_position - The center world position of the altar the smoke is on.
    OfferingSmoke::OfferingSmoke(const MATH::Vector2f& altar_center_world_position) :
        Sprite(),
        TotalElapsedTimeInSeconds(0.0f)
    {
        // CREATE THE ANIMATED SPRITE FOR THE OFERING SMOKE.
        const std::string SMOKE_ANIMATION_ID = "smoke";
        const bool IS_LOOPING = true;
        const sf::Time TOTAL_DURATION = sf::seconds(2.0f);
        const std::vector<MATH::IntRectangle> FRAMES =
        {
            // Frames are duplicated some so that things appear to loop a few times.
            MATH::IntRectangle::FromLeftTopAndDimensions(48, 0, 16, 32),
            MATH::IntRectangle::FromLeftTopAndDimensions(64, 0, 16, 32),
        };
        const std::shared_ptr<GRAPHICS::AnimationSequence> SMOKE_ANIMATION = std::make_shared<GRAPHICS::AnimationSequence>(
            SMOKE_ANIMATION_ID,
            IS_LOOPING,
            TOTAL_DURATION,
            FRAMES);
        Sprite = GRAPHICS::AnimatedSprite(RESOURCES::AssetId::OTHER_OBJECT_TEXTURE, SMOKE_ANIMATION);
        Sprite.UseAnimationSequence(SMOKE_ANIMATION_ID);

        // Since the smoke is a vertical column, it needs to be moved up a bit to appear on the altar.
        MATH::Vector2f smoke_world_position = altar_center_world_position;
        smoke_world_position.Y -= 16.0f;
        Sprite.SetWorldPosition(smoke_world_position);

        // The offering is part of the altar, so it starts out invisible until the player makes an offering.
        Sprite.CurrentFrameSprite.IsVisible = false;
    }

    /// Updates the state of the smoke based on the elapsed time.
    /// Smoke animates and disappears/fades away over time.
    /// @param[in]  elapsed_time - The elapsed time for which to update the smoke.
    void OfferingSmoke::Update(const sf::Time& elapsed_time)
    {
        // UPDATE THE FRAME-BASED ANIMATION.
        Sprite.Update(elapsed_time);

        // TRACK THE TOTAL TIME THIS CLOUD THAT BEEN AROUND.
        float elapsed_time_in_seconds = elapsed_time.asSeconds();
        TotalElapsedTimeInSeconds += elapsed_time_in_seconds;

        // ADD ADDITIONAL EFFECTS BASED ON THE CURRENT ANIMATION SEQUENCE.
        // Effects are changed to make things more visually appealing.
        std::shared_ptr<GRAPHICS::AnimationSequence> animation = Sprite.GetCurrentAnimationSequence();
        bool animation_exists = (nullptr != animation);
        if (animation_exists)
        {
            // DETERMINE HOW FAR THE ANIMATION HAS PROGRESSED.
            // This is needed to scale certain effects to the appropriate range based on elapsed time.
            float animation_duration_in_seconds = animation->TotalDuration.asSeconds();
            float animation_progress_ratio = (TotalElapsedTimeInSeconds / animation_duration_in_seconds);

            // SCALE THE SPRITE BASED ON HOW FAR THE ANIMATION HAS PROGRESSED.
            // It should get smaller over time.
            const float MAX_SCALE = 1.5f;
            const float MIN_SCALE = 0.5f;
            float scale = MAX_SCALE - animation_progress_ratio;
            bool scale_too_low = (scale < MIN_SCALE);
            if (scale_too_low)
            {
                scale = MIN_SCALE;
            }
            Sprite.SetScale(scale);

            // FADE THE SPRITE OUT AS THE ANIMATION PROGRESSES.
            GRAPHICS::Color color = Sprite.GetColor();
            color.Alpha = GRAPHICS::Color::MAX_COLOR_COMPONENT - static_cast<uint8_t>(animation_progress_ratio * GRAPHICS::Color::MAX_COLOR_COMPONENT);
            constexpr uint8_t MIN_ALPHA = 128;
            bool alpha_too_low = (color.Alpha < MIN_ALPHA);
            if (alpha_too_low)
            {
                color.Alpha = MIN_ALPHA;
            }
            Sprite.SetColor(color);

            // RESET THE ELAPSED TIME IF THE MAX TIME HAS BEEN EXCEEDED.
            // This animation should be looping.
            /// @todo   Smoother sine wave instead?
            bool one_cycle_of_animation_complete = (TotalElapsedTimeInSeconds > animation_duration_in_seconds);
            if (one_cycle_of_animation_complete)
            {
                TotalElapsedTimeInSeconds = 0.0f;
            }
        }
    }
}
