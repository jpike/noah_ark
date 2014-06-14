#include <hgestrings.h>
#include <string>
#include "NoahArkGame.h"
#include "States/TileMapScrollingTestState.h"

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

bool NoahArkGame::UpdateGame()
{
    // UPDATE THE SINGLETON INSTANCE OF THE GAME.
    return GetInstance().Update();
}

bool NoahArkGame::RenderGame()
{
    // RENDER THE SINGLETON INSTANCE OF THE GAME.
    return GetInstance().Render();
}

///////////////////////////////////////////////////////////
// INSTANCE METHODS.
///////////////////////////////////////////////////////////

NoahArkGame::NoahArkGame() : 
    m_pGameEngine(nullptr),
    m_resourceManager(),
    m_graphicsSystem(),
    m_physicsSystem(),
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
    // INITIALIZE THE GAME ENGINE.
    bool gameEngineInitialized = InitializeGameEngine();
    if (!gameEngineInitialized)
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
    std::shared_ptr<STATES::IGameState> initialState(new STATES::TileMapScrollingTestState(m_pGameEngine, m_graphicsSystem));
    m_stateManager->SetCurrentState(initialState);

    // ALL INITIALIZATION STEPS SUCCEEDED.
    return true;
}

bool NoahArkGame::Shutdown()
{
    // SHUTDOWN THE STATE MANAGER.
    m_stateManager.reset();

    // SHUTDOWN THE PHYSICS SYSTEM.
    m_physicsSystem.reset();

    // SHUTDOWN THE GRAPHICS SYSTEM.
    m_graphicsSystem.reset();

    // SHUTDOWN THE RESOURCE MANAGER.
    m_resourceManager.reset();
    
    // SHUTDOWN THE GAME ENGINE.
    m_pGameEngine->System_Shutdown();
	m_pGameEngine->Release();
	// Release above frees memory, so we can just set the variable to NULL here.
	m_pGameEngine = nullptr;
    
    // ALL SHUTDOWN STEPS SUCCEEDED.
    return true;
}

bool NoahArkGame::RunGameLoop()
{
    // RUN THE GAME LOOP.
    return m_pGameEngine->System_Start();
}

bool NoahArkGame::InitializeGameEngine()
{
    // CREATE THE UNDERLYING GAME ENGINE.
    m_pGameEngine = hgeCreate(HGE_VERSION);
    bool engineCreated = ( m_pGameEngine != nullptr );
    if (!engineCreated)
    {
        return false;
    }

    // CONFIGURE THE UNDERLYING GAME ENGINE.
    // Currently, flexibility of file-based configuration isn't needed.
    // Only settings that are important to the current operation of the
    // game are explicitly set.  Other settings may be set in the future
    // as required.

    // Set the callback functions needed for running the game.
    m_pGameEngine->System_SetState(HGE_FRAMEFUNC, NoahArkGame::UpdateGame);
	m_pGameEngine->System_SetState(HGE_RENDERFUNC, NoahArkGame::RenderGame);

    // The following set of settings are largely arbitrary but are deemed to be the
    // current best settings for the game.  They may be changed in the future.
    m_pGameEngine->System_SetState(HGE_TITLE, "Bible Games - Noah's Ark");
    m_pGameEngine->System_SetState(HGE_SCREENWIDTH, 512);
    m_pGameEngine->System_SetState(HGE_SCREENHEIGHT, 384);
    m_pGameEngine->System_SetState(HGE_WINDOWED, true);
    m_pGameEngine->System_SetState(HGE_LOGFILE, "noah_ark_game.log");
    m_pGameEngine->System_SetState(HGE_FPS, 60);
    m_pGameEngine->System_SetState(HGE_HIDEMOUSE, false);

    // The following set of settings are important to the operation of the game
    // and should not be changed without deep consideration.
    // Z-buffering is needed for the graphics system to properly render overlapping objects.
    m_pGameEngine->System_SetState(HGE_ZBUFFER, true);
    /// @todo Document why the texture filtering is disabled.
    m_pGameEngine->System_SetState(HGE_TEXTUREFILTER, false);
    /// @todo Sound is currently disabled since we aren't using HGE's built-in BASS audio
    ///       functionality.  If we end up using BASS for audio, then this will need to be enabled.
    m_pGameEngine->System_SetState(HGE_USESOUND, false);    
    
    // INITIALIZE THE UNDERLYING GAME ENGINE.
    bool gameEngineInitialized = m_pGameEngine->System_Initiate();
    return gameEngineInitialized;
}

bool NoahArkGame::InitializeSubsystems()
{
    // INITIALIZE THE RESOURCE MANAGER.
    const char* RESOURCE_FILE_RELATIVE_PATH = "res/resources.txt";
    m_resourceManager = std::make_shared<hgeResourceManager>(RESOURCE_FILE_RELATIVE_PATH);

    // INITIALIZE THE GRAPHICS SYSTEM.
    m_graphicsSystem = std::make_shared<GRAPHICS::GraphicsSystem>(m_pGameEngine, m_resourceManager);

    // INITIALIZE THE PHYSICS SYSTEM.
    m_physicsSystem = std::make_shared<PHYSICS::PhysicsSystem>();

    // INITIALIZE THE STATE MANAGER.
    m_stateManager = std::make_shared<STATES::StateManager>();
    
    // RETURN THE RESULT OF INITIALIZATION.
    // This will always be true for now.
    bool allSubsystemsInitialized = true;
    return allSubsystemsInitialized;
}


bool NoahArkGame::Update()
{
    return m_stateManager->Update();
}

bool NoahArkGame::Render()
{
    // SETUP THE GAME ENGINE FOR RENDERING.
    m_pGameEngine->Gfx_BeginScene();

    /// @todo   Should this clear color be retrieved from the graphics system?
    const DWORD BLACK_COLOR = 0;
	m_pGameEngine->Gfx_Clear(BLACK_COLOR);

    // RENDER THE GAME.
    m_graphicsSystem->Render();

    // FINISH RENDERING THE CURRENT SCENE.
    m_pGameEngine->Gfx_EndScene();

    // False is required to always be returned by HGE.
    return false;
}