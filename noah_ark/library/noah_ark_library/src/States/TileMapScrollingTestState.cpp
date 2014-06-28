#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Maps/OverworldMapSpecification.h"
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

    // INITIALIZE THE SURROUNDING MAP LOADER.
    m_surroundingMapLoader = std::unique_ptr<MAPS::SurroundingTileMapLoader>(
        new MAPS::SurroundingTileMapLoader(&m_overworldSpec));
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
            std::cout << "Scrolling finished." << std::endl;
            
            // Re-enable user input.
            m_inputController.EnableInput();

            // Load the next set of tile maps.
            //LoadSurroundingMapsAfterScrolling(m_scrollProcess->GetStartPoint(), m_scrollProcess->GetEndPoint());
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

    // CREATE THE OVERWORLD FROM THE SPECIFICATION.
    MAPS::TileMapLoader mapLoader;

    // Load the initial current tile map.
    // It starts out at 0,0 in the world.  Other tile maps will be positioned relative to it.
    const MATH::Vector2ui STARTING_TILE_MAP_OVERWORLD_GRID_POSITION = m_overworldSpec.GetStartingTileMapPosition();
    const MATH::Vector2f STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION = MATH::Vector2f(0.0f, 0.0f);
    m_camera.SetTopLeftViewPosition(STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION);
    std::shared_ptr<Tmx::Map> startingTmxMap = mapLoader.LoadMap(m_overworldSpec.GetStartingTileMapFilepath());
    std::shared_ptr<MAPS::TileMap> startingTileMap = std::make_shared<MAPS::TileMap>(
        STARTING_TILE_MAP_OVERWORLD_GRID_POSITION,
        STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION,
        startingTmxMap, 
        m_graphicsSystem);
    m_overworldMap.SetCurrentTileMap(startingTileMap);

    // Load the top tile map.
    MATH::Vector2ui startingTileMapPosition = m_overworldSpec.GetStartingTileMapPosition();
    bool topTileMapExists = (startingTileMapPosition.Y > 0);
    if (topTileMapExists)
    {
        // Get the filepath for the top tile map.
        MATH::Vector2ui topTileMapPosition = startingTileMapPosition;
        topTileMapPosition.Y--;
        std::string topTileMapFilepath = m_overworldSpec.GetTileMapFilepath(
            topTileMapPosition.Y,
            topTileMapPosition.X);

        // Create the top tile map in the overworld.
        std::shared_ptr<Tmx::Map> topTmxMap = mapLoader.LoadMap(topTileMapFilepath);

        // Calculate the grid position of the top tile map.
        MATH::Vector2ui topTileMapOverworldGridPosition = STARTING_TILE_MAP_OVERWORLD_GRID_POSITION;
        topTileMapOverworldGridPosition.Y--;

        // Calculate the world position of the top map.
        float topMapHeightInTiles = static_cast<float>(topTmxMap->GetHeight());
        float topMapTileHeightInPixels = static_cast<float>(topTmxMap->GetTileHeight());
        float topMapHeightInPixels = topMapHeightInTiles * topMapTileHeightInPixels;
        float topMapWorldYPosition = STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.Y - topMapHeightInPixels;
        MATH::Vector2f topTileMapTopLeftWorldPosition(
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.X,
            topMapWorldYPosition);

        std::shared_ptr<MAPS::TileMap> topTileMap = std::make_shared<MAPS::TileMap>(
            topTileMapOverworldGridPosition,
            topTileMapTopLeftWorldPosition,
            topTmxMap,
            m_graphicsSystem);
        m_overworldMap.SetTopTileMap(topTileMap);
    }

    // Load the bottom tile map.
    MATH::Vector2ui bottomTileMapPosition = startingTileMapPosition;
    bottomTileMapPosition.Y++;
    bool bottomTileMapExists = (bottomTileMapPosition.Y < m_overworldSpec.GetOverworldHeightInTileMaps());
    if (bottomTileMapExists)
    {
        // Get the filepath for the bottom tile map.
        std::string bottomTileMapFilepath = m_overworldSpec.GetTileMapFilepath(
            bottomTileMapPosition.Y,
            bottomTileMapPosition.X);

        // Create the bottom tile map in the overworld.
        std::shared_ptr<Tmx::Map> bottomTmxMap = mapLoader.LoadMap(bottomTileMapFilepath);

        // Calculate the grid position of the bottom tile map.
        MATH::Vector2ui bottomTileMapOverworldGridPosition = STARTING_TILE_MAP_OVERWORLD_GRID_POSITION;
        bottomTileMapOverworldGridPosition.Y++;

        // Calculate the world position of the bottom map.
        float centerMapHeightInTiles = static_cast<float>(startingTmxMap->GetHeight());
        float centerMapTileHeightInPixels = static_cast<float>(startingTmxMap->GetTileHeight());
        float centerMapHeightInPixels = centerMapHeightInTiles * centerMapTileHeightInPixels;
        float bottomMapWorldYPosition = STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.Y + centerMapHeightInPixels;
        MATH::Vector2f bottomTileMapTopLeftWorldPosition(
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.X,
            bottomMapWorldYPosition);

        std::shared_ptr<MAPS::TileMap> bottomTileMap = std::make_shared<MAPS::TileMap>(
            bottomTileMapOverworldGridPosition,
            bottomTileMapTopLeftWorldPosition,
            bottomTmxMap,
            m_graphicsSystem);
        m_overworldMap.SetBottomTileMap(bottomTileMap);
    }

    // Load the left tile map.
    bool leftTileMapExists = (startingTileMapPosition.X > 0);
    if (leftTileMapExists)
    {
        // Get the filepath for the left tile map.
        MATH::Vector2ui leftTileMapPosition = startingTileMapPosition;
        leftTileMapPosition.X--;
        std::string leftTileMapFilepath = m_overworldSpec.GetTileMapFilepath(
            leftTileMapPosition.Y,
            leftTileMapPosition.X);

        // Create the left tile map in the overworld.
        std::shared_ptr<Tmx::Map> leftTmxMap = mapLoader.LoadMap(leftTileMapFilepath);

        // Calculate the grid position of the left tile map.
        MATH::Vector2ui leftTileMapOverworldGridPosition = STARTING_TILE_MAP_OVERWORLD_GRID_POSITION;
        leftTileMapOverworldGridPosition.X--;

        // Calculate the world position of the left map.
        float leftMapWidthInTiles = static_cast<float>(leftTmxMap->GetWidth());
        float leftMapTileWidthInPixels = static_cast<float>(leftTmxMap->GetTileWidth());
        float leftMapWidthInPixels = leftMapWidthInTiles * leftMapTileWidthInPixels;
        float leftMapWorldXPosition = STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.X - leftMapWidthInPixels;
        MATH::Vector2f leftTileMapTopLeftWorldPosition(
            leftMapWorldXPosition,
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.Y);

        std::shared_ptr<MAPS::TileMap> leftTileMap = std::make_shared<MAPS::TileMap>(
            leftTileMapOverworldGridPosition,
            leftTileMapTopLeftWorldPosition,
            leftTmxMap,
            m_graphicsSystem);
        m_overworldMap.SetLeftTileMap(leftTileMap);
    }

    // Load the right tile map.
    MATH::Vector2ui rightTileMapPosition = startingTileMapPosition;
    rightTileMapPosition.X++;
    bool rightTileMapExists = (rightTileMapPosition.X < m_overworldSpec.GetOverworldWidthInTileMaps());
    if (rightTileMapExists)
    {
        // Get the filepath for the right tile map.
        std::string rightTileMapFilepath = m_overworldSpec.GetTileMapFilepath(
            rightTileMapPosition.Y,
            rightTileMapPosition.X);

        // Create the right tile map in the overworld.
        std::shared_ptr<Tmx::Map> rightTmxMap = mapLoader.LoadMap(rightTileMapFilepath);

        // Calculate the grid position of the right tile map.
        MATH::Vector2ui rightTileMapOverworldGridPosition = STARTING_TILE_MAP_OVERWORLD_GRID_POSITION;
        rightTileMapOverworldGridPosition.X++;

        // Calculate the world position of the left map.
        float centerMapWidthInTiles = static_cast<float>(startingTmxMap->GetWidth());
        float centerMapTileWidthInPixels = static_cast<float>(startingTmxMap->GetTileWidth());
        float centerMapWidthInPixels = centerMapWidthInTiles * centerMapTileWidthInPixels;
        float rightMapWorldXPosition = STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.X + centerMapWidthInPixels;
        MATH::Vector2f rightTileMapTopLeftWorldPosition(
            rightMapWorldXPosition,
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.Y);

        std::shared_ptr<MAPS::TileMap> rightTileMap = std::make_shared<MAPS::TileMap>(
            rightTileMapOverworldGridPosition,
            rightTileMapTopLeftWorldPosition,
            rightTmxMap,
            m_graphicsSystem);
        m_overworldMap.SetRightTileMap(rightTileMap);
    }

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
        std::cout << "Scroll Up!" << std::endl;
        //m_camera.MoveUp(MOVE_DISTANCE_IN_PIXELS);
        //m_graphicsSystem->SetCamera(m_camera);

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
        std::cout << "Scroll Down!" << std::endl;
        //m_camera.MoveDown(MOVE_DISTANCE_IN_PIXELS);
        //m_graphicsSystem->SetCamera(m_camera);

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
        std::cout << "Scroll Left!" << std::endl;
        //m_camera.MoveLeft(MOVE_DISTANCE_IN_PIXELS);
        //m_graphicsSystem->SetCamera(m_camera);

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
        std::cout << "Scroll Right!" << std::endl;
        //m_camera.MoveRight(MOVE_DISTANCE_IN_PIXELS);
        //m_graphicsSystem->SetCamera(m_camera);

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
            std::shared_ptr<MAPS::TileMap> newTopMap = BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap());
            m_overworldMap.SetTopTileMap(newTopMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap());
            m_overworldMap.SetLeftTileMap(newLeftTileMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap());
            m_overworldMap.SetRightTileMap(newRightTileMap);
            break;
        }
        // The current and top maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::DOWN:
        {
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap());
            m_overworldMap.SetBottomTileMap(newBottomMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap());
            m_overworldMap.SetLeftTileMap(newLeftTileMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap());
            m_overworldMap.SetRightTileMap(newRightTileMap);
            break;
        }
        // The current and right maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::LEFT:
        {
            // Create the new top tile map.
            std::shared_ptr<MAPS::TileMap> newTopMap = BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap());
            m_overworldMap.SetTopTileMap(newTopMap);
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap());
            m_overworldMap.SetBottomTileMap(newBottomMap);
            // Create the new left tile map.
            std::shared_ptr<MAPS::TileMap> newLeftTileMap = BuildLeftTileMap(
                *centerMap,
                m_surroundingMapLoader->GetLeftTileMap());
            m_overworldMap.SetLeftTileMap(newLeftTileMap);
            break;
        }
        // The current and left maps have already been set.
        case PROCESSES::ScrollProcess::ScrollDirection::RIGHT:
        {
            // Create the new top tile map.
            std::shared_ptr<MAPS::TileMap> newTopMap = BuildTopTileMap(
                *centerMap,
                m_surroundingMapLoader->GetTopTileMap());
            m_overworldMap.SetTopTileMap(newTopMap);
            // Create the new bottom tile map.
            std::shared_ptr<MAPS::TileMap> newBottomMap = BuildBottomTileMap(
                *centerMap,
                m_surroundingMapLoader->GetBottomTileMap());
            m_overworldMap.SetBottomTileMap(newBottomMap);
            // Create the new right tile map.
            std::shared_ptr<MAPS::TileMap> newRightTileMap = BuildRightTileMap(
                *centerMap,
                m_surroundingMapLoader->GetRightTileMap());
            m_overworldMap.SetRightTileMap(newRightTileMap);
            break;
        }
    }
}

