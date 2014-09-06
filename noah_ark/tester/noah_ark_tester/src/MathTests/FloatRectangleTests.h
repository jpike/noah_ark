#pragma once

#include "Math/FloatRectangle.h"

namespace FLOAT_RECTANGLE_TESTS
{

    TEST_CASE( "Constructor.", "[FloatRectangle][Constructor]" )
    {
        // CREATE THE TEST RECTANGLE.
        const float CENTER_X_POSITION = 70.0f;
        const float CENTER_Y_POSITION = 54.0f;
        const float WIDTH = 32.0f;
        const float HEIGHT = 47.0f;
        MATH::FloatRectangle testRectangle(
            CENTER_X_POSITION,
            CENTER_Y_POSITION,
            WIDTH,
            HEIGHT);

        // VALIDATE THE RECTANGLE.
        REQUIRE( CENTER_X_POSITION == testRectangle.GetCenterXPosition() );
        REQUIRE( CENTER_Y_POSITION == testRectangle.GetCenterYPosition() );
        REQUIRE( WIDTH == testRectangle.GetWidth() );
        REQUIRE( HEIGHT == testRectangle.GetHeight() );
    }

}