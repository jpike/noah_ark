#pragma once

#include <stdexcept>
#include "Graphics/GraphicsSystem.h"
#include "ResourcesTests/ResourceManagerTestConstants.h"

/// A namespace for testing the GraphicsSystem class.
namespace TEST_GRAPHICS_SYSTEM
{
    TEST_CASE( "GraphicsSystem constructor.  Both null parameters.", "[GraphicsSystem][Constructor][Fail]" )
    {
        // CREATE PARAMETERS FOR CONSTRUCTION.
        const std::shared_ptr<sf::RenderTarget> NO_RENDER_TARGET;
        const std::shared_ptr<RESOURCES::ResourceManager> NO_RESOURCE_MANAGER;

        // TRY CREATING A GRAPHICS SYSTEM.
        // We want to verify that an exception is thrown.
        bool exceptionThrown = false;
        try
        {
            GRAPHICS::GraphicsSystem graphicsSystem(NO_RENDER_TARGET, NO_RESOURCE_MANAGER);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }

        // VERIFY THAT THE EXPECTED EXCEPTION WAS THROWN.
        REQUIRE(exceptionThrown);
    }

    TEST_CASE( "GraphicsSystem constructor.  Null render target.", "[GraphicsSystem][Constructor][Fail]" )
    {
        // CREATE PARAMETERS FOR CONSTRUCTION.
        const std::shared_ptr<sf::RenderTarget> NO_RENDER_TARGET;
        std::shared_ptr<RESOURCES::ResourceManager> resourceManager = std::make_shared<RESOURCES::ResourceManager>(
            TEST_RESOURCE_MANAGER::TEST_RESOURCES_FILEPATH);

        // TRY CREATING A GRAPHICS SYSTEM.
        // We want to verify that an exception is thrown.
        bool exceptionThrown = false;
        try
        {
            GRAPHICS::GraphicsSystem graphicsSystem(NO_RENDER_TARGET, resourceManager);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }

        // VERIFY THAT THE EXPECTED EXCEPTION WAS THROWN.
        REQUIRE(exceptionThrown);
    }

    TEST_CASE( "GraphicsSystem constructor.  Null resource manager.", "[GraphicsSystem][Constructor][Fail]" )
    {
        // CREATE PARAMETERS FOR CONSTRUCTION.
        std::shared_ptr<sf::RenderTarget> renderTarget = std::make_shared<sf::RenderTexture>();
        const std::shared_ptr<RESOURCES::ResourceManager> NO_RESOURCE_MANAGER;

        // TRY CREATING A GRAPHICS SYSTEM.
        // We want to verify that an exception is thrown.
        bool exceptionThrown = false;
        try
        {
            GRAPHICS::GraphicsSystem graphicsSystem(renderTarget, NO_RESOURCE_MANAGER);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }

        // VERIFY THAT THE EXPECTED EXCEPTION WAS THROWN.
        REQUIRE(exceptionThrown);
    }

    TEST_CASE( "GraphicsSystem constructor.  Valid parameters.", "[GraphicsSystem][Constructor][Success]" )
    {
        // CREATE PARAMETERS FOR CONSTRUCTION.
        std::shared_ptr<sf::RenderTarget> renderTarget = std::make_shared<sf::RenderTexture>();
        std::shared_ptr<RESOURCES::ResourceManager> resourceManager = std::make_shared<RESOURCES::ResourceManager>(
            TEST_RESOURCE_MANAGER::TEST_RESOURCES_FILEPATH);

        // TRY CREATING A GRAPHICS SYSTEM.
        // We want to verify that an exception not thrown.
        bool exceptionThrown = false;
        try
        {
            GRAPHICS::GraphicsSystem graphicsSystem(renderTarget, resourceManager);
        }
        catch (const std::invalid_argument&)
        {
            exceptionThrown = true;
        }

        // VERIFY THAT THE EXPECTED EXCEPTION WAS NOT THROWN.
        REQUIRE(!exceptionThrown);
    }
    