std::shared_ptr<MAPS::TileMap> TileMapScrollingTestState::BuildTopTileMap(
    const MAPS::TileMap& centerMap,
    const std::shared_ptr<Tmx::Map>& topTmxMap)
{
    // CHECK IF A TOP TMX MAP WAS PROVIDED.
    bool topTmxMapExists = (nullptr != topTmxMap);
    if (!topTmxMapExists)
    {
        // No tile map can be constructed without a TMX map.
        return nullptr;
    }

    // CALCULATE THE GRID POSITION OF THE TOP MAP.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerMap.GetOverworldGridPosition();
    MATH::Vector2ui topTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    topTileMapOverworldGridPosition.Y--;

    // CALCULATE THE WORLD POSITION OF THE TOP MAP.
    float topMapHeightInTiles = static_cast<float>(topTmxMap->GetHeight());
    float topMapTileHeightInPixels = static_cast<float>(topTmxMap->GetTileHeight());
    float topMapHeightInPixels = topMapHeightInTiles * topMapTileHeightInPixels;
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerMap.GetTopLeftWorldPosition();
    float topMapWorldYPosition = centerTileMapTopLeftWorldPosition.Y - topMapHeightInPixels;
    MATH::Vector2f topTileMapTopLeftWorldPosition(
        centerTileMapTopLeftWorldPosition.X,
        topMapWorldYPosition);

    // CREATE THE TOP TILE MAP.
    std::shared_ptr<MAPS::TileMap> topTileMap = std::make_shared<MAPS::TileMap>(
        topTileMapOverworldGridPosition,
        topTileMapTopLeftWorldPosition,
        topTmxMap,
        m_graphicsSystem);
    return topTileMap;
}

