#include <cmath>
#include "Math/Number.h"
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

        // DETERMINE HOW FAR THE ANIMATION HAS PROGRESSED.
        // This is needed to scale certain effects to the appropriate range based on elapsed time.
        constexpr float MAX_SCALE = 1.5f;
        float signed_magnitude_of_sine_wave_through_animation = MAX_SCALE * std::sinf(TotalElapsedTimeInSeconds);
        float unsigned_magnitude_of_sine_wave_through_animation = std::fabsf(signed_magnitude_of_sine_wave_through_animation);
        float scale = unsigned_magnitude_of_sine_wave_through_animation;

        // SCALE THE SPRITE'S SIZE.
        Sprite.SetScale(scale);

        // SCALE THE SPRITE'S ALPHA FOR TRANSPARENCY.
        // Some manual clamping is needed to avoid integer truncation issues.
        GRAPHICS::Color color = Sprite.GetColor();
        color.Alpha = static_cast<uint8_t>(MATH::Number::Clamp<float>(
            (static_cast<float>(GRAPHICS::Color::MAX_COLOR_COMPONENT) * scale),
            static_cast<float>(GRAPHICS::Color::MIN_COLOR_COMPONENT),
            static_cast<float>(GRAPHICS::Color::MAX_COLOR_COMPONENT)));
        Sprite.SetColor(color);
    }
}
