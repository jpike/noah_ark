#include <sstream>
#include <stdexcept>
#include "States/TileMapScrollingTestState.h"

using namespace STATES;

TileMapScrollingTestState::TileMapScrollingTestState(
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem) :
    m_camera(),
    m_graphicsSystem(graphicsSystem),
    m_inputController(),
    m_overworldSpec(),
    m_overworldMap(),
    m_scrollingOverworld(),
    m_noahPlayer(),
    Assets()
{
    // CREATE THE COLLECTION OF ASSETS.
    Assets = std::make_shared<RESOURCES::Assets>();

    // LOAD THE OVERWORLD FROM FILE.
    const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/test_overworld_map.json";
    bool overworldLoadedSuccessfully = LoadOverworldMap(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
    if (!overworldLoadedSuccessfully)
    {
        std::stringstream errorMessage;
        errorMessage << "Error loading overworld for specification file: " << TEST_OVERWORLD_SPECIFICATION_FILEPATH << std::endl;
        throw std::runtime_error(errorMessage.str());
    }

    // INITIALIZE THE PLAYER CHARACTER.
    InitializePlayer(m_noahPlayer);
}

TileMapScrollingTestState::~TileMapScrollingTestState()
{

}

bool TileMapScrollingTestState::Update(const float elapsedTimeInSeconds)
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

    // UPDATE ANY GRAPHICS THAT NEED TO CHANGE OVER TIME.
    m_graphicsSystem->Update(elapsedTimeInSeconds);

    // CONTINUE RUNNING THE GAME.
    return false;
}

bool TileMapScrollingTestState::LoadOverworldMap(const std::string& overworldSpecFilepath)
{
    // LOAD THE MAP SPECIFICATION.
    bool overworldSpecLoadedSuccessfully = m_overworldSpec.LoadFromFile(overworldSpecFilepath);
    if (!overworldSpecLoadedSuccessfully)
    {
        return false;
    }

    // LOAD THE OVERWORLD MAP.
    // Create a tile map builder needed for the creating tile maps.
    std::shared_ptr<MAPS::TileMapBuilder> tileMapBuilder = std::make_shared<MAPS::TileMapBuilder>(
        m_graphicsSystem,
        Assets);

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
        new MAPS::ScrollableOverworldMap(m_overworldMap, &m_overworldSpec, tileMapBuilder, m_graphicsSystem));
    
    // SET THE CAMERA TO VIEW THE STARTING TILE MAP.
    m_camera.SetTopLeftViewPosition(STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION);

    return true;
}

bool TileMapScrollingTestState::InitializePlayer(OBJECTS::Noah& noahPlayer)
{
    /// @todo   Add error checking to this method (i.e. for nullptrs).
    
    // LOAD THE ANIMATIONS FOR NOAH.
    // Load the animations for walking in each direction.
    std::shared_ptr<GRAPHICS::AnimationSequence> walkFrontAnimation = 
        m_graphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_FRONT_ANIMATION_NAME);
    std::shared_ptr<GRAPHICS::AnimationSequence> walkBackAnimation = 
        m_graphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_BACK_ANIMATION_NAME);
    std::shared_ptr<GRAPHICS::AnimationSequence> walkLeftAnimation = 
        m_graphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_LEFT_ANIMATION_NAME);
    std::shared_ptr<GRAPHICS::AnimationSequence> walkRightAnimation = 
        m_graphicsSystem->GetAnimationSequence(OBJECTS::Noah::WALK_RIGHT_ANIMATION_NAME);

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

    // POSITION NOAH IN THE WORLD.
    // For now, the position is arbitrary.
    const MATH::Vector2f NOAH_INITIAL_WORLD_POSITION(100.0f, 100.0f);
    noahSprite->SetWorldPosition(NOAH_INITIAL_WORLD_POSITION.X, NOAH_INITIAL_WORLD_POSITION.Y);

    // SET THE ANIMATED SPRITE FOR THE NOAH PLAYER.
    noahPlayer.SetSprite(noahSprite);

    // INITIALIZING THE PLAYER SUCCEEDED.
    return true;
}

void TileMapScrollingTestState::HandleUserInput(
    const INPUT_CONTROL::IDebugInputController& inputController,
    const float elapsedTimeInSeconds)
{
    // DEFINE SCROLLING PARAMETERS.
    const float MOVE_DISTANCE_IN_PIXELS = 2.0f;
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
    if (inputController.ScrollUpButtonPressed())
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

    if (inputController.ScrollDownButtonPressed())
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

    if (inputController.ScrollLeftButtonPressed())
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

    if (inputController.ScrollRightButtonPressed())
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