std::shared_ptr<MAPS::TileMap> TileMapScrollingTestState::BuildBottomTileMap(
    const MAPS::TileMap& centerMap,
    const std::shared_ptr<Tmx::Map>& bottomTmxMap)
{
    // CHECK IF A BOTTOM TMX MAP WAS PROVIDED.
    bool bottomTmxMapExists = (nullptr != bottomTmxMap);
    if (!bottomTmxMapExists)
    {
        // No tile map can be constructed without a TMX map.
        return nullptr;
    }

    // CALCULATE THE GRID POSITION OF THE BOTTOM MAP.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerMap.GetOverworldGridPosition();
    MATH::Vector2ui bottomTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    bottomTileMapOverworldGridPosition.Y++;

    // CALCULATE THE WORLD POSITION OF THE BOTTOM MAP.
    float centerMapHeightInTiles = static_cast<float>(centerMap.GetHeightInTiles());
    float centerMapTileHeightInPixels = static_cast<float>(centerMap.GetTileHeightInPixels());
    float centerMapHeightInPixels = centerMapHeightInTiles * centerMapTileHeightInPixels;
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerMap.GetTopLeftWorldPosition();
    float bottomMapWorldYPosition = centerTileMapTopLeftWorldPosition.Y + centerMapHeightInPixels;
    MATH::Vector2f bottomTileMapTopLeftWorldPosition(
        centerTileMapTopLeftWorldPosition.X,
        bottomMapWorldYPosition);

    // CREATE THE BOTTOM TILE MAP.
    std::shared_ptr<MAPS::TileMap> bottomTileMap = std::make_shared<MAPS::TileMap>(
        bottomTileMapOverworldGridPosition,
        bottomTileMapTopLeftWorldPosition,
        bottomTmxMap,
        m_graphicsSystem);
    return bottomTileMap;
}

