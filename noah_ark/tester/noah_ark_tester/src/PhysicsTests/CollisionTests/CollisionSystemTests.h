#pragma once

// This guard is needed to avoid issues where catch is already included.
// Otherwise, we can't split up tests into multiple files.
#ifndef CATCH_CONFIG_MAIN
#include <catch.hpp>
#endif

#include "Physics/Collision/CollisionSystem.h"

TEST_CASE( "Create collision box.  Success.", "[CollisionSystem][CreateCollisionBox]" )
{
    // CREATE THE COLLISION SYSTEM.
    PHYSICS::COLLISION::CollisionSystem collisionSystem;
    
    // CREATE A COLLISION BOX.
    // The position and dimensions of the box are arbitrary but chosen
    // to be representable as integers to allow for validation.
    const float WORLD_X_CENTER_POSITION = 5.0f;
    const float WORLD_Y_CENTER_POSITION = 12.0f;
    const float BOX_WIDTH = 8.0f;
    const float BOX_HEIGHT = 1.0f;
    std::shared_ptr<PHYSICS::COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
        WORLD_X_CENTER_POSITION,
        WORLD_Y_CENTER_POSITION,
        BOX_WIDTH,
        BOX_HEIGHT);

    // VERIFY THAT THE COLLIDER WAS CREATED.
    bool collisionBoxCreatedSuccessfully = (nullptr != boxCollider);
    REQUIRE( collisionBoxCreatedSuccessfully );

    // VERIFY THE COORDINATES OF THE BOX.
    hgeRect boundingBox = boxCollider->GetBoundingBox();
    // Coordinates will be converted to integers so that we can properly
    // compare the coordinates and ignore floating point issues.
    const float BOX_HALF_WIDTH = BOX_WIDTH / 2.0f;
    const float EXPECTED_TOP_LEFT_X_POSITION = WORLD_X_CENTER_POSITION - BOX_HALF_WIDTH;
    CHECK( static_cast<int>(EXPECTED_TOP_LEFT_X_POSITION) == static_cast<int>(boundingBox.x1) );

    const float BOX_HALF_HEIGHT = BOX_HEIGHT / 2.0f;
    // Up on the screen results in smaller Y values.
    const float EXPECTED_TOP_LEFT_Y_POSITION = WORLD_Y_CENTER_POSITION - BOX_HALF_HEIGHT;
    CHECK( static_cast<int>(EXPECTED_TOP_LEFT_Y_POSITION) == static_cast<int>(boundingBox.y1) );

    const float EXPECTED_BOTTOM_RIGHT_X_POSITION = WORLD_X_CENTER_POSITION + BOX_HALF_WIDTH;
    CHECK( static_cast<int>(EXPECTED_BOTTOM_RIGHT_X_POSITION) == static_cast<int>(boundingBox.x2) );

    const float EXPECTED_BOTTOM_RIGHT_Y_POSITION = WORLD_Y_CENTER_POSITION + BOX_HALF_HEIGHT;
    CHECK( static_cast<int>(EXPECTED_BOTTOM_RIGHT_Y_POSITION) == static_cast<int>(boundingBox.y2) );

    // VERIFY THAT THE COLLIDER IS MANAGED BY THE SYSTEM.
    bool collisionBoxManagedBySystem = collisionSystem.ManagesCollisionComponent(boxCollider);
    CHECK( collisionBoxManagedBySystem );
}

TEST_CASE( "Create collision box.  Failure due to zero width.", "[CollisionSystem][CreateCollisionBox]" )
{
    // CREATE THE COLLISION SYSTEM.
    PHYSICS::COLLISION::CollisionSystem collisionSystem;
    
    // CREATE A COLLISION BOX.
    // Other than the width, the other parameters for the box are arbitrary.
    const float WORLD_X_CENTER_POSITION = 5.0f;
    const float WORLD_Y_CENTER_POSITION = 12.0f;
    const float BOX_WIDTH = 0.0f;
    const float BOX_HEIGHT = 1.0f;
    std::shared_ptr<PHYSICS::COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
        WORLD_X_CENTER_POSITION,
        WORLD_Y_CENTER_POSITION,
        BOX_WIDTH,
        BOX_HEIGHT);

    // VERIFY THAT THE COLLIDER FAILED TO BE CREATED CREATED.
    bool collisionBoxCreatedSuccessfully = (nullptr != boxCollider);
    REQUIRE( !collisionBoxCreatedSuccessfully );
}

TEST_CASE( "Create collision box.  Failure due to zero height.", "[CollisionSystem][CreateCollisionBox]" )
{
    // CREATE THE COLLISION SYSTEM.
    PHYSICS::COLLISION::CollisionSystem collisionSystem;
    
    // CREATE A COLLISION BOX.
    // Other than the height, the other parameters for the box are arbitrary.
    const float WORLD_X_CENTER_POSITION = 5.0f;
    const float WORLD_Y_CENTER_POSITION = 12.0f;
    const float BOX_WIDTH = 8.0f;
    const float BOX_HEIGHT = 0.0f;
    std::shared_ptr<PHYSICS::COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
        WORLD_X_CENTER_POSITION,
        WORLD_Y_CENTER_POSITION,
        BOX_WIDTH,
        BOX_HEIGHT);

    // VERIFY THAT THE COLLIDER FAILED TO BE CREATED.
    bool collisionBoxCreatedSuccessfully = (nullptr != boxCollider);
    REQUIRE( !collisionBoxCreatedSuccessfully );
}

TEST_CASE( "Create collision box.  Failure due to negative width.", "[CollisionSystem][CreateCollisionBox]" )
{
    // CREATE THE COLLISION SYSTEM.
    PHYSICS::COLLISION::CollisionSystem collisionSystem;
    
    // CREATE A COLLISION BOX.
    // Other than the width, the other parameters for the box are arbitrary.
    const float WORLD_X_CENTER_POSITION = 5.0f;
    const float WORLD_Y_CENTER_POSITION = 12.0f;
    const float BOX_WIDTH = -8.0f;
    const float BOX_HEIGHT = 1.0f;
    std::shared_ptr<PHYSICS::COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
        WORLD_X_CENTER_POSITION,
        WORLD_Y_CENTER_POSITION,
        BOX_WIDTH,
        BOX_HEIGHT);

    // VERIFY THAT THE COLLIDER FAILED TO BE CREATED CREATED.
    bool collisionBoxCreatedSuccessfully = (nullptr != boxCollider);
    REQUIRE( !collisionBoxCreatedSuccessfully );
}

TEST_CASE( "Create collision box.  Failure due to negative height.", "[CollisionSystem][CreateCollisionBox]" )
{
    // CREATE THE COLLISION SYSTEM.
    PHYSICS::COLLISION::CollisionSystem collisionSystem;
    
    // CREATE A COLLISION BOX.
    // Other than the height, the other parameters for the box are arbitrary.
    const float WORLD_X_CENTER_POSITION = 5.0f;
    const float WORLD_Y_CENTER_POSITION = 12.0f;
    const float BOX_WIDTH = 8.0f;
    const float BOX_HEIGHT = -1.0f;
    std::shared_ptr<PHYSICS::COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
        WORLD_X_CENTER_POSITION,
        WORLD_Y_CENTER_POSITION,
        BOX_WIDTH,
        BOX_HEIGHT);

    // VERIFY THAT THE COLLIDER FAILED TO BE CREATED.
    bool collisionBoxCreatedSuccessfully = (nullptr != boxCollider);
    REQUIRE( !collisionBoxCreatedSuccessfully );
}
