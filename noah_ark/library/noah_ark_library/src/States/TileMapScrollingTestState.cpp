#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Maps/OverworldMapSpecification.h"
#include "States/TileMapScrollingTestState.h"

using namespace STATES;

TileMapScrollingTestState::TileMapScrollingTestState(
    HGE* const pGameEngine,
    std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem) :
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
    // Load the initial tile map.
    std::shared_ptr<Tmx::Map> initialTmxMap(new Tmx::Map());
    initialTmxMap->ParseFile(overworldSpec.GetStartingTileMapFilepath());
    std::shared_ptr<MAPS::TileMap> initialTileMap( new MAPS::TileMap(initialTmxMap, m_graphicsSystem) );
    m_overworldMap.SetCurrentTileMap(initialTileMap);
    return true;
}

void TileMapScrollingTestState::HandleUserInput(const INPUT_CONTROL::IDebugInputController& inputController)
{
    if (inputController.ScrollUpButtonPressed())
    {
        std::cout << "Scroll Up!" << std::endl;
    }

    if (inputController.ScrollDownButtonPressed())
    {
        std::cout << "Scroll Down!" << std::endl;
    }

    if (inputController.ScrollLeftButtonPressed())
    {
        std::cout << "Scroll Left!" << std::endl;
    }

    if (inputController.ScrollRightButtonPressed())
    {
        std::cout << "Scroll Right!" << std::endl;
    }
}