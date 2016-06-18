#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "CoreTests/Array2DTests.h"
#include "GraphicsTests/AnimatedSpriteTests.h"
#include "GraphicsTests/AnimationSequenceTests.h"
#include "GraphicsTests/GuiTests/TextBoxTests.h"
#include "GraphicsTests/GuiTests/TextPageTests.h"
#include "MathTests/FloatRectangleTests.h"

TEST_CASE( "Final test case to use for debugging." )
{
    int i = 1;
    REQUIRE( i == 1 );
}