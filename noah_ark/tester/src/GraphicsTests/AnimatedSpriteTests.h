#pragma once

#include <stdexcept>
#include "Graphics/AnimatedSprite.h"

/// A namespace for testing the AnimatedSprite class.
namespace TEST_ANIMATED_SPRITE
{
    TEST_CASE( "Constructor.  Null sprite.", "[AnimatedSprite][Constructor][Failure]" )
    {
        // CONSTRUCT THE ANIMATED SPRITE WITH A NULL SPRITE RESOURCE.
        // We want to verify that the appropriate exception is thrown.
        bool exceptionThrown = false;
        try
        {
            GRAPHICS::AnimatedSprite animatedSprite(nullptr);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }

        // VERIFY THAT AN EXCEPTION WAS THROWN.
        REQUIRE(exceptionThrown);
    }

    TEST_CASE( "Constructor.  Valid sprite.", "[AnimatedSprite][Constructor][Success]" )
    {
        // CONSTRUCT THE ANIMATED SPRITE WITH A VALID SPRITE RESOURCE.
        // We want to verify that the appropriate exception is not thrown.
        bool exceptionThrown = false;
        try
        {
            std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
            GRAPHICS::AnimatedSprite animatedSprite(spriteResource);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }

        // VERIFY THAT AN EXCEPTION WAS NOT THROWN.
        REQUIRE(!exceptionThrown);
    }

    TEST_CASE( "Constructor.  Verify visible status.", "[AnimatedSprite][Constructor][Visibility]" )
    {
        // CONSTRUCT THE ANIMATED SPRITE WITH A VALID SPRITE RESOURCE..
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);
        
