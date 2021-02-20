#include <cmath>
#include "Objects/FlamingSword.h"
#include "Resources/AssetId.h"

namespace OBJECTS
{
    /// Constructor.
    /// @param[in]  world_x_position - The world x position of the sword.
    /// @param[in]  world_y_position - The world y position of the sword.
    FlamingSword::FlamingSword(const float world_x_position, const float world_y_position) :
        Sprite(),
        TotalElapsedTime(sf::Time::Zero)
    {
        // CREATE THE SPRITE.
        const MATH::FloatRectangle TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 0, 16, 16);
        GRAPHICS::Sprite sprite(RESOURCES::AssetId::FLAMING_SWORD_TEXTURE, TEXTURE_SUB_RECTANGLE);
        sprite.WorldPosition = MATH::Vector2f(world_x_position, world_y_position);
        // The sprite origin should be the graphical center of its sub-rectangle.
        sprite.Origin = TEXTURE_SUB_RECTANGLE.Center();
        // The sprite should be visible by default.
        sprite.IsVisible = true;
        // The alpha starts at 0 to have it fade in more naturally.
        sprite.Color.Alpha = 0;
        Sprite = sprite;
    }

    /// Update the flaming sword for a frame of animation.
    /// @param[in]  elapsed_time - The elapsed time since the last frame.
    void FlamingSword::Update(const sf::Time& elasped_time)
    {
        // ROTATE THE FLAMING SWORD.
        constexpr float ROTATION_SPEED_IN_DEGREES_PER_SECOND = 120.0f;
        float elapsed_seconds = elasped_time.asSeconds();
        float rotation_amount_in_degrees = ROTATION_SPEED_IN_DEGREES_PER_SECOND * elapsed_seconds;
        Sprite.RotationAngleInDegrees += rotation_amount_in_degrees;

        // FADE THE FLAMING SWORD TO HAVE IT APPEAR/DISAPPEAR IN A MYSTERIOUS WAY.
        float sprite_tint_amount = std::sinf(TotalElapsedTime.asSeconds());
        Sprite.Color.Alpha = static_cast<uint8_t>(sprite_tint_amount * GRAPHICS::Color::MAX_COLOR_COMPONENT);

        // MAKE THE FLAMING SWORD INVISIBLE IF ITS MAX TIME HAS ELAPSED.
        const sf::Time MAX_ELAPSED_TIME = sf::seconds(3.0f);
        TotalElapsedTime += elasped_time;
        Sprite.IsVisible = (TotalElapsedTime < MAX_ELAPSED_TIME);
    }
}
