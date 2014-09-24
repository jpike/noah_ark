#include <sstream>
#include <stdexcept>
#include "States/OverworldState.h"

using namespace STATES;

OverworldState::OverworldState(
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem,
    std::shared_ptr<PHYSICS::COLLISION::CollisionSystem>& collisionSystem) :
    m_camera(),
    m_graphicsSystem(graphicsSystem),
    m_inputController(),
    m_collisionSystem(collisionSystem),
    m_overworldSpec(),
    m_overworldMap(),
    m_scrollingOverworld(),
    m_noahPlayer()
{
    // VALIDATE THE POINTER MEMBERS.
    // Make sure a graphics system was provided.
    bool graphicsSystemExists = (nullptr != m_graphicsSystem.get());
    if (!graphicsSystemExists)
    {
        throw std::runtime_error("Error creating overworld state: no graphics system provided.");
    }

    // Make sure a collision system was provided.
    bool collisionSystemExists = (nullptr != m_collisionSystem.get());
    if (!collisionSystemExists)
    {
        throw std::runtime_error("Error creating overworld state: no collision system provided.");
    }
    
    // LOAD THE OVERWORLD FROM FILE.
    const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/overworld_map.xml";
    bool overworldLoadedSuccessfully = LoadOverworldMap(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
    if (!overworldLoadedSuccessfully)
    {
        std::stringstream errorMessage;
        errorMessage << "Error loading overworld for specification file: " << TEST_OVERWORLD_SPECIFICATION_FILEPATH << std::endl;
        throw std::runtime_error(errorMessage.str());
    }

    // INITIALIZE THE PLAYER CHARACTER.
    bool playerInitializedSuccessfully = InitializePlayer(m_noahPlayer);
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
    HandleUserInput(m_inputController, elapsedTimeInSeconds);
    
    // SCROLL THE MAP IF SCROLLING IS STILL OCCURRING.
    if (m_scrollingOverworld->IsScrolling())
    {
        // Scroll the map.
        MATH::Vector2f& currentScrollPosition = m_scrollingOverworld->Scroll(elapsedTimeInSeconds);
        m_camera.SetTopLeftViewPosition(currentScrollPosition);
        m_graphicsSystem->SetCamera(m_camera);

        // Check if scrolling has finished.
        if (m_scrollingOverworld->IsScrollingComplete())
        {            
            // Re-enable user input.
            m_inputController.EnableInput();
        }
    }

    // SIMULATE MOVEMENT AND HANDLE COLLISIONS.
    m_collisionSystem->SimulateMovement();

    // UPDATE ANY GRAPHICS THAT NEED TO CHANGE OVER TIME.
    m_graphicsSystem->Update(elapsedTimeInSeconds);

    // CONTINUE RUNNING THE GAME.
    return false;
}

bool OverworldState::LoadOverworldMap(const std::string& overworldSpecFilepath)
{
    // LOAD THE MAP SPECIFICATION.
    bool overworldSpecLoadedSuccessfully = m_overworldSpec.LoadFromFile(overworldSpecFilepath);
    if (!overworldSpecLoadedSuccessfully)
    {
        return false;
    }

    // LOAD THE OVERWORLD MAP.
    // Create a tile map builder needed for the creating tile maps.
    std::shared_ptr<MAPS::TileMapBuilder> tileMapBuilder = std::make_shared<MAPS::TileMapBuilder>(m_graphicsSystem);

    // The starting tile map starts out at 0,0 in the world.  Other tile maps will be positioned relative to it.
    const MAPS::OverworldGridPosition STARTING_TILE_MAP_OVERWORLD_GRID_POSITION = m_overworldSpec.GetStartingTileMapPosition();
    const MATH::Vector2f STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION = MATH::Vector2f(0.0f, 0.0f);

    m_overworldMap = std::make_shared<MAPS::OverworldMap>();

    m_overworldMap->PopulateFromSpecification(
        m_overworldSpec,
        STARTING_TILE_MAP_OVERWORLD_GRID_POSITION,
        STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION,
        tileMapBuilder);

    // Wrap the overworld map so that it can be scrolled.
    m_scrollingOverworld = std::unique_ptr<MAPS::ScrollableOverworldMap>(
        new MAPS::ScrollableOverworldMap(m_overworldMap, &m_overworldSpec, tileMapBuilder));
    
    // SET THE CAMERA TO VIEW THE STARTING TILE MAP.
    m_camera.SetTopLeftViewPosition(STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION);

    // PROVIDE THE OVERWORLD MAP DATA TO THE COLLISION SYSTEM.
    m_collisionSystem->SetOverworldMap(m_overworldMap);

    return true;
}

bool OverworldState::InitializePlayer(OBJECTS::Noah& noahPlayer)
{    
    // LOAD THE ANIMATIONS FOR NOAH.
    // Load the animations for walking in each direction.
    std::shared_ptr<GRAPHICS::AnimationSequence> walkFrontAnimation = 
        m_graphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_FRONT_ANIMATION_NAME);
    bool walkFrontAnimationLoadedSuccessfully = (nullptr != walkFrontAnimation);
    if (!walkFrontAnimationLoadedSuccessfully)
    {
        return false;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walkBackAnimation = 
        m_graphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_BACK_ANIMATION_NAME);
    bool walkBackAnimationLoadedSuccessfully = (nullptr != walkBackAnimation);
    if (!walkBackAnimationLoadedSuccessfully)
    {
        return false;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walkLeftAnimation = 
        m_graphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_LEFT_ANIMATION_NAME);
    bool walkLeftAnimationLoadedSuccessfully = (nullptr != walkLeftAnimation);
    if (!walkLeftAnimationLoadedSuccessfully)
    {
        return false;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> walkRightAnimation = 
        m_graphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_RIGHT_ANIMATION_NAME);
    bool walkRightAnimationLoadedSuccessfully = (nullptr != walkRightAnimation);
    if (!walkRightAnimationLoadedSuccessfully)
    {
        return false;
    }

    // CREATE THE ANIMATED SPRITE FOR NOAH.
    const std::string NOAH_SPRITE_RESOURCE_NAME = "noah_sprite";
    std::shared_ptr<GRAPHICS::AnimatedSprite> noahSprite = m_graphicsSystem->CreateAnimatedSprite(
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

    // CREATE THE COLLISION BOX FOR NOAH.
    MATH::FloatRectangle noahBoundingBox = noahSprite->GetWorldBoundingBox();
    std::shared_ptr<PHYSICS::COLLISION::BoxCollider> noahCollider = m_collisionSystem->CreateBoxCollider(
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
    const INPUT_CONTROL::KeyboardInputController& inputController,
    const float elapsedTimeInSeconds)
{
    // DEFINE SCROLLING PARAMETERS.
    const float PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS = 8.0f;
    const float MAX_SCROLL_TIME_IN_SECONDS = 3.0f;

    // GET CURRENT MAP TOP-LEFT CORNER.
    // This is needed for all possible scrolling scenarios.
    std::shared_ptr<MAPS::TileMap> currentTileMap = m_overworldMap->GetCurrentTileMap();
    bool currentTileMapExists = (nullptr != currentTileMap);
    if (!currentTileMapExists)
    {
        // We can't scroll without knowledge of the current map.
        return;
    }
    MATH::Vector2f currentMapTopLeftPosition = currentTileMap->GetTopLeftWorldPosition();
    MATH::Vector2f currentMapBottomRightPosition = currentTileMap->GetBottomRightWorldPosition();
    
    // SCROLL IN THE DIRECTION BASED ON USER INPUT.
    if (inputController.UpButtonPressed())
    {
        m_noahPlayer.MoveUp(elapsedTimeInSeconds);

        // CHECK IF NOAH HAS EXCEEDED THE CURRENT MAP'S TOP BOUNDARY.
        MATH::Vector2f noahPosition = m_noahPlayer.GetWorldPosition();
        bool noahMovedBeyondCurrentMapTopBoundary = (noahPosition.Y < currentMapTopLeftPosition.Y);
        if (!noahMovedBeyondCurrentMapTopBoundary)
        {
            // Noah hasn't moved beyond the top boundary, so we don't need to do anything.
            return;
        }

        // ATTEMPT TO START SCROLLING UP.
        bool scrollingUpSucceeded = m_scrollingOverworld->BeginScrollingUp();
        if (scrollingUpSucceeded)
        {
            // Move Noah a few more pixels up so that he will be more visibile on the new map.
            noahPosition.Y -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
            m_noahPlayer.SetWorldPosition(noahPosition.X, noahPosition.Y);

            // Disable user input while scrolling is occurring.
            m_inputController.DisableInput();

            // Return to prevent other key presses from interfering with scrolling.
            return;
        }
        else
        {
            // Ensure that Noah stays within the bounds of the current map.
            m_noahPlayer.SetWorldPosition(noahPosition.X, currentMapTopLeftPosition.Y);
        }
    }

    if (inputController.DownButtonPressed())
    {
        m_noahPlayer.MoveDown(elapsedTimeInSeconds);

        // CHECK IF NOAH HAS EXCEEDED THE CURRENT MAP'S BOTTOM BOUNDARY.
        MATH::Vector2f noahPosition = m_noahPlayer.GetWorldPosition();
        bool noahMovedBeyondCurrentMapBottomBoundary = (noahPosition.Y > currentMapBottomRightPosition.Y);
        if (!noahMovedBeyondCurrentMapBottomBoundary)
        {
            // Noah hasn't moved beyond the bottom boundary, so we don't need to do anything.
            return;
        }

        // ATTEMPT TO START SCROLLING DOWN.
        bool scrollingDownSucceeded = m_scrollingOverworld->BeginScrollingDown();
        if (scrollingDownSucceeded)
        {
            // Move Noah a few more pixels down so that he will be more visibile on the new map.
            noahPosition.Y += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
            m_noahPlayer.SetWorldPosition(noahPosition.X, noahPosition.Y);

            // Disable user input while scrolling is occurring.
            m_inputController.DisableInput();

            // Return to prevent other key presses from interfering with scrolling.
            return;
        }
        else
        {
            // Ensure that Noah stays within the bounds of the current map.
            m_noahPlayer.SetWorldPosition(noahPosition.X, currentMapBottomRightPosition.Y);
        }
    }

    if (inputController.LeftButtonPressed())
    {
        m_noahPlayer.MoveLeft(elapsedTimeInSeconds);

        // CHECK IF NOAH HAS EXCEEDED THE CURRENT MAP'S LEFT BOUNDARY.
        MATH::Vector2f noahPosition = m_noahPlayer.GetWorldPosition();
        bool noahMovedBeyondCurrentMapLeftBoundary = (noahPosition.X < currentMapTopLeftPosition.X);
        if (!noahMovedBeyondCurrentMapLeftBoundary)
        {
            // Noah hasn't moved beyond the left boundary, so we don't need to do anything.
            return;
        }

        // ATTEMPT TO START SCROLLING LEFT.
        bool scrollingLeftSucceeded = m_scrollingOverworld->BeginScrollingLeft();
        if (scrollingLeftSucceeded)
        {
            // Move Noah a few more pixels left so that he will be more visibile on the new map.
            noahPosition.X -= PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
            m_noahPlayer.SetWorldPosition(noahPosition.X, noahPosition.Y);

            // Disable user input while scrolling is occurring.
            m_inputController.DisableInput();

            // Return to prevent other key presses from interfering with scrolling.
            return;
        }
        else
        {
            // Ensure that Noah stays within the bounds of the current map.
            m_noahPlayer.SetWorldPosition(currentMapTopLeftPosition.X, noahPosition.Y);
        }
    }

    if (inputController.RightButtonPressed())
    {
        m_noahPlayer.MoveRight(elapsedTimeInSeconds);

        // CHECK IF NOAH HAS EXCEEDED THE CURRENT MAP'S RIGHT BOUNDARY.
        MATH::Vector2f noahPosition = m_noahPlayer.GetWorldPosition();
        bool noahMovedBeyondCurrentMapRightBoundary = (noahPosition.X > currentMapBottomRightPosition.X);
        if (!noahMovedBeyondCurrentMapRightBoundary)
        {
            // Noah hasn't moved beyond the right boundary, so we don't need to do anything.
            return;
        }

        // ATTEMPT TO START SCROLLING RIGHT.
        bool scrollingRightSucceeded = m_scrollingOverworld->BeginScrollingRight();
        if (scrollingRightSucceeded)
        {
            // Move Noah a few more pixels right so that he will be more visibile on the new map.
            noahPosition.X += PLAYER_POSITION_ADJUSTMENT_FOR_SCROLLING_IN_PIXELS;
            m_noahPlayer.SetWorldPosition(noahPosition.X, noahPosition.Y);

            // Disable user input while scrolling is occurring.
            m_inputController.DisableInput();

            // Return to prevent other key presses from interfering with scrolling.
            return;
        }
        else
        {
            // Ensure that Noah stays within the bounds of the current map.
            m_noahPlayer.SetWorldPosition(currentMapBottomRightPosition.X, noahPosition.Y);
        }
    }

    // If we reach this point, no buttons were pressed, so stop playing Noah's current animation.
    m_noahPlayer.Idle();
}