std::shared_ptr<MAPS::TileMap> TileMapScrollingTestState::BuildLeftTileMap(
    const MAPS::TileMap& centerMap,
    const std::shared_ptr<Tmx::Map>& leftTmxMap)
{
    // CHECK IF A LEFT TMX MAP WAS PROVIDED.
    bool leftTmxMapExists = (nullptr != leftTmxMap);
    if (!leftTmxMapExists)
    {
        // No tile map can be constructed without a TMX map.
        return nullptr;
    }

    // CALCULATE THE GRID POSITION OF THE LEFT MAP.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerMap.GetOverworldGridPosition();
    MATH::Vector2ui leftTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    leftTileMapOverworldGridPosition.X--;

    // CALCULATE THE WORLD POSITION OF THE LEFT MAP.
    float leftMapWidthInTiles = static_cast<float>(leftTmxMap->GetWidth());
    float leftMapTileWidthInPixels = static_cast<float>(leftTmxMap->GetTileWidth());
    float leftMapWidthInPixels = leftMapWidthInTiles * leftMapTileWidthInPixels;
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerMap.GetTopLeftWorldPosition();
    float leftMapWorldXPosition = centerTileMapTopLeftWorldPosition.X - leftMapWidthInPixels;
    MATH::Vector2f leftTileMapTopLeftWorldPosition(
        leftMapWorldXPosition,
        centerTileMapTopLeftWorldPosition.Y);

    // CREATE THE LEFT TILE MAP.
    std::shared_ptr<MAPS::TileMap> leftTileMap = std::make_shared<MAPS::TileMap>(
        leftTileMapOverworldGridPosition,
        leftTileMapTopLeftWorldPosition,
        leftTmxMap,
        m_graphicsSystem);
    return leftTileMap;
}

