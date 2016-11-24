#pragma once

#include <stdexcept>
#include "Graphics/AnimatedSprite.h"

/// A namespace for testing the AnimatedSprite class.
namespace TEST_ANIMATED_SPRITE
{
    TEST_CASE( "Constructor.  Verify world position.", "[AnimatedSprite][Constructor][WorldPosition]" )
    {
        // CONSTRUCT A SPRITE WITH AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_WORLD_X_POSITION = 40.0f;
        const float INITIAL_WORLD_Y_POSITION = 64.0f;
        GRAPHICS::Sprite sprite;
        sprite.SetWorldPosition(INITIAL_WORLD_X_POSITION, INITIAL_WORLD_Y_POSITION);

        // CONSTRUCT AN ANIMATED SPRITE FROM THE SPRITE.
        GRAPHICS::AnimatedSprite animatedSprite(sprite);
        
        // VERIFY THAT THE ANIMATED SPRITE'S WORLD POSITION WAS INITIALIZED FROM THE SPRITE.
        MATH::Vector2f animatedSpriteWorldPosition = animatedSprite.GetWorldPosition();
        // The components of the position are verified using the constants, rather than
        // the initial sprite, because the sprite is passed as a shared_ptr,
        // which means its position could have theoretically been modified in the constructor.
        REQUIRE( INITIAL_WORLD_X_POSITION == animatedSpriteWorldPosition.X );
        REQUIRE( INITIAL_WORLD_Y_POSITION == animatedSpriteWorldPosition.Y );
    }

    TEST_CASE( "Constructor.  Verify bounding box.", "[AnimatedSprite][Constructor][BoundingBox]" )
    {
        // CONSTRUCT A SPRITE WITH AN INITIAL POSITION AND SIZE.
        GRAPHICS::Sprite sprite;
        
        // The initial position is arbitrary.
        const float INITIAL_WORLD_X_POSITION = 40.0f;
        const float INITIAL_WORLD_Y_POSITION = 64.0f;
        sprite.SetWorldPosition(INITIAL_WORLD_X_POSITION, INITIAL_WORLD_Y_POSITION);

        // The initial size is chosen arbitrary.
        const int SPRITE_WIDTH = 10;
        const int SPRITE_HEIGHT = 35;
        MATH::IntRectangle spriteTextureRectangle = MATH::IntRectangle::FromLeftTopAndDimensions(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
        sprite.SetTextureRectangle(spriteTextureRectangle);

        const float SPRITE_CENTER_X = static_cast<float>(SPRITE_WIDTH) / 2.0f;
        const float SPRITE_CENTER_Y = static_cast<float>(SPRITE_HEIGHT) / 2.0f;
        sprite.SetOrigin(MATH::Vector2f(SPRITE_CENTER_X, SPRITE_CENTER_Y));

        // CONSTRUCT AN ANIMATED SPRITE FROM THE SPRITE.
        GRAPHICS::AnimatedSprite animatedSprite(sprite);
        
        // VERIFY THAT THE ANIMATED SPRITE'S BOUNDING BOX IS CORRECT.
        MATH::FloatRectangle animatedSpriteBoundingBox = animatedSprite.GetWorldBoundingBox();
        REQUIRE( INITIAL_WORLD_X_POSITION == animatedSpriteBoundingBox.GetCenterXPosition() );
        REQUIRE( INITIAL_WORLD_Y_POSITION == animatedSpriteBoundingBox.GetCenterYPosition() );
        // The width/height are cast to integers to allow validation despite floating-point issues.
        // Integral values are expected, so they sould cast directly to integers without problem.
        REQUIRE( SPRITE_WIDTH == static_cast<int>(animatedSpriteBoundingBox.GetWidth()) );
        REQUIRE( SPRITE_HEIGHT == static_cast<int>(animatedSpriteBoundingBox.GetHeight()) );
    }

    TEST_CASE( "Set new position.  Negative values.", "[AnimatedSprite][SetWorldPosition][NegativeValues]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE.
        GRAPHICS::Sprite sprite;
        GRAPHICS::AnimatedSprite animatedSprite(sprite);
        
        // SET A NEW POSITION.
        // The new position is negative but otherwise arbitrary.
        const float NEW_X_POSITION = -28.0f;
        const float NEW_Y_POSITION = -15.0f;
        animatedSprite.SetWorldPosition(NEW_X_POSITION, NEW_Y_POSITION);

        // VERIFY THAT THE ANIMATED SPRITE NOW HAS THE NEW POSITION.
        MATH::Vector2f actualNewSpritePosition = animatedSprite.GetWorldPosition();
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(NEW_X_POSITION) == static_cast<int>(actualNewSpritePosition.X) );
        REQUIRE( static_cast<int>(NEW_Y_POSITION) == static_cast<int>(actualNewSpritePosition.Y) );

        // VERIFY THAT THE BOUNDING BOX HAS THE NEW CENTER POSITION.
        MATH::FloatRectangle newBoundingBox = animatedSprite.GetWorldBoundingBox();
        // Values are cast to integers for the same reason as the direct position.
        REQUIRE( static_cast<int>(NEW_X_POSITION) == static_cast<int>(newBoundingBox.GetCenterXPosition()) );
        REQUIRE( static_cast<int>(NEW_Y_POSITION) == static_cast<int>(newBoundingBox.GetCenterYPosition()) );
    }
}