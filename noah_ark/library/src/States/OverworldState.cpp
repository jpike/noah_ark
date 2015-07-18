#include <sstream>
#include <stdexcept>
#include "States/OverworldState.h"

namespace STATES
{
    OverworldState::OverworldState(
        std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system,
        std::shared_ptr<PHYSICS::COLLISION::CollisionSystem>& collision_system) :
        Camera(),
        Assets(),
        GraphicsSystem(graphics_system),
        InputController(),
        CollisionSystem(collision_system),
        OverworldSpec(),
        OverworldMap(),
        ScrollingOverworld(),
        NoahPlayer()
    {
        // VALIDATE THE POINTER MEMBERS.
        // Make sure a graphics system was provided.
        bool graphicsSystemExists = (nullptr != GraphicsSystem.get());
        if (!graphicsSystemExists)
        {
            throw std::runtime_error("Error creating overworld state: no graphics system provided.");
        }

        // Make sure a collision system was provided.
        bool collisionSystemExists = (nullptr != CollisionSystem.get());
        if (!collisionSystemExists)
        {
            throw std::runtime_error("Error creating overworld state: no collision system provided.");
        }

        // CREATE THE COLLECTION OF ASSETS.
        Assets = std::make_shared<RESOURCES::Assets>();

        // LOAD THE OVERWORLD FROM FILE.
        const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/overworld_map.json";
        bool overworldLoadedSuccessfully = LoadOverworldMap(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
        if (!overworldLoadedSuccessfully)
        {
            std::stringstream errorMessage;
            errorMessage << "Error loading overworld for specification file: " << TEST_OVERWORLD_SPECIFICATION_FILEPATH << std::endl;
            throw std::runtime_error(errorMessage.str());
        }

        // INITIALIZE THE PLAYER CHARACTER.
        bool playerInitializedSuccessfully = InitializePlayer(NoahPlayer);
        if (!playerInitializedSuccessfully)
        {
            std::stringstream errorMessage;
            errorMessage << "Error initializing Noah player character." << std::endl;
            throw std::runtime_error(errorMessage.str());
        }
    }

    OverworldState::~OverworldState()
    {

    }

    bool OverworldState::Update(const float elapsedTimeInSeconds)
    {
        // HANDLE USER INPUT.
        HandleUserInput(elapsedTimeInSeconds, InputController);

        // SCROLL THE MAP IF SCROLLING IS STILL OCCURRING.
        if (ScrollingOverworld->IsScrolling())
        {
            // Scroll the map.
            MATH::Vector2f& currentScrollPosition = ScrollingOverworld->Scroll(elapsedTimeInSeconds);
            Camera.SetTopLeftViewPosition(currentScrollPosition);
            GraphicsSystem->SetCamera(Camera);

            // Check if scrolling has finished.
            if (ScrollingOverworld->IsScrollingComplete())
            {
                // Re-enable user input.
                InputController.EnableInput();
            }
        }

        // UPDATE NOAH.
        NoahPlayer.Update(elapsedTimeInSeconds);

        // SIMULATE MOVEMENT AND HANDLE COLLISIONS.
        CollisionSystem->Update();

        // UPDATE ANY GRAPHICS THAT NEED TO CHANGE OVER TIME.
        GraphicsSystem->Update(elapsedTimeInSeconds);

        // CONTINUE RUNNING THE GAME.
        return false;
    }

    bool OverworldState::LoadOverworldMap(const std::string& overworldSpecFilepath)
    {
        // LOAD THE MAP SPECIFICATION.
        bool overworldSpecLoadedSuccessfully = OverworldSpec.LoadFromFile(overworldSpecFilepath);
        if (!overworldSpecLoadedSuccessfully)
        {
            return false;
        }

        // LOAD THE OVERWORLD MAP.
        // Create a tile map builder needed for the creating tile maps.
        std::shared_ptr<MAPS::TileMapBuilder> tileMapBuilder = std::make_shared<MAPS::TileMapBuilder>(
            GraphicsSystem,
            Assets);

        // The starting tile map starts out at 0,0 in the world.  Other tile maps will be positioned relative to it.
        const MAPS::OverworldGridPosition STARTING_TILE_MAP_OVERWORLD_GRID_POSITION = OverworldSpec.GetStartingTileMapPosition();
        const MATH::Vector2f STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION = MATH::Vector2f(0.0f, 0.0f);

        OverworldMap = std::make_shared<MAPS::OverworldMap>();

        OverworldMap->PopulateFromSpecification(
            OverworldSpec,
            STARTING_TILE_MAP_OVERWORLD_GRID_POSITION,
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION,
            tileMapBuilder);

        // Add the overworld map so that it can be rendered.
        std::shared_ptr<MAPS::TileMap> starting_tile_map = OverworldMap->GetCurrentTileMap();
        GraphicsSystem->AddGraphicsComponent(
            GRAPHICS::GraphicsLayer::GROUND,
            starting_tile_map);

        // Wrap the overworld map so that it can be scrolled.
        ScrollingOverworld = std::unique_ptr<MAPS::ScrollableOverworldMap>(
            new MAPS::ScrollableOverworldMap(OverworldMap, &OverworldSpec, tileMapBuilder, GraphicsSystem));

        // SET THE CAMERA TO VIEW THE STARTING TILE MAP.
        Camera.SetTopLeftViewPosition(STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION);

        // PROVIDE THE OVERWORLD MAP DATA TO THE COLLISION SYSTEM.
        CollisionSystem->SetOverworldMap(OverworldMap);

        return true;
    }

    bool OverworldState::InitializePlayer(OBJECTS::Noah& noahPlayer)
    {
        // LOAD THE ANIMATIONS FOR NOAH.
        // Load the animations for walking in each direction.
        std::shared_ptr<GRAPHICS::AnimationSequence> walkFrontAnimation =
            GraphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_FRONT_ANIMATION_NAME);
        bool walkFrontAnimationLoadedSuccessfully = (nullptr != walkFrontAnimation);
        if (!walkFrontAnimationLoadedSuccessfully)
        {
            return false;
        }

        std::shared_ptr<GRAPHICS::AnimationSequence> walkBackAnimation =
            GraphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_BACK_ANIMATION_NAME);
        bool walkBackAnimationLoadedSuccessfully = (nullptr != walkBackAnimation);
        if (!walkBackAnimationLoadedSuccessfully)
        {
            return false;
        }