std::shared_ptr<MAPS::TileMap> TileMapScrollingTestState::BuildRightTileMap(
    const MAPS::TileMap& centerMap,
    const std::shared_ptr<Tmx::Map>& rightTmxMap)
{
    // CHECK IF A RIGHT TMX MAP WAS PROVIDED.
    bool rightTmxMapExists = (nullptr != rightTmxMap);
    if (!rightTmxMapExists)
    {
        // No tile map can be constructed without a TMX map.
        return nullptr;
    }

    // CALCULATE THE GRID POSITION OF THE RIGHT MAP.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerMap.GetOverworldGridPosition();
    MATH::Vector2ui rightTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
    rightTileMapOverworldGridPosition.X++;

    // CALCULATE THE WORLD POSITION OF THE LEFT MAP.
    float centerMapWidthInTiles = static_cast<float>(centerMap.GetWidthInTiles());
    float centerMapTileWidthInPixels = static_cast<float>(centerMap.GetTileWidthInPixels());
    float centerMapWidthInPixels = centerMapWidthInTiles * centerMapTileWidthInPixels;
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerMap.GetTopLeftWorldPosition();
    float rightMapWorldXPosition = centerTileMapTopLeftWorldPosition.X + centerMapWidthInPixels;
    MATH::Vector2f rightTileMapTopLeftWorldPosition(
        rightMapWorldXPosition,
        centerTileMapTopLeftWorldPosition.Y);

    // CREATE THE RIGHT TILE MAP.
    std::shared_ptr<MAPS::TileMap> rightTileMap = std::make_shared<MAPS::TileMap>(
        rightTileMapOverworldGridPosition,
        rightTileMapTopLeftWorldPosition,
        rightTmxMap,
        m_graphicsSystem);
    return rightTileMap;
}

