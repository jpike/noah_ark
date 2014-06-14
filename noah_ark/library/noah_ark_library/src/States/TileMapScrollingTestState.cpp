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
    m_graphicsSystem(graphicsSystem),
    m_inputController(pGameEngine),
    m_overworldMap()
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

bool TileMapScrollingTestState::Update()
{
    // HANDLE USER INPUT.
    HandleUserInput(m_inputController);
    
    // CONTINUE RUNNING THE GAME.
    return false;
}

bool TileMapScrollingTestState::LoadOverworldMap(const std::string& overworldSpecFilepath)
{
    // LOAD THE MAP SPECIFICATION.
    MAPS::OverworldMapSpecification overworldSpec;
    bool overworldSpecLoadedSuccessfully = overworldSpec.LoadFromFile(overworldSpecFilepath);
    if (!overworldSpecLoadedSuccessfully)
    {
        return false;
    }

    // CREATE THE OVERWORLD FROM THE SPECIFICATION.
    MAPS::TileMapLoader mapLoader;

    // Load the initial current tile map.
    // It starts out at 0,0 in the world.  Other tile maps will be positioned relative to it.
    const MATH::Vector2f STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION = MATH::Vector2f(0.0f, 0.0f);
    m_camera.SetTopLeftViewPosition(STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION);
    std::shared_ptr<Tmx::Map> startingTmxMap = mapLoader.LoadMap(overworldSpec.GetStartingTileMapFilepath());
    std::shared_ptr<MAPS::TileMap> startingTileMap = std::make_shared<MAPS::TileMap>(
        STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION,
        startingTmxMap, 
        m_graphicsSystem);
    m_overworldMap.SetCurrentTileMap(startingTileMap);

    // Load the top tile map.
    MATH::Vector2ui startingTileMapPosition = overworldSpec.GetStartingTileMapPosition();
    bool topTileMapExists = (startingTileMapPosition.Y > 0);
    if (topTileMapExists)
    {
        // Get the filepath for the top tile map.
        MATH::Vector2ui topTileMapPosition = startingTileMapPosition;
        topTileMapPosition.Y--;
        std::string topTileMapFilepath = overworldSpec.GetTileMapFilepath(
            topTileMapPosition.Y,
            topTileMapPosition.X);

        // Create the top tile map in the overworld.
        std::shared_ptr<Tmx::Map> topTmxMap = mapLoader.LoadMap(topTileMapFilepath);
        // Calculate the world position of the top map.
        float topMapHeightInTiles = static_cast<float>(topTmxMap->GetHeight());
        float topMapTileHeightInPixels = static_cast<float>(topTmxMap->GetTileHeight());
        float topMapHeightInPixels = topMapHeightInTiles * topMapTileHeightInPixels;
        float topMapWorldYPosition = STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.Y - topMapHeightInPixels;
        MATH::Vector2f topTileMapTopLeftWorldPosition(
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.X,
            topMapWorldYPosition);

        std::shared_ptr<MAPS::TileMap> topTileMap = std::make_shared<MAPS::TileMap>(
            topTileMapTopLeftWorldPosition,
            topTmxMap,
            m_graphicsSystem);
        m_overworldMap.SetTopTileMap(topTileMap);
    }

    // Load the bottom tile map.
    MATH::Vector2ui bottomTileMapPosition = startingTileMapPosition;
    bottomTileMapPosition.Y++;
    bool bottomTileMapExists = (bottomTileMapPosition.Y < overworldSpec.GetOverworldHeightInTileMaps());
    if (bottomTileMapExists)
    {
        // Get the filepath for the bottom tile map.
        std::string bottomTileMapFilepath = overworldSpec.GetTileMapFilepath(
            bottomTileMapPosition.Y,
            bottomTileMapPosition.X);

        // Create the bottom tile map in the overworld.
        std::shared_ptr<Tmx::Map> bottomTmxMap = mapLoader.LoadMap(bottomTileMapFilepath);
        // Calculate the world position of the bottom map.
        float centerMapHeightInTiles = static_cast<float>(startingTmxMap->GetHeight());
        float centerMapTileHeightInPixels = static_cast<float>(startingTmxMap->GetTileHeight());
        float centerMapHeightInPixels = centerMapHeightInTiles * centerMapTileHeightInPixels;
        float bottomMapWorldYPosition = STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.Y + centerMapHeightInPixels;
        MATH::Vector2f bottomTileMapTopLeftWorldPosition(
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.X,
            bottomMapWorldYPosition);

        std::shared_ptr<MAPS::TileMap> bottomTileMap = std::make_shared<MAPS::TileMap>(
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
        std::string leftTileMapFilepath = overworldSpec.GetTileMapFilepath(
            leftTileMapPosition.Y,
            leftTileMapPosition.X);

        // Create the left tile map in the overworld.
        std::shared_ptr<Tmx::Map> leftTmxMap = mapLoader.LoadMap(leftTileMapFilepath);
        // Calculate the world position of the left map.
        float leftMapWidthInTiles = static_cast<float>(leftTmxMap->GetWidth());
        float leftMapTileWidthInPixels = static_cast<float>(leftTmxMap->GetTileWidth());
        float leftMapWidthInPixels = leftMapWidthInTiles * leftMapTileWidthInPixels;
        float leftMapWorldXPosition = STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.X - leftMapWidthInPixels;
        MATH::Vector2f leftTileMapTopLeftWorldPosition(
            leftMapWorldXPosition,
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.Y);

        std::shared_ptr<MAPS::TileMap> leftTileMap = std::make_shared<MAPS::TileMap>(
            leftTileMapTopLeftWorldPosition,
            leftTmxMap,
            m_graphicsSystem);
        m_overworldMap.SetLeftTileMap(leftTileMap);
    }

    // Load the right tile map.
    MATH::Vector2ui rightTileMapPosition = startingTileMapPosition;
    rightTileMapPosition.X++;
    bool rightTileMapExists = (rightTileMapPosition.X < overworldSpec.GetOverworldWidthInTileMaps());
    if (rightTileMapExists)
    {
        // Get the filepath for the right tile map.
        std::string rightTileMapFilepath = overworldSpec.GetTileMapFilepath(
            rightTileMapPosition.Y,
            rightTileMapPosition.X);

        // Create the right tile map in the overworld.
        std::shared_ptr<Tmx::Map> rightTmxMap = mapLoader.LoadMap(rightTileMapFilepath);
        // Calculate the world position of the left map.
        float centerMapWidthInTiles = static_cast<float>(startingTmxMap->GetWidth());
        float centerMapTileWidthInPixels = static_cast<float>(startingTmxMap->GetTileWidth());
        float centerMapWidthInPixels = centerMapWidthInTiles * centerMapTileWidthInPixels;
        float rightMapWorldXPosition = STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.X + centerMapWidthInPixels;
        MATH::Vector2f rightTileMapTopLeftWorldPosition(
            rightMapWorldXPosition,
            STARTING_TILE_MAP_TOP_LEFT_WORLD_POSITION.Y);

        std::shared_ptr<MAPS::TileMap> rightTileMap = std::make_shared<MAPS::TileMap>(
            rightTileMapTopLeftWorldPosition,
            rightTmxMap,
            m_graphicsSystem);
        m_overworldMap.SetRightTileMap(rightTileMap);
    }

    return true;
}

void TileMapScrollingTestState::HandleUserInput(const INPUT_CONTROL::IDebugInputController& inputController)
{
    const float MOVE_DISTANCE_IN_PIXELS = 2.0f;

    if (inputController.ScrollUpButtonPressed())
    {
        std::cout << "Scroll Up!" << std::endl;
        m_camera.MoveUp(MOVE_DISTANCE_IN_PIXELS);
        m_graphicsSystem->SetCamera(m_camera);
    }

    if (inputController.ScrollDownButtonPressed())
    {
        std::cout << "Scroll Down!" << std::endl;
        m_camera.MoveDown(MOVE_DISTANCE_IN_PIXELS);
        m_graphicsSystem->SetCamera(m_camera);
    }

    if (inputController.ScrollLeftButtonPressed())
    {
        std::cout << "Scroll Left!" << std::endl;
        m_camera.MoveLeft(MOVE_DISTANCE_IN_PIXELS);
        m_graphicsSystem->SetCamera(m_camera);
    }

    if (inputController.ScrollRightButtonPressed())
    {
        std::cout << "Scroll Right!" << std::endl;
        m_camera.MoveRight(MOVE_DISTANCE_IN_PIXELS);
        m_graphicsSystem->SetCamera(m_camera);
    }
}