#pragma once

#include "Collision/CollisionSystem.h"

namespace COLLISION_SYSTEM_TESTS
{

    TEST_CASE( "Create collision box.  Success.", "[CollisionSystem][CreateCollisionBox]" )
    {
        // CREATE THE COLLISION SYSTEM.
        COLLISION::CollisionSystem collisionSystem;
    
        // CREATE A COLLISION BOX.
        // The position and dimensions of the box are arbitrary but chosen
        // to be representable as integers to allow for validation.
        const float WORLD_X_CENTER_POSITION = 5.0f;
        const float WORLD_Y_CENTER_POSITION = 12.0f;
        const float BOX_WIDTH = 8.0f;
        const float BOX_HEIGHT = 1.0f;
        std::shared_ptr<COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
            WORLD_X_CENTER_POSITION,
            WORLD_Y_CENTER_POSITION,
            BOX_WIDTH,
            BOX_HEIGHT);

        // VERIFY THAT THE COLLIDER WAS CREATED.
        bool collisionBoxCreatedSuccessfully = (nullptr != boxCollider);
        REQUIRE( collisionBoxCreatedSuccessfully );

        // VERIFY THE COORDINATES OF THE BOX.
        MATH::FloatRectangle boundingBox = boxCollider->GetBoundingBox();
        CHECK( WORLD_X_CENTER_POSITION == boundingBox.GetCenterXPosition() );
        CHECK( WORLD_Y_CENTER_POSITION == boundingBox.GetCenterYPosition() );
        CHECK( BOX_WIDTH == boundingBox.GetWidth() );
        CHECK( BOX_HEIGHT == boundingBox.GetHeight() );

        // VERIFY THAT THE COLLIDER IS MANAGED BY THE SYSTEM.
        bool collisionBoxManagedBySystem = collisionSystem.ManagesCollisionComponent(boxCollider);
        CHECK( collisionBoxManagedBySystem );
    }

    TEST_CASE( "Create collision box.  Failure due to zero width.", "[CollisionSystem][CreateCollisionBox]" )
    {
        // CREATE THE COLLISION SYSTEM.
        COLLISION::CollisionSystem collisionSystem;
    
        // CREATE A COLLISION BOX.
        // Other than the width, the other parameters for the box are arbitrary.
        const float WORLD_X_CENTER_POSITION = 5.0f;
        const float WORLD_Y_CENTER_POSITION = 12.0f;
        const float BOX_WIDTH = 0.0f;
        const float BOX_HEIGHT = 1.0f;
        std::shared_ptr<COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
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
        COLLISION::CollisionSystem collisionSystem;
    
        // CREATE A COLLISION BOX.
        // Other than the height, the other parameters for the box are arbitrary.
        const float WORLD_X_CENTER_POSITION = 5.0f;
        const float WORLD_Y_CENTER_POSITION = 12.0f;
        const float BOX_WIDTH = 8.0f;
        const float BOX_HEIGHT = 0.0f;
        std::shared_ptr<COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
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
        COLLISION::CollisionSystem collisionSystem;
    
        // CREATE A COLLISION BOX.
        // Other than the width, the other parameters for the box are arbitrary.
        const float WORLD_X_CENTER_POSITION = 5.0f;
        const float WORLD_Y_CENTER_POSITION = 12.0f;
        const float BOX_WIDTH = -8.0f;
        const float BOX_HEIGHT = 1.0f;
        std::shared_ptr<COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
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
        COLLISION::CollisionSystem collisionSystem;
    
        // CREATE A COLLISION BOX.
        // Other than the height, the other parameters for the box are arbitrary.
        const float WORLD_X_CENTER_POSITION = 5.0f;
        const float WORLD_Y_CENTER_POSITION = 12.0f;
        const float BOX_WIDTH = 8.0f;
        const float BOX_HEIGHT = -1.0f;
        std::shared_ptr<COLLISION::BoxCollider> boxCollider = collisionSystem.CreateBoxCollider(
            WORLD_X_CENTER_POSITION,
            WORLD_Y_CENTER_POSITION,
            BOX_WIDTH,
            BOX_HEIGHT);

        // VERIFY THAT THE COLLIDER FAILED TO BE CREATED.
        bool collisionBoxCreatedSuccessfully = (nullptr != boxCollider);
        REQUIRE( !collisionBoxCreatedSuccessfully );
    }

}