void TileMapScrollingTestState::LoadSurroundingMapsAfterScrolling(const MATH::Vector2f& scrollStartPoint, const MATH::Vector2f& scrollEndPoint)
{
    // CREATE VARIABLES TO HOLD THE NEW MAPS FOR THE OVERWORLD.
    std::shared_ptr<MAPS::TileMap> newCurrentMap;
    std::shared_ptr<MAPS::TileMap> newLeftMap;
    std::shared_ptr<MAPS::TileMap> newRightMap;
    std::shared_ptr<MAPS::TileMap> newTopMap;
    std::shared_ptr<MAPS::TileMap> newBottomMap;
    
    // DETERMINE WHICH DIRECTION WE JUST SCROLLED.
    float horizontalScrollDistance = (scrollEndPoint.X - scrollStartPoint.X);
    float verticalScrollDistance = (scrollEndPoint.Y - scrollStartPoint.Y);
    bool scrolledLeft = (horizontalScrollDistance < 0.0f);
    bool scrolledRight = (horizontalScrollDistance > 0.0f);
    bool scrolledUp = (verticalScrollDistance < 0.0f);
    bool scrolledDown = (verticalScrollDistance > 0.0f);

    // DETERMINE THE NEW SET OF MAPS FOR THE OVERWORLD BASED ON THE SCROLLED DIRECTION.
    if (scrolledLeft)
    {
        // The old left map has become the current map.
        newCurrentMap = m_overworldMap.GetLeftTileMap();
        bool newCurrentMapExists = (nullptr != newCurrentMap);
        if (!newCurrentMapExists)
        {
            // Return early since we can't update the maps if a new current map doesn't exist.
            return;
        }
    }
    else if (scrolledRight)
    {
        // The old right map has become the current map.
        newCurrentMap = m_overworldMap.GetRightTileMap();
        bool newCurrentMapExists = (nullptr != newCurrentMap);
        if (!newCurrentMapExists)
        {
            // Return early since we can't update the maps if a new current map doesn't exist.
            return;
        }
    }
    else if (scrolledUp)
    {
        // The old top map has become the current map.
        newCurrentMap = m_overworldMap.GetTopTileMap();
        bool newCurrentMapExists = (nullptr != newCurrentMap);
        if (!newCurrentMapExists)
        {
            // Return early since we can't update the maps if a new current map doesn't exist.
            return;
        }
    }
    else if (scrolledDown)
    {
        // The old bottom map has become the current map.
        newCurrentMap = m_overworldMap.GetBottomTileMap();
    }
    else
    {
        // Return early since we don't have to update the overworld maps if we didn't scroll.
        return;
    }

    // VERIFY THAT A NEW CURRENT MAP EXISTS.
    bool newCurrentMapExists = (nullptr != newCurrentMap);
    if (!newCurrentMapExists)
    {
        // Return early since we can't update the maps if a new current map doesn't exist.
        return;
    }

    // LOAD THE MAPS SURROUNDING THE NEW CURRENT MAP.
    LoadSurroundingMaps(
        newCurrentMap,
        newTopMap,
        newBottomMap,
        newLeftMap,
        newRightMap);

    // UPDATE THE OVERWORLD MAP'S TILE MAPS.
    m_overworldMap.SetCurrentTileMap(newCurrentMap);
    m_overworldMap.SetLeftTileMap(newLeftMap);
    m_overworldMap.SetRightTileMap(newRightMap);
    m_overworldMap.SetTopTileMap(newTopMap);
    m_overworldMap.SetBottomTileMap(newBottomMap);
}

