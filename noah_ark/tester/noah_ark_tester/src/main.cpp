#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "MapsTests/OverworldMapSpecificationTests.h"
#include "MapsTests/SurroundingMapLoaderTests.h"
#include "MathTests/FloatRectangleTests.h"
#include "PhysicsTests/CollisionTests/CollisionSystemTests.h"
#include "ResourcesTests/ResourceManagerTests.h"
#include "StatesTests/StateManagerTests.h"

TEST_CASE( "Final test case to use for debugging." )
{
    int i = 1;
    REQUIRE( i == 1 );
}