    TEST_CASE( "Change camera for GraphicsSystem.", "[GraphicsSystem][GetCamera][SetCamera]" )
    {
        // CREATE THE GRAPHICS SYSTEM.
        std::shared_ptr<sf::RenderTarget> renderTarget = std::make_shared<sf::RenderTexture>();
        std::shared_ptr<RESOURCES::ResourceManager> resourceManager = std::make_shared<RESOURCES::ResourceManager>(
            TEST_RESOURCE_MANAGER::TEST_RESOURCES_FILEPATH);
        GRAPHICS::GraphicsSystem graphicsSystem(renderTarget, resourceManager);

        // VERIFY THE DEFAULT CAMERA CREATED DURING CONSTRUCTION.
        // It should be equal to a default camera.
        const GRAPHICS::Camera EXPECTED_DEFAULT_CAMERA;
        const float EXPECTED_DEFAULT_LEFT_VIEW_POSITION = EXPECTED_DEFAULT_CAMERA.GetLeftViewPosition();
        const float EXPECTED_DEFAULT_TOP_VIEW_POSITION = EXPECTED_DEFAULT_CAMERA.GetTopViewPosition();

        GRAPHICS::Camera actualDefaultCamera = graphicsSystem.GetCamera();
        float actualDefaultLeftViewPosition = actualDefaultCamera.GetLeftViewPosition();
        float actualDefaultTopViewPosition = actualDefaultCamera.GetTopViewPosition();

        // Values are cast to integers during comparison to avoid floating-point rounding issues.
        REQUIRE( static_cast<int>(EXPECTED_DEFAULT_LEFT_VIEW_POSITION) == static_cast<int>(actualDefaultLeftViewPosition) );
        REQUIRE( static_cast<int>(EXPECTED_DEFAULT_TOP_VIEW_POSITION) == static_cast<int>(actualDefaultTopViewPosition) );
        
        // SET A DIFFERENT CAMERA IN THE GRAPHICS SYSTEM.
        const float CHANGED_LEFT_VIEW_POSITION = EXPECTED_DEFAULT_LEFT_VIEW_POSITION + 24.0f;
        const float CHANGED_TOP_VIEW_POSITION = EXPECTED_DEFAULT_TOP_VIEW_POSITION - 91.0f;
        GRAPHICS::Camera newCamera(CHANGED_LEFT_VIEW_POSITION, CHANGED_TOP_VIEW_POSITION);
        graphicsSystem.SetCamera(newCamera);

        // VERIFY THAT THE CAMERA WAS PROPERLY CHANGED.
        GRAPHICS::Camera actualChangedCamera = graphicsSystem.GetCamera();
        float actualChangedLeftViewPosition = actualChangedCamera.GetLeftViewPosition();
        float actualChangedTopViewPosition = actualChangedCamera.GetTopViewPosition();

        // Values are cast to integers during comparison to avoid floating-point rounding issues.
        REQUIRE( static_cast<int>(CHANGED_LEFT_VIEW_POSITION) == static_cast<int>(actualChangedLeftViewPosition) );
        REQUIRE( static_cast<int>(CHANGED_TOP_VIEW_POSITION) == static_cast<int>(actualChangedTopViewPosition) );
    }

    TEST_CASE( "Successfully create sprite from GraphicsSystem.", "[GraphicsSystem][CreateSprite][Success]" )
    {
        // CREATE THE GRAPHICS SYSTEM.
        std::shared_ptr<sf::RenderTarget> renderTarget = std::make_shared<sf::RenderTexture>();
        std::shared_ptr<RESOURCES::ResourceManager> resourceManager = std::make_shared<RESOURCES::ResourceManager>(
            TEST_RESOURCE_MANAGER::TEST_RESOURCES_FILEPATH);
        GRAPHICS::GraphicsSystem graphicsSystem(renderTarget, resourceManager);

        // CREATE A TEXTURE FOR THE SPRITE.
        std::shared_ptr<sf::Texture> textureResource = std::make_shared<sf::Texture>();
        std::shared_ptr<GRAPHICS::Texture> texture = std::make_shared<GRAPHICS::Texture>(textureResource);

        // CREATE A SPRITE.
        // The exact values are arbitrary.
        const float TEST_X_POSITION = 456.0f;
        const float TEST_Y_POSITION = 34.0f;
        const float TEST_WIDTH = 22.0f;
        const float TEST_HEIGHT = 67.0f;
        const GRAPHICS::GraphicsLayer TEST_GRAPHICS_LAYER = GRAPHICS::GraphicsLayer::GROUND;
        std::shared_ptr<GRAPHICS::Sprite> sprite = graphicsSystem.CreateSprite(
            texture,
            TEST_X_POSITION,
            TEST_Y_POSITION,
            TEST_WIDTH,
            TEST_HEIGHT,
            TEST_GRAPHICS_LAYER);

        // VERIFY THE SPRITE WAS SUCCESSFULLY CREATED.
        REQUIRE( (nullptr != sprite) );

        // VERIFY THE DIMENSIONS OF THE SPRITE.
        MATH::FloatRectangle actualSpriteBoundingBox = sprite->GetBoundingBox();

        float actualWidth = actualSpriteBoundingBox.GetWidth();
        REQUIRE( static_cast<int>(TEST_WIDTH) == static_cast<int>(actualWidth) );

        float actualHeight = actualSpriteBoundingBox.GetHeight();
        REQUIRE( static_cast<int>(TEST_HEIGHT) == static_cast<int>(actualHeight) );

        /// @todo   Add additional validation.
    }
}