        std::shared_ptr<GRAPHICS::AnimationSequence> walkLeftAnimation =
            GraphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_LEFT_ANIMATION_NAME);
        bool walkLeftAnimationLoadedSuccessfully = (nullptr != walkLeftAnimation);
        if (!walkLeftAnimationLoadedSuccessfully)
        {
            return false;
        }

        std::shared_ptr<GRAPHICS::AnimationSequence> walkRightAnimation =
            GraphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_RIGHT_ANIMATION_NAME);
        bool walkRightAnimationLoadedSuccessfully = (nullptr != walkRightAnimation);
        if (!walkRightAnimationLoadedSuccessfully)
        {
            return false;
        }

        // CREATE THE ANIMATED SPRITE FOR NOAH.
        const std::string NOAH_SPRITE_RESOURCE_NAME = "noah_sprite";
        std::shared_ptr<GRAPHICS::AnimatedSprite> noahSprite = GraphicsSystem->CreateAnimatedSprite(
            NOAH_SPRITE_RESOURCE_NAME,
            GRAPHICS::GraphicsLayer::PLAYER);

        // Add the animations to the sprite.
        noahSprite->AddAnimationSequence(OBJECTS::Noah::WALK_FRONT_ANIMATION_NAME, walkFrontAnimation);
        noahSprite->AddAnimationSequence(OBJECTS::Noah::WALK_BACK_ANIMATION_NAME, walkBackAnimation);
        noahSprite->AddAnimationSequence(OBJECTS::Noah::WALK_LEFT_ANIMATION_NAME, walkLeftAnimation);
        noahSprite->AddAnimationSequence(OBJECTS::Noah::WALK_RIGHT_ANIMATION_NAME, walkRightAnimation);

        // Set the initial animation sequence to have the player facing forward (downward).
        noahSprite->UseAnimationSequence(OBJECTS::Noah::WALK_FRONT_ANIMATION_NAME);

        // POSITION NOAH ON SCREEN.
        // The same position component will be used for the collision box to allowing
        // automatic synchronization of positions.
        std::shared_ptr<MATH::Vector2f> noahPositionComponent = std::make_shared<MATH::Vector2f>(300.0f, 100.0f);
        noahSprite->SetPositionComponent(noahPositionComponent);

        // SET THE ANIMATED SPRITE FOR THE NOAH PLAYER.
        noahPlayer.SetSprite(noahSprite);

        // CREATE THE AXE FOR NOAH.
        std::shared_ptr<OBJECTS::Axe> axe = OBJECTS::Axe::Create(*Assets);
        bool axe_created = (nullptr != axe);
        if (axe_created)
        {
            // GIVE NOAH THE AXE.
            noahPlayer.SetAxe(axe);

            // PROVIDE THE AXE SPRITE TO THE GRAPHICS SYSTEM FOR RENDERING.
            std::shared_ptr<GRAPHICS::IGraphicsComponent> axe_sprite = axe->GetSprite();
            GraphicsSystem->AddGraphicsComponent(
                GRAPHICS::GraphicsLayer::PLAYER,
                axe_sprite);
        }
        else
        {
            std::runtime_error("Failed to create axe for Noah.");
        }

        // CREATE THE COLLISION BOX FOR NOAH.
        MATH::FloatRectangle noahBoundingBox = noahSprite->GetWorldBoundingBox();
        std::shared_ptr<PHYSICS::COLLISION::BoxCollider> noahCollider = CollisionSystem->CreateBoxCollider(
            noahPositionComponent->X,
            noahPositionComponent->Y,
            noahBoundingBox.GetWidth(),
            noahBoundingBox.GetHeight());

        // POSITION NOAH IN THE WORLD.
        // The same position component as used for the sprite is used for the collision box to allowing
        // automatic synchronization of positions.
        noahCollider->SetPositionComponent(noahPositionComponent);

        // SET THE COLLISION BOX.
        noahPlayer.SetCollider(noahCollider);

        // INITIALIZING THE PLAYER SUCCEEDED.
        return true;
    }

    void OverworldState::HandleUserInput(
        const float elapsed_time_in_seconds,
        INPUT_CONTROL::KeyboardInputController& input_controller)
    {
        // SWING THE PLAYER'S AXE IF THE APPROPRIATE BUTTON WAS PRESSED.
        if (input_controller.PrimaryActionButtonPressed())
        {
            // A new axe swing may not be created if the player's
            // axe is already being swung.
            std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = NoahPlayer.SwingAxe();
            bool axe_swing_occurred = (nullptr != axe_swing);
            if (axe_swing_occurred)
            {
                // Allow the axe to collide with other objects.
                CollisionSystem->Add(axe_swing);
            }
        }

        // DEFINE SCROLLING PARAMETERS.
        const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
        const float MAX_SCROLL_TIME_IN_SECONDS = 3.0f;

        // GET CURRENT MAP TOP-LEFT CORNER.
        // This is needed for all possible scrolling scenarios.
        std::shared_ptr<MAPS::TileMap> currentTileMap = OverworldMap->GetCurrentTileMap();
        bool currentTileMapExists = (nullptr != currentTileMap);
        if (!currentTileMapExists)
        {
            // We can't scroll without knowledge of the current map.
            return;
        }
        MATH::Vector2f currentMapTopLeftPosition = currentTileMap->GetTopLeftWorldPosition();
        MATH::Vector2f currentMapBottomRightPosition = currentTileMap->GetBottomRightWorldPosition();

        // SCROLL IN THE DIRECTION BASED ON USER INPUT.
        if (input_controller.UpButtonPressed())
        {
            NoahPlayer.MoveUp(elapsed_time_in_seconds);

            // CHECK IF NOAH HAS EXCEEDED THE CURRENT MAP'S TOP BOUNDARY.
            MATH::Vector2f noahPosition = NoahPlayer.GetWorldPosition();
            bool noahMovedBeyondCurrentMapTopBoundary = (noahPosition.Y < currentMapTopLeftPosition.Y);
            if (!noahMovedBeyondCurrentMapTopBoundary)
            {
                // Noah hasn't moved beyond the top boundary, so we don't need to do anything.
                return;
            }

            // ATTEMPT TO START SCROLLING UP.
            bool scrollingUpSucceeded = ScrollingOverworld->BeginScrollingUp();
            if (scrollingUpSucceeded)
            {
                // Move Noah a few more pixels up so that he will be more visibile on the new map.
                noahPosition.Y -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                NoahPlayer.SetWorldPosition(noahPosition.X, noahPosition.Y);

                // Disable user input while scrolling is occurring.
                input_controller.DisableInput();

                // Return to prevent other key presses from interfering with scrolling.
                return;
            }
            else
            {
                // Ensure that Noah stays within the bounds of the current map.
                NoahPlayer.SetWorldPosition(noahPosition.X, currentMapTopLeftPosition.Y);
            }
        }

        if (input_controller.DownButtonPressed())
        {
            NoahPlayer.MoveDown(elapsed_time_in_seconds);

            // CHECK IF NOAH HAS EXCEEDED THE CURRENT MAP'S BOTTOM BOUNDARY.
            MATH::Vector2f noahPosition = NoahPlayer.GetWorldPosition();
            bool noahMovedBeyondCurrentMapBottomBoundary = (noahPosition.Y > currentMapBottomRightPosition.Y);
            if (!noahMovedBeyondCurrentMapBottomBoundary)
            {
                // Noah hasn't moved beyond the bottom boundary, so we don't need to do anything.
                return;
            }

            // ATTEMPT TO START SCROLLING DOWN.
            bool scrollingDownSucceeded = ScrollingOverworld->BeginScrollingDown();
            if (scrollingDownSucceeded)
            {
                // Move Noah a few more pixels down so that he will be more visibile on the new map.
                noahPosition.Y += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                NoahPlayer.SetWorldPosition(noahPosition.X, noahPosition.Y);

                // Disable user input while scrolling is occurring.
                input_controller.DisableInput();

                // Return to prevent other key presses from interfering with scrolling.
                return;
            }
            else
            {
                // Ensure that Noah stays within the bounds of the current map.
                NoahPlayer.SetWorldPosition(noahPosition.X, currentMapBottomRightPosition.Y);
            }
        }

        if (input_controller.LeftButtonPressed())
        {
            NoahPlayer.MoveLeft(elapsed_time_in_seconds);

            // CHECK IF NOAH HAS EXCEEDED THE CURRENT MAP'S LEFT BOUNDARY.
            MATH::Vector2f noahPosition = NoahPlayer.GetWorldPosition();
            bool noahMovedBeyondCurrentMapLeftBoundary = (noahPosition.X < currentMapTopLeftPosition.X);
            if (!noahMovedBeyondCurrentMapLeftBoundary)
            {
                // Noah hasn't moved beyond the left boundary, so we don't need to do anything.
                return;
            }

            // ATTEMPT TO START SCROLLING LEFT.
            bool scrollingLeftSucceeded = ScrollingOverworld->BeginScrollingLeft();
            if (scrollingLeftSucceeded)
            {
                // Move Noah a few more pixels left so that he will be more visibile on the new map.
                noahPosition.X -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                NoahPlayer.SetWorldPosition(noahPosition.X, noahPosition.Y);

                // Disable user input while scrolling is occurring.
                input_controller.DisableInput();

                // Return to prevent other key presses from interfering with scrolling.
                return;
            }
            else
            {
                // Ensure that Noah stays within the bounds of the current map.
                NoahPlayer.SetWorldPosition(currentMapTopLeftPosition.X, noahPosition.Y);
            }
        }

        if (input_controller.RightButtonPressed())
        {
            NoahPlayer.MoveRight(elapsed_time_in_seconds);

            // CHECK IF NOAH HAS EXCEEDED THE CURRENT MAP'S RIGHT BOUNDARY.
            MATH::Vector2f noahPosition = NoahPlayer.GetWorldPosition();
            bool noahMovedBeyondCurrentMapRightBoundary = (noahPosition.X > currentMapBottomRightPosition.X);
            if (!noahMovedBeyondCurrentMapRightBoundary)
            {
                // Noah hasn't moved beyond the right boundary, so we don't need to do anything.
                return;
            }

            // ATTEMPT TO START SCROLLING RIGHT.
            bool scrollingRightSucceeded = ScrollingOverworld->BeginScrollingRight();
            if (scrollingRightSucceeded)
            {
                // Move Noah a few more pixels right so that he will be more visibile on the new map.
                noahPosition.X += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
                NoahPlayer.SetWorldPosition(noahPosition.X, noahPosition.Y);

                // Disable user input while scrolling is occurring.
                input_controller.DisableInput();

                // Return to prevent other key presses from interfering with scrolling.
                return;
            }
            else
            {
                // Ensure that Noah stays within the bounds of the current map.
                NoahPlayer.SetWorldPosition(currentMapBottomRightPosition.X, noahPosition.Y);
            }
        }

        // If we reach this point, no buttons were pressed, so stop playing Noah's current animation.
        NoahPlayer.Idle();
    }
}