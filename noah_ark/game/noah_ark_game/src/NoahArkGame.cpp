#include <exception>
#include <iostream>
#include <string>
#include "NoahArkGame.h"
#include "States/OverworldState.h"

// STATIC MEMBER VARIABLES.
std::unique_ptr<NoahArkGame> NoahArkGame::singleNoahArkGame = nullptr;

///////////////////////////////////////////////////////////
// STATIC METHODS.
///////////////////////////////////////////////////////////

int NoahArkGame::RunGame()
{
    // CREATE THE SINGLETON INSTANCE OF THE GAME.
    singleNoahArkGame = std::unique_ptr<NoahArkGame>(new NoahArkGame());
    
    // INITIALIZE THE GAME.
    bool initializationSuccessful = singleNoahArkGame->Initialize();
    if (!initializationSuccessful)
    {
        return EXIT_CODE_INITIALIZATION_FAILURE;
    }

    // RUN THE GAME.
    // This will not return until the user chooses to exit or an error occurs.
    bool gameCompletedSuccessfully = singleNoahArkGame->RunGameLoop();

    // SHUTDOWN THE GAME.
    bool shutdownSuccessful = singleNoahArkGame->Shutdown();
    // Free memory used by the singleton instance of the game.
    singleNoahArkGame.reset();

    // DETERMINE THE EXIT CODE TO RETURN.
    if (gameCompletedSuccessfully)
    {
        if (shutdownSuccessful)
        {
            return EXIT_CODE_SUCCESS;
        }
        else
        {
            return EXIT_CODE_SHUTDOWN_FAILURE;
        }
    }
    else
    {
        if (shutdownSuccessful)
        {
            return EXIT_CODE_FAILURE;
        }
        else
        {
            return (EXIT_CODE_FAILURE | EXIT_CODE_SHUTDOWN_FAILURE);
        }
    }
}

///////////////////////////////////////////////////////////
// INSTANCE METHODS.
///////////////////////////////////////////////////////////

NoahArkGame::NoahArkGame() : 
    m_window(),
    m_resourceManager(),
    m_graphicsSystem(),
    m_collisionSystem(),
    m_stateManager()
{
    // CHECK IF AN INSTANCE OF THE GAME HAS ALREADY BEEN CREATED.
    bool gameAlreadyCreated = (singleNoahArkGame.get() != nullptr);
    if (gameAlreadyCreated)
    {
        // Limitations in HGE prevent supporting more than one instance.
        throw std::logic_error("Cannot create more than one instance of NoahArkGame.");
    }
}

NoahArkGame::~NoahArkGame()
{
    // Nothing else to do.
}

bool NoahArkGame::Initialize()
{
    // INITIALIZE THE MAIN WINDOW.
    bool windowInitialized = InitializeWindow();
    if (!windowInitialized)
    {
        return false;
    }

    // INITIALIZE OTHER SUBSYSTEMS.
    bool subsystemsInitialized = InitializeSubsystems();
    if (!subsystemsInitialized)
    {
        return false;
    }

    // SET THE INITIAL GAME STATE.
    /// @todo The state needs to be changed once all of the game states and finally implemented.
    ///       It should go to an "intro video" or "title screen" state initially.
    std::shared_ptr<STATES::IGameState> initialState(
        new STATES::OverworldState(
            m_graphicsSystem, 
            m_collisionSystem));
    m_stateManager->SetCurrentState(initialState);

    // ALL INITIALIZATION STEPS SUCCEEDED.
    return true;
}

bool NoahArkGame::Shutdown()
{
    // SHUTDOWN THE STATE MANAGER.
    m_stateManager.reset();

    // SHUTDOWN THE COLLISION SYSTEM.
    m_collisionSystem.reset();

    // SHUTDOWN THE GRAPHICS SYSTEM.
    m_graphicsSystem.reset();
    
    // SHUTDOWN THE RESOURCE MANAGER.
    m_resourceManager.reset();

    // CLOSE THE WINDOW IF IT IS STILL OPEN.
    m_window->close();
    
    // ALL SHUTDOWN STEPS SUCCEEDED.
    return true;
}

bool NoahArkGame::RunGameLoop()
{
    // RUN THE GAME LOOP.
    try
    {
        // PROCESS EVENTS AS LONG AS THE WINDOW IS OPEN.
        sf::Clock gameLoopClock;
        while (m_window->isOpen())
        {
            // PROCESS WINDOW EVENTS.
            sf::Event event;
            while (m_window->pollEvent(event))
            {
                // Handle the current event based on its type.
                switch (event.type)
                {
                case sf::Event::Closed:
                    m_window->close();
                    break;
                }
            }

            // UPDATE AND DISPLAY THE GAME IN THE WINDOW.
            if (m_window->isOpen())
            {
                // Update the game for the new frame.
                sf::Time elapsedTime = gameLoopClock.restart();
                Update(elapsedTime);

                // Render the current state of the game.
                m_window->clear();
                Render();
                m_window->display();
            }
        }
        
        return true;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Exception during game: " << exception.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown error during game. " << std::endl;
        return false;
    }
}

bool NoahArkGame::InitializeWindow()
{
    // CREATE THE WINDOW.
    // The width and height are currently set to match the dimensions
    // of a single tile map in the game.
    const unsigned int SCREEN_WIDTH_IN_PIXELS = 512;
    const unsigned int SCREEN_HEIGHT_IN_PIXELS = 384;
    const std::string GAME_TITLE = "Bible Games - Noah's Ark";
    m_window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS),
        GAME_TITLE);

    return true;
}

bool NoahArkGame::InitializeSubsystems()
{
    // INITIALIZE THE RESOURCE MANAGER.
    const char* RESOURCE_FILE_RELATIVE_PATH = "res/resources.json";
    m_resourceManager = std::make_shared<RESOURCES::ResourceManager>(RESOURCE_FILE_RELATIVE_PATH);

    // INITIALIZE THE GRAPHICS SYSTEM.
    m_graphicsSystem = std::make_shared<GRAPHICS::GraphicsSystem>(m_window, m_resourceManager);

    // INITIALIZE THE COLLISION SYSTEM.
    m_collisionSystem = std::make_shared<PHYSICS::COLLISION::CollisionSystem>();

    // INITIALIZE THE STATE MANAGER.
    m_stateManager = std::make_shared<STATES::StateManager>();
    
    // RETURN THE RESULT OF INITIALIZATION.
    // This will always be true for now.
    bool allSubsystemsInitialized = true;
    return allSubsystemsInitialized;
}

void NoahArkGame::Update(const sf::Time& elapsedTime)
{   
    m_stateManager->Update(elapsedTime.asSeconds());
}

void NoahArkGame::Render()
{
    // RENDER THE GAME.
    m_graphicsSystem->Render();
}