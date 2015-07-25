#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "CollisionTests/CollisionSystemTests.h"
#include "CoreTests/Array2DTests.h"
#include "GraphicsTests/AnimatedSpriteTests.h"
#include "GraphicsTests/AnimationSequenceTests.h"
#include "GraphicsTests/CameraTests.h"
#include "GraphicsTests/GraphicsSystemTests.h"
#include "MapsTests/OverworldMapSpecificationTests.h"
#include "MapsTests/SurroundingMapLoaderTests.h"
#include "MapsTests/TiledMapJsonFileTests.h"
#include "MapsTests/TileMapTests.h"
#include "MathTests/FloatRectangleTests.h"
#include "ResourcesTests/ResourceManagerTests.h"
#include "StatesTests/StateManagerTests.h"

TEST_CASE( "Final test case to use for debugging." )
{
    int i = 1;
    REQUIRE( i == 1 );
}