        // VERIFY THAT THE ANIMATED SPRITE IS VISIBLE BY DEFAULT
        REQUIRE( true == animatedSprite.IsVisible() );
    }

    TEST_CASE( "Constructor.  Verify world position.", "[AnimatedSprite][Constructor][WorldPosition]" )
    {
        // CONSTRUCT A SPRITE RESOURCE WITH AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_WORLD_X_POSITION = 40.0f;
        const float INITIAL_WORLD_Y_POSITION = 64.0f;
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        spriteResource->setPosition(INITIAL_WORLD_X_POSITION, INITIAL_WORLD_Y_POSITION);

        // CONSTRUCT AN ANIMATED SPRITE FROM THE RESOURCE.
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);
        
        // VERIFY THAT THE ANIMATED SPRITE'S WORLD POSITION WAS INITIALIZED FROM THE SPRITE.
        MATH::Vector2f animatedSpriteWorldPosition = animatedSprite.GetWorldPosition();
        // The components of the position are verified using the constants, rather than
        // the initial sprite resource, because the sprite resource is passed as a shared_ptr,
        // which means its position could have theoretically been modified in the constructor.
        REQUIRE( INITIAL_WORLD_X_POSITION == animatedSpriteWorldPosition.X );
        REQUIRE( INITIAL_WORLD_Y_POSITION == animatedSpriteWorldPosition.Y );
    }

    TEST_CASE( "Constructor.  Verify bounding box.", "[AnimatedSprite][Constructor][BoundingBox]" )
    {
        // CONSTRUCT A SPRITE RESOURCE WITH AN INITIAL POSITION AND SIZE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        
        // The initial position is arbitrary.
        const float INITIAL_WORLD_X_POSITION = 40.0f;
        const float INITIAL_WORLD_Y_POSITION = 64.0f;
        spriteResource->setPosition(INITIAL_WORLD_X_POSITION, INITIAL_WORLD_Y_POSITION);

        // The initial size is chosen arbitrary.
        const int SPRITE_WIDTH = 10;
        const int SPRITE_HEIGHT = 35;
        sf::IntRect spriteTextureRectangle(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
        spriteResource->setTextureRect(spriteTextureRectangle);

        /// @todo   Rethink about how centering should occur.
        const float SPRITE_CENTER_X = static_cast<float>(SPRITE_WIDTH) / 2.0f;
        const float SPRITE_CENTER_Y = static_cast<float>(SPRITE_HEIGHT) / 2.0f;
        spriteResource->setOrigin(SPRITE_CENTER_X, SPRITE_CENTER_Y);

        // CONSTRUCT AN ANIMATED SPRITE FROM THE RESOURCE.
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);
        
        // VERIFY THAT THE ANIMATED SPRITE'S BOUNDING BOX IS CORRECT.
        MATH::FloatRectangle animatedSpriteBoundingBox = animatedSprite.GetWorldBoundingBox();
        REQUIRE( INITIAL_WORLD_X_POSITION == animatedSpriteBoundingBox.GetCenterXPosition() );
        REQUIRE( INITIAL_WORLD_Y_POSITION == animatedSpriteBoundingBox.GetCenterYPosition() );
        // The width/height are cast to integers to allow validation despite floating-point issues.
        // Integral values are expected, so they sould cast directly to integers without problem.
        REQUIRE( SPRITE_WIDTH == static_cast<int>(animatedSpriteBoundingBox.GetWidth()) );
        REQUIRE( SPRITE_HEIGHT == static_cast<int>(animatedSpriteBoundingBox.GetHeight()) );
    }

    TEST_CASE( "Set null position component.", "[AnimatedSprite][SetPositionComponent][Failure]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);
        
        // SET A NULL POSITION COMPONENT.
        // We want to verify that an exception is thrown.
        bool exceptionThrown = false;
        try
        {
            animatedSprite.SetPositionComponent(nullptr);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }

        // VERIFY THAT AN EXCEPTION WAS THROWN.
        REQUIRE(exceptionThrown);
    }

    TEST_CASE( "Set valid position component.", "[AnimatedSprite][SetPositionComponent][Success]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);
        
        // SET A VALID POSITION COMPONENT.
        // The new position is arbitrary.
        const float NEW_X_POSITION = 128.0f;
        const float NEW_Y_POSITION = 7.0f;
        std::shared_ptr<MATH::Vector2f> newPositionComponent = std::make_shared<MATH::Vector2f>(
            NEW_X_POSITION,
            NEW_Y_POSITION);
        // We want to verify that an exception is not thrown.
        bool exceptionThrown = false;
        try
        {
            animatedSprite.SetPositionComponent(newPositionComponent);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }

        // VERIFY THAT AN EXCEPTION WAS NOT THROWN.
        REQUIRE(!exceptionThrown);

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

    TEST_CASE( "Set new position.  Negative values.", "[AnimatedSprite][SetWorldPosition][NegativeValues]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);
        
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

    TEST_CASE( "Move animated sprite up by negative amount.", "[AnimatedSprite][MoveUp][Negative]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE UP BY A NEGATIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = -11.0f;
        animatedSprite.MoveUp(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and upward movement is in the negative Y direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite up by zero.", "[AnimatedSprite][MoveUp][Zero]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE UP BY ZERO.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 0.0f;
        animatedSprite.MoveUp(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and upward movement is in the negative Y direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite up by positive amount.", "[AnimatedSprite][MoveUp][Positive]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE UP BY A POSITIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 8.0f;
        animatedSprite.MoveUp(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and upward movement is in the negative Y direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite down by negative amount.", "[AnimatedSprite][MoveDown][Negative]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE DOWN BY A NEGATIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = -11.0f;
        animatedSprite.MoveDown(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and downward movement is in the positive Y direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite down by zero.", "[AnimatedSprite][MoveDown][Zero]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE DOWN BY ZERO.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 0.0f;
        animatedSprite.MoveDown(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and downward movement is in the positive Y direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite down by positive amount.", "[AnimatedSprite][MoveDown][Positive]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE DOWN BY A POSITIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 8.0f;
        animatedSprite.MoveDown(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and downward movement is in the positive Y direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite left by negative amount.", "[AnimatedSprite][MoveLeft][Negative]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE LEFT BY A NEGATIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = -11.0f;
        animatedSprite.MoveLeft(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and leftward movement is in the negative X direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite left by zero.", "[AnimatedSprite][MoveLeft][Zero]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE LEFT BY ZERO.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 0.0f;
        animatedSprite.MoveLeft(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and leftward movement is in the negative X direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite left by positive amount.", "[AnimatedSprite][MoveLeft][Positive]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE LEFT BY A POSITIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 8.0f;
        animatedSprite.MoveLeft(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and leftward movement is in the negative X direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite right by negative amount.", "[AnimatedSprite][MoveRight][Negative]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE RIGHT BY A NEGATIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = -11.0f;
        animatedSprite.MoveRight(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and rightward movement is in the positive X direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite right by zero.", "[AnimatedSprite][MoveRight][Zero]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE RIGHT BY ZERO.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 0.0f;
        animatedSprite.MoveRight(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and rightward movement is in the positive X direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }

    TEST_CASE( "Move animated sprite right by positive amount.", "[AnimatedSprite][MoveRight][Positive]" )
    {
        // CONSTRUCT AN ANIMATED SPRITE FROM A VALID SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>();
        GRAPHICS::AnimatedSprite animatedSprite(spriteResource);

        // SET AN INITIAL POSITION.
        // The initial position is arbitrary.
        const float INITIAL_X_POSITION = 32.0f;
        const float INITIAL_Y_POSITION = 55.0f;
        animatedSprite.SetWorldPosition(INITIAL_X_POSITION, INITIAL_Y_POSITION);

        // MOVE THE SPRITE RIGHT BY A POSITIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 8.0f;
        animatedSprite.MoveRight(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and rightward movement is in the positive X direction.
        const float EXPECTED_FINAL_X_POSITION = INITIAL_X_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_Y_POSITION = INITIAL_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        MATH::Vector2f actualFinalPosition = animatedSprite.GetWorldPosition();
        REQUIRE( static_cast<int>(EXPECTED_FINAL_X_POSITION) == static_cast<int>(actualFinalPosition.X) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_Y_POSITION) == static_cast<int>(actualFinalPosition.Y) );
    }
}