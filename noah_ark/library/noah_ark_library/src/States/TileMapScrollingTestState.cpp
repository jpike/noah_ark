#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Maps/OverworldMapSpecification.h"
#include "Maps/TileMapBuilder.h"
#include "Maps/TileMapLoader.h"
#include "States/TileMapScrollingTestState.h"

using namespace STATES;

TileMapScrollingTestState::TileMapScrollingTestState(
    HGE* const pGameEngine,
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem) :
    m_camera(),
    m_scrollProcess(),
    m_graphicsSystem(graphicsSystem),
    m_inputController(pGameEngine),
    m_overworldSpec(),
    m_overworldMap(),
    m_surroundingMapLoader()
{
    // LOAD THE OVERWORLD FROM FILE.
    const std::string TEST_OVERWORLD_SPECIFICATION_FILEPATH = "res/maps/test_overworld_map.xml";
    bool overworldLoadedSuccessfully = LoadOverworldMap(TEST_OVERWORLD_SPECIFICATION_FILEPATH);
    if (!overworldLoadedSuccessfully)
    {
        std::stringstream errorMessage;
        errorMessage << "Error loading overworld for specification file: " << TEST_OVERWORLD_SPECIFICATION_FILEPATH << std::endl;
        throw std::runtime_error(errorMessage.str());
    }
}

TileMapScrollingTestState::~TileMapScrollingTestState()
{

}

bool TileMapScrollingTestState::Update(const float elapsedTimeInSeconds)
{
    // HANDLE USER INPUT.
    HandleUserInput(m_inputController);
    
    // SCROLL THE MAP IF SCROLLING IS STILL OCCURRING.
    bool scrollingActive = (nullptr != m_scrollProcess.get());
    if (scrollingActive)
    {
        // Scroll the map.
        MATH::Vector2f& currentScrollPosition = m_scrollProcess->Scroll(elapsedTimeInSeconds);
        m_camera.SetTopLeftViewPosition(currentScrollPosition);
        m_graphicsSystem->SetCamera(m_camera);

        // Check if scrolling has finished.
        bool scrollingFinished = m_scrollProcess->IsComplete();
        if (scrollingFinished)
        {            
            // Re-enable user input.
            m_inputController.EnableInput();

            // Load the next set of tile maps.
            UpdateSurroundingMapsAfterScrolling(m_scrollProcess->GetDirection());

            // Delete the scrolling process.
            m_scrollProcess.reset();
        }
    }

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
    // The starting tile map starts out at 0,0 in the world.  Other tile maps will be positioned relative to it.
    const MAPS::OverworldGridPosition STARTING_TILE_MAP_OVERWORLD_GRID_POSITION = m_overworldSpec.GetStartingTileMapPosition();
    const MATH::Vector2f STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION = MATH::Vector2f(0.0f, 0.0f);

    m_overworldMap.PopulateFromSpecification(
        m_overworldSpec,
        STARTING_TILE_MAP_OVERWORLD_GRID_POSITION,
        STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION,
        m_graphicsSystem);
    
    // SET THE CAMERA TO VIEW THE STARTING TILE MAP.
    m_camera.SetTopLeftViewPosition(STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION);

    // INITIALIZE THE SURROUNDING MAP LOADER.
    // This will allow surrounding tile maps to be loaded later during scrolling.
    m_surroundingMapLoader = std::unique_ptr<MAPS::SurroundingTileMapLoader>(
        new MAPS::SurroundingTileMapLoader(&m_overworldSpec));

    return true;
}

void TileMapScrollingTestState::HandleUserInput(const INPUT_CONTROL::IDebugInputController& inputController)
{
    // DEFINE SCROLLING PARAMETERS.
    const float MOVE_DISTANCE_IN_PIXELS = 2.0f;
    const float MAX_SCROLL_TIME_IN_SECONDS = 3.0f;

    // GET CURRENT MAP TOP-LEFT CORNER.
    // This is needed for all possible scrolling scenarios.
    std::shared_ptr<MAPS::TileMap> currentTileMap = m_overworldMap.GetCurrentTileMap();
    bool currentTileMapExists = (nullptr != currentTileMap);
    if (!currentTileMapExists)
    {
        // We can't scroll without knowledge of the current map.
        return;
    }
    MATH::Vector2f currentMapTopLeftPosition = currentTileMap->GetTopLeftWorldPosition();
    
    // SCROLL IN THE DIRECTION BASED ON USER INPUT.
    if (inputController.ScrollUpButtonPressed())
    {
        // CHECK IF A TOP MAP EXISTS.
        std::shared_ptr<MAPS::TileMap> topTileMap = m_overworldMap.GetTopTileMap();
        bool topTileMapExists = (nullptr != topTileMap);
        if (topTileMapExists)
        {
            // Get the top map's top-left corner.
            MATH::Vector2f topMapTopLeftPosition = topTileMap->GetTopLeftWorldPosition();

            // DISABLE USER INPUT WHILE SCROLLING IS OCCURRING.
            m_inputController.DisableInput();
            
            // CREATE SCROLL PROCESS.
            m_scrollProcess = std::unique_ptr<PROCESSES::ScrollProcess>(
                new PROCESSES::ScrollProcess(
                    PROCESSES::ScrollProcess::ScrollDirection::UP,
                    currentMapTopLeftPosition,
                    topMapTopLeftPosition,
                    MAX_SCROLL_TIME_IN_SECONDS));

            // SWITCH THE CURRENT MAP WITH THE TOP MAP.
            m_overworldMap.SetBottomTileMap(currentTileMap);
            m_overworldMap.SetCurrentTileMap(topTileMap);

            // KICK OFF LOADING THE NEW SURROUNDING TILE MAPS.
            // The new bottom tile map has already been loaded.
            MATH::Vector2ui newCurrentMapOverworldGridPosition = topTileMap->GetOverworldGridPosition();
            m_surroundingMapLoader->StartLoadingLeftTileMap(newCurrentMapOverworldGridPosition);
            m_surroundingMapLoader->StartLoadingRightTileMap(newCurrentMapOverworldGridPosition);
            m_surroundingMapLoader->StartLoadingTopTileMap(newCurrentMapOverworldGridPosition);

            // Return to prevent other key presses from interfering with scrolling.
            return;
        }
    }

    if (inputController.ScrollDownButtonPressed())
    {
        // CHECK IF A BOTTOM MAP EXISTS.
        std::shared_ptr<MAPS::TileMap> bottomTileMap = m_overworldMap.GetBottomTileMap();
        bool bottomTileMapExists = (nullptr != bottomTileMap);
        if (bottomTileMapExists)
        {
            // Get the bottom map's top-left corner.
            MATH::Vector2f bottomMapTopLeftPosition = bottomTileMap->GetTopLeftWorldPosition();

            // DISABLE USER INPUT WHILE SCROLLING IS OCCURRING.
            m_inputController.DisableInput();
            
            // CREATE SCROLL PROCESS.
            m_scrollProcess = std::unique_ptr<PROCESSES::ScrollProcess>(
                new PROCESSES::ScrollProcess(
                    PROCESSES::ScrollProcess::ScrollDirection::DOWN,
                    currentMapTopLeftPosition,
                    bottomMapTopLeftPosition,
                    MAX_SCROLL_TIME_IN_SECONDS));

            // SWITCH THE CURRENT MAP WITH THE BOTTOM MAP.
            m_overworldMap.SetTopTileMap(currentTileMap);
            m_overworldMap.SetCurrentTileMap(bottomTileMap);

            // KICK OFF LOADING THE NEW SURROUNDING TILE MAPS.
            // The new top tile map has already been loaded.
            MATH::Vector2ui newCurrentMapOverworldGridPosition = bottomTileMap->GetOverworldGridPosition();
            m_surroundingMapLoader->StartLoadingLeftTileMap(newCurrentMapOverworldGridPosition);
            m_surroundingMapLoader->StartLoadingRightTileMap(newCurrentMapOverworldGridPosition);
            m_surroundingMapLoader->StartLoadingBottomTileMap(newCurrentMapOverworldGridPosition);

            // Return to prevent other key presses from interfering with scrolling.
            return;
        }
    }

    if (inputController.ScrollLeftButtonPressed())
    {
        // CHECK IF A LEFT MAP EXISTS.
        std::shared_ptr<MAPS::TileMap> leftTileMap = m_overworldMap.GetLeftTileMap();
        bool leftTileMapExists = (nullptr != leftTileMap);
        if (leftTileMapExists)
        {
            // Get the left map's top-left corner.
            MATH::Vector2f leftMapTopLeftPosition = leftTileMap->GetTopLeftWorldPosition();

            // DISABLE USER INPUT WHILE SCROLLING IS OCCURRING.
            m_inputController.DisableInput();
            
            // CREATE SCROLL PROCESS.
            m_scrollProcess = std::unique_ptr<PROCESSES::ScrollProcess>(
                new PROCESSES::ScrollProcess(
                    PROCESSES::ScrollProcess::ScrollDirection::LEFT,
                    currentMapTopLeftPosition,
                    leftMapTopLeftPosition,
                    MAX_SCROLL_TIME_IN_SECONDS));

            // SWITCH THE CURRENT MAP WITH THE LEFT MAP.
            m_overworldMap.SetRightTileMap(currentTileMap);
            m_overworldMap.SetCurrentTileMap(leftTileMap);

            // KICK OFF LOADING THE NEW SURROUNDING TILE MAPS.
            // The new right tile map has already been loaded.
            MATH::Vector2ui newCurrentMapOverworldGridPosition = leftTileMap->GetOverworldGridPosition();
            m_surroundingMapLoader->StartLoadingLeftTileMap(newCurrentMapOverworldGridPosition);
            m_surroundingMapLoader->StartLoadingTopTileMap(newCurrentMapOverworldGridPosition);
            m_surroundingMapLoader->StartLoadingBottomTileMap(newCurrentMapOverworldGridPosition);

            // Return to prevent other key presses from interfering with scrolling.
            return;
        }
    }

    if (inputController.ScrollRightButtonPressed())
    {
        // CHECK IF A RIGHT MAP EXISTS.
        std::shared_ptr<MAPS::TileMap> rightTileMap = m_overworldMap.GetRightTileMap();
        bool rightTileMapExists = (nullptr != rightTileMap);
        if (rightTileMapExists)
        {
            // Get the right map's top-left corner.
            MATH::Vector2f rightMapTopLeftPosition = rightTileMap->GetTopLeftWorldPosition();

            // DISABLE USER INPUT WHILE SCROLLING IS OCCURRING.
            m_inputController.DisableInput();
            
            // CREATE SCROLL PROCESS.
            m_scrollProcess = std::unique_ptr<PROCESSES::ScrollProcess>(
                new PROCESSES::ScrollProcess(
                    PROCESSES::ScrollProcess::ScrollDirection::RIGHT,
                    currentMapTopLeftPosition,
                    rightMapTopLeftPosition,
                    MAX_SCROLL_TIME_IN_SECONDS));

            // SWITCH THE CURRENT MAP WITH THE RIGHT MAP.
            m_overworldMap.SetLeftTileMap(currentTileMap);
            m_overworldMap.SetCurrentTileMap(rightTileMap);

            // KICK OFF LOADING THE NEW SURROUNDING TILE MAPS.
            // The new left tile map has already been loaded.
            MATH::Vector2ui newCurrentMapOverworldGridPosition = rightTileMap->GetOverworldGridPosition();
            m_surroundingMapLoader->StartLoadingRightTileMap(newCurrentMapOverworldGridPosition);
            m_surroundingMapLoader->StartLoadingTopTileMap(newCurrentMapOverworldGridPosition);
            m_surroundingMapLoader->StartLoadingBottomTileMap(newCurrentMapOverworldGridPosition);

            // Return to prevent other key presses from interfering with scrolling.
            return;
        }
    }
}

void TileMapScrollingTestState::UpdateSurroundingMapsAfterScrolling(const PROCESSES::ScrollProcess::ScrollDirection direction)
{
    // VERIFY THAT A CURRENT CENTER MAP EXISTS.
    // This center map is required to make any updates.
    std::shared_ptr<MAPS::TileMap> centerMap = m_overworldMap.GetCurrentTileMap();
    bool centerMapExists = (nullptr != centerMap);
    if (!centerMapExists)
    {
        return;
    }
    
    // UPDATE THE MAPS DEPENDING ON THE DIRECTION.
    // If one of the valid directions wasn't specified, then we just won't
    // update the maps since we don't know what to do.
    switch (direction)
    {
        // The current and bottom maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::UP:
        {
            // Create the new top tile map.
            std::shared_ptr<MAPS::TileMap> newTopMap = MAPS::TileMapBuilder::BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetTopTileMap(newTopMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = MAPS::TileMapBuilder::BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetLeftTileMap(newLeftTileMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = MAPS::TileMapBuilder::BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetRightTileMap(newRightTileMap);
            break;
        }
        // The current and top maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::DOWN:
        {
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = MAPS::TileMapBuilder::BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetBottomTileMap(newBottomMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = MAPS::TileMapBuilder::BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetLeftTileMap(newLeftTileMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = MAPS::TileMapBuilder::BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetRightTileMap(newRightTileMap);
            break;
        }
        // The current and right maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::LEFT:
        {
            // Create the new top tile map.
            std::shared_ptr<MAPS::TileMap> newTopMap = MAPS::TileMapBuilder::BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetTopTileMap(newTopMap);
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = MAPS::TileMapBuilder::BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetBottomTileMap(newBottomMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = MAPS::TileMapBuilder::BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetLeftTileMap(newLeftTileMap);
            break;
        }
        // The current and left maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::RIGHT:
        {
            // Create the new top tile map.
            std::shared_ptr<MAPS::TileMap> newTopMap = MAPS::TileMapBuilder::BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetTopTileMap(newTopMap);
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = MAPS::TileMapBuilder::BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetBottomTileMap(newBottomMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = MAPS::TileMapBuilder::BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap(),
                m_graphicsSystem);
            m_overworldMap.SetRightTileMap(newRightTileMap);
            break;
        }
    }
}