void TileMapScrollingTestState::LoadSurroundingMaps(
    const std::shared_ptr<const MAPS::TileMap>& centerTileMap,
    std::shared_ptr<MAPS::TileMap>& topTileMap,
    std::shared_ptr<MAPS::TileMap>& bottomTileMap,
    std::shared_ptr<MAPS::TileMap>& leftTileMap,
    std::shared_ptr<MAPS::TileMap>& rightTileMap)
{
    // RESET THE OUT PARAMETERS.
    topTileMap.reset();
    bottomTileMap.reset();
    leftTileMap.reset();
    rightTileMap.reset();
    
    // Get the center tile map's positions.
    MATH::Vector2ui centerTileMapOverworldGridPosition = centerTileMap->GetOverworldGridPosition();
    MATH::Vector2f centerTileMapTopLeftWorldPosition = centerTileMap->GetTopLeftWorldPosition();

    /// @todo   Clean-up and refactor this code so that it is shorter and so that most of this logic isn't duplicated.

    // Load the top tile map.
    MAPS::TileMapLoader mapLoader;
    bool topTileMapExists = (centerTileMapOverworldGridPosition.Y > 0);
    if (topTileMapExists)
    {
        // Get the filepath for the top tile map.
        MATH::Vector2ui topTileMapPosition = centerTileMapOverworldGridPosition;
        topTileMapPosition.Y--;
        topTileMapExists = (
            topTileMapPosition.X < m_overworldSpec.GetOverworldWidthInTileMaps() &&
            topTileMapPosition.Y < m_overworldSpec.GetOverworldHeightInTileMaps());
        if (topTileMapExists)
        {
            std::string topTileMapFilepath = m_overworldSpec.GetTileMapFilepath(
                topTileMapPosition.Y,
                topTileMapPosition.X);

            // Create the top tile map in the overworld.
            std::shared_ptr<Tmx::Map> topTmxMap = mapLoader.LoadMap(topTileMapFilepath);

            // Calculate the grid position of the top tile map.
            MATH::Vector2ui topTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
            topTileMapOverworldGridPosition.Y--;

            // Calculate the world position of the top map.
            float topMapHeightInTiles = static_cast<float>(topTmxMap->GetHeight());
            float topMapTileHeightInPixels = static_cast<float>(topTmxMap->GetTileHeight());
            float topMapHeightInPixels = topMapHeightInTiles * topMapTileHeightInPixels;
            float topMapWorldYPosition = centerTileMapTopLeftWorldPosition.Y - topMapHeightInPixels;
            MATH::Vector2f topTileMapTopLeftWorldPosition(
                centerTileMapTopLeftWorldPosition.X,
                topMapWorldYPosition);

            topTileMap = std::make_shared<MAPS::TileMap>(
                topTileMapOverworldGridPosition,
                topTileMapTopLeftWorldPosition,
                topTmxMap,
                m_graphicsSystem);
        }
    }

    // Load the bottom tile map.
    MATH::Vector2ui bottomTileMapPosition = centerTileMapOverworldGridPosition;
    bottomTileMapPosition.Y++;
    bool bottomTileMapExists = (bottomTileMapPosition.Y < m_overworldSpec.GetOverworldHeightInTileMaps());
    if (bottomTileMapExists)
    {
        bottomTileMapExists = (
            bottomTileMapPosition.X < m_overworldSpec.GetOverworldWidthInTileMaps() &&
            bottomTileMapPosition.Y < m_overworldSpec.GetOverworldHeightInTileMaps());
        if (bottomTileMapExists)
        {
        
            // Get the filepath for the bottom tile map.
            std::string bottomTileMapFilepath = m_overworldSpec.GetTileMapFilepath(
                bottomTileMapPosition.Y,
                bottomTileMapPosition.X);

            // Create the bottom tile map in the overworld.
            std::shared_ptr<Tmx::Map> bottomTmxMap = mapLoader.LoadMap(bottomTileMapFilepath);

            // Calculate the grid position of the bottom tile map.
            MATH::Vector2ui bottomTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
            bottomTileMapOverworldGridPosition.Y++;

            // Calculate the world position of the bottom map.
            float centerMapHeightInTiles = static_cast<float>(centerTileMap->GetHeightInTiles());
            float centerMapTileHeightInPixels = static_cast<float>(centerTileMap->GetTileHeightInPixels());
            float centerMapHeightInPixels = centerMapHeightInTiles * centerMapTileHeightInPixels;
            float bottomMapWorldYPosition = centerTileMapTopLeftWorldPosition.Y + centerMapHeightInPixels;
            MATH::Vector2f bottomTileMapTopLeftWorldPosition(
                centerTileMapTopLeftWorldPosition.X,
                bottomMapWorldYPosition);

            bottomTileMap = std::make_shared<MAPS::TileMap>(
                bottomTileMapOverworldGridPosition,
                bottomTileMapTopLeftWorldPosition,
                bottomTmxMap,
                m_graphicsSystem);
        }
    }

    // Load the left tile map.
    bool leftTileMapExists = (centerTileMapOverworldGridPosition.X > 0);
    if (leftTileMapExists)
    {
        // Get the filepath for the left tile map.
        MATH::Vector2ui leftTileMapPosition = centerTileMapOverworldGridPosition;
        leftTileMapPosition.X--;
        leftTileMapExists = (
            leftTileMapPosition.X < m_overworldSpec.GetOverworldWidthInTileMaps() &&
            leftTileMapPosition.Y < m_overworldSpec.GetOverworldHeightInTileMaps());
        if (leftTileMapExists)
        {

            std::string leftTileMapFilepath = m_overworldSpec.GetTileMapFilepath(
                leftTileMapPosition.Y,
                leftTileMapPosition.X);

            // Create the left tile map in the overworld.
            std::shared_ptr<Tmx::Map> leftTmxMap = mapLoader.LoadMap(leftTileMapFilepath);

            // Calculate the grid position of the left tile map.
            MATH::Vector2ui leftTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
            leftTileMapOverworldGridPosition.X--;

            // Calculate the world position of the left map.
            float leftMapWidthInTiles = static_cast<float>(leftTmxMap->GetWidth());
            float leftMapTileWidthInPixels = static_cast<float>(leftTmxMap->GetTileWidth());
            float leftMapWidthInPixels = leftMapWidthInTiles * leftMapTileWidthInPixels;
            float leftMapWorldXPosition = centerTileMapTopLeftWorldPosition.X - leftMapWidthInPixels;
            MATH::Vector2f leftTileMapTopLeftWorldPosition(
                leftMapWorldXPosition,
                centerTileMapTopLeftWorldPosition.Y);

            leftTileMap = std::make_shared<MAPS::TileMap>(
                leftTileMapOverworldGridPosition,
                leftTileMapTopLeftWorldPosition,
                leftTmxMap,
                m_graphicsSystem);
        }
    }

    // Load the right tile map.
    MATH::Vector2ui rightTileMapPosition = centerTileMapOverworldGridPosition;
    rightTileMapPosition.X++;
    bool rightTileMapExists = (rightTileMapPosition.X < m_overworldSpec.GetOverworldWidthInTileMaps());
    if (rightTileMapExists)
    {
        rightTileMapExists = (
            rightTileMapPosition.X < m_overworldSpec.GetOverworldWidthInTileMaps() &&
            rightTileMapPosition.Y < m_overworldSpec.GetOverworldHeightInTileMaps());
        if (rightTileMapExists)
        {
            // Get the filepath for the right tile map.
            std::string rightTileMapFilepath = m_overworldSpec.GetTileMapFilepath(
                rightTileMapPosition.Y,
                rightTileMapPosition.X);

            // Create the right tile map in the overworld.
            std::shared_ptr<Tmx::Map> rightTmxMap = mapLoader.LoadMap(rightTileMapFilepath);

            // Calculate the grid position of the right tile map.
            MATH::Vector2ui rightTileMapOverworldGridPosition = centerTileMapOverworldGridPosition;
            rightTileMapOverworldGridPosition.X++;

            // Calculate the world position of the left map.
            float centerMapWidthInTiles = static_cast<float>(centerTileMap->GetWidthInTiles());
            float centerMapTileWidthInPixels = static_cast<float>(centerTileMap->GetTileWidthInPixels());
            float centerMapWidthInPixels = centerMapWidthInTiles * centerMapTileWidthInPixels;
            float rightMapWorldXPosition = centerTileMapTopLeftWorldPosition.X + centerMapWidthInPixels;
            MATH::Vector2f rightTileMapTopLeftWorldPosition(
                rightMapWorldXPosition,
                centerTileMapTopLeftWorldPosition.Y);

            rightTileMap = std::make_shared<MAPS::TileMap>(
                rightTileMapOverworldGridPosition,
                rightTileMapTopLeftWorldPosition,
                rightTmxMap,
                m_graphicsSystem);
        }
    }
}