#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "MapsTests/OverworldMapSpecificationTests.h"
/// @todo   The following tests are commented out because they seem to result
///         in catch compilation errors that I haven't been able to resolve yet.
// #include "MapsTests/SurroundingMapLoaderTests.h"
#include "PhysicsTests/CollisionTests/CollisionSystemTests.h"
#include "StatesTests/StateManagerTests.h"

TEST_CASE( "Final test case to use for debugging." )
{
    int i = 1;
    REQUIRE( i == 1 );
}