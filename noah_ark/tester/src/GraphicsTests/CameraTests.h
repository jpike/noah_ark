#pragma once

#include "Graphics/Camera.h"

/// A namespace for testing the Camera class.
namespace TEST_CAMERA
{
    TEST_CASE( "Constructor.  Default values.", "[Camera][Constructor][DefaultValues]" )
    {
        // CONSTRUCT A CAMERA WITH DEFAULT VALUES.
        GRAPHICS::Camera camera;

        // VERIFY THAT THE FIELDS ARE PROPERLY SET.
        // Both position coordinates should be at 0,0.
        // Values are cast to integers to avoid floating-point issues.  Numbers used
        // in test cases are intentionally integers to make validation easier.
        // Differences less than a whole integer should be unimportant since that
        // represents less than a single pixel, which should not be visible to players.
        const int EXPECTED_LEFT_X_POSITION = 0;
        const int EXPECTED_TOP_Y_POSITION = 0;

        REQUIRE( EXPECTED_LEFT_X_POSITION == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( EXPECTED_TOP_Y_POSITION == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Constructor.  Non-default values.", "[Camera][Constructor][NonDefaultValues]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        // The specific values are arbitrary.
        const float TEST_LEFT_X_POSITION = -243.0f;
        const float TEST_TOP_Y_POSITION = 10.0f;
        GRAPHICS::Camera camera(TEST_LEFT_X_POSITION, TEST_TOP_Y_POSITION);

        // VERIFY THAT THE FIELDS ARE PROPERLY SET.
        // Values are cast to integers to avoid floating-point issues.  Numbers used
        // in test cases are intentionally integers to make validation easier.
        // Differences less than a whole integer should be unimportant since that
        // represents less than a single pixel, which should not be visible to players.
        REQUIRE( static_cast<int>(TEST_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(TEST_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Set top-left view position.", "[Camera][SetTopLeftViewPosition]" )
    {
        // CONSTRUCT A CAMERA WITH DEFAULT VALUES.
        GRAPHICS::Camera camera;

        // SET A VIEW POSITION DIFFERENT FROM THE DEFAULT VALUES.
        const float TEST_LEFT_X_POSITION = 44.0f;
        const float TEST_TOP_Y_POSITION = 57.0f;
        const MATH::Vector2f TEST_VIEW_POSITION(TEST_LEFT_X_POSITION, TEST_TOP_Y_POSITION);
        camera.SetTopLeftViewPosition(TEST_VIEW_POSITION);

        // VERIFY THAT THE FIELDS ARE PROPERLY SET.
        // Values are cast to integers to avoid floating-point issues.  Numbers used
        // in test cases are intentionally integers to make validation easier.
        // Differences less than a whole integer should be unimportant since that
        // represents less than a single pixel, which should not be visible to players.
        REQUIRE( static_cast<int>(TEST_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(TEST_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera up by negative amount.", "[Camera][MoveUp][Negative]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA UP BY A NEGATIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = -11.0f;
        camera.MoveUp(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and upward movement is in the negative Y direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera up by zero.", "[Camera][MoveUp][Zero]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA UP BY ZERO.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 0.0f;
        camera.MoveUp(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and upward movement is in the negative Y direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera up by positive amount.", "[Camera][MoveUp][Positive]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA UP BY A POSITIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 9.0f;
        camera.MoveUp(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and upward movement is in the negative Y direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera down by negative amount.", "[Camera][MoveDown][Negative]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA DOWN BY A NEGATIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = -11.0f;
        camera.MoveDown(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and downward movement is in the positive Y direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera down by zero.", "[Camera][MoveDown][Zero]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA DOWN BY ZERO.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 0.0f;
        camera.MoveDown(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and downward movement is in the positive Y direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera down by positive amount.", "[Camera][MoveDown][Positive]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA DOWN BY A POSITIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 9.0f;
        camera.MoveDown(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the Y position should change by the amount moved,
        // and downward movement is in the positive Y direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera left by negative amount.", "[Camera][MoveLeft][Negative]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA LEFT BY A NEGATIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = -11.0f;
        camera.MoveLeft(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and leftward movement is in the negative X direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera left by zero.", "[Camera][MoveLeft][Zero]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA LEFT BY ZERO.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 0.0f;
        camera.MoveLeft(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and leftward movement is in the negative X direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera left by positive amount.", "[Camera][MoveLeft][Positive]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA LEFT BY A POSITIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 9.0f;
        camera.MoveLeft(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and leftward movement is in the negative X direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION - MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera right by negative amount.", "[Camera][MoveRight][Negative]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA RIGHT BY A NEGATIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = -11.0f;
        camera.MoveRight(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and rightward movement is in the positive X direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera right by zero.", "[Camera][MoveRight][Zero]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA RIGHT BY ZERO.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 0.0f;
        camera.MoveRight(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and rightward movement is in the positive X direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }

    TEST_CASE( "Move camera right by positive amount.", "[Camera][MoveRight][Positive]" )
    {
        // CONSTRUCT A CAMERA WITH NON-DEFAULT VALUES.
        const float INITIAL_LEFT_X_POSITION = 32.0f;
        const float INITIAL_TOP_Y_POSITION = 55.0f;
        GRAPHICS::Camera camera(INITIAL_LEFT_X_POSITION, INITIAL_TOP_Y_POSITION);

        // MOVE THE CAMERA RIGHT BY A POSITIVE AMOUNT.
        // The specific amount is arbitrary.
        const float MOVEMENT_AMOUNT_IN_PIXELS = 9.0f;
        camera.MoveRight(MOVEMENT_AMOUNT_IN_PIXELS);

        // VERIFY THE FINAL POSITION.
        // Only the X position should change by the amount moved,
        // and rightward movement is in the positive X direction.
        const float EXPECTED_FINAL_LEFT_X_POSITION = INITIAL_LEFT_X_POSITION + MOVEMENT_AMOUNT_IN_PIXELS;
        const float EXPECTED_FINAL_TOP_Y_POSITION = INITIAL_TOP_Y_POSITION;
        // The positions are cast to integers to allow for validation ignoring floating-point issues.
        // It is expected that the positions should be convert directly to integers.  A position
        // that differs by less than an integral amount also isn't that important since that means
        // it differs by less than a pixel, which shouldn't be that noticeable to a human eye.
        REQUIRE( static_cast<int>(EXPECTED_FINAL_LEFT_X_POSITION) == static_cast<int>(camera.GetLeftViewPosition()) );
        REQUIRE( static_cast<int>(EXPECTED_FINAL_TOP_Y_POSITION) == static_cast<int>(camera.GetTopViewPosition()) );
    }
}