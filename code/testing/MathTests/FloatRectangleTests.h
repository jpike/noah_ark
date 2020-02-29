#pragma once

#include "Math/Rectangle.h"

/// A namespace for testing the FloatRectangle class.
namespace FLOAT_RECTANGLE_TESTS
{

    TEST_CASE( "Constructor.", "[FloatRectangle][Constructor]" )
    {
        // CREATE THE TEST RECTANGLE.
        const float CENTER_X_POSITION = 70.0f;
        const float CENTER_Y_POSITION = 54.0f;
        const float WIDTH = 32.0f;
        const float HEIGHT = 47.0f;
        MATH::FloatRectangle testRectangle = MATH::FloatRectangle::FromCenterAndDimensions(
            CENTER_X_POSITION,
            CENTER_Y_POSITION,
            WIDTH,
            HEIGHT);

        // VALIDATE THE RECTANGLE.
        REQUIRE( CENTER_X_POSITION == testRectangle.CenterX() );
        REQUIRE( CENTER_Y_POSITION == testRectangle.CenterY() );
        REQUIRE( WIDTH == testRectangle.Width() );
        REQUIRE( HEIGHT == testRectangle.Height() );
    }

    TEST_CASE( "Change the center position.", "[FloatRectangle][SetCenterPosition]" )
    {
        // CREATE THE INITIAL TEST RECTANGLE.
        const float ORIGINAL_CENTER_X_POSITION = 70.0f;
        const float ORIGINAL_CENTER_Y_POSITION = 54.0f;
        const float WIDTH = 32.0f;
        const float HEIGHT = 47.0f;
        MATH::FloatRectangle testRectangle = MATH::FloatRectangle::FromCenterAndDimensions(
            ORIGINAL_CENTER_X_POSITION,
            ORIGINAL_CENTER_Y_POSITION,
            WIDTH,
            HEIGHT);

        // CHANGE THE CENTER POSITION.
        const float NEW_CENTER_X_POSITION = 97.0f;
        const float NEW_CENTER_Y_POSITION = -22.0f;
        testRectangle.SetCenterPosition(NEW_CENTER_X_POSITION, NEW_CENTER_Y_POSITION);

        // VALIDATE THE CHANGED RECTANGLE.
        REQUIRE( NEW_CENTER_X_POSITION == testRectangle.CenterX() );
        REQUIRE( NEW_CENTER_Y_POSITION == testRectangle.CenterY() );
        REQUIRE( WIDTH == testRectangle.Width() );
        REQUIRE( HEIGHT == testRectangle.Height() );
    }

}