#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Collision/CollisionSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Resources/ResourceManager.h"
#include "States/StateManager.h"

///////////////////////////////////////////////////////////
/// @brief  Encapsulates the logic for running the entire Noah's Ark game.
///         Only one instance of the game should be created due to limitations
///         in the underlying game engine.
///////////////////////////////////////////////////////////
class NoahArkGame
{
public:
    // CONSTANTS.
    // Exit codes that may be returned from running the game.
    // The values for the codes are intentionally chosen so that
    // they may be logically OR'ed together.
    static const int EXIT_CODE_SUCCESS = 0x0;   ///< The game exited normally and successfully.
    static const int EXIT_CODE_FAILURE = 0x1;   ///< The game exited due to a general error.
    static const int EXIT_CODE_INITIALIZATION_FAILURE = 0x2;    ///< The game failed to initialize.
    static const int EXIT_CODE_SHUTDOWN_FAILURE = 0x4;   ///< The game failed to properly shutdown.
    
    // STATIC METHODS.
    /// @brief  Runs the game until completion (user chooses to exit or an error occurs).
    /// @return The exit code indicating the how the game completed.
    static int RunGame();

    // INSTANCE METHODS.
    /// @brief  Destructor.  Public to allow destruction by smart pointers.
    ~NoahArkGame();

private:
    // STATIC MEMBER VARIABLES.
    static std::unique_ptr<NoahArkGame> singleNoahArkGame; ///< The singleton instance of the game.
    
    // STATIC METHODS.
    /// @brief  Helper method to get a reference to the singleton instance of the Noah's Ark game.
    ///         The instance is required to already exist.
    /// @return The singleton instance of the Noah's Ark game.
    static NoahArkGame& GetInstance()   { return *singleNoahArkGame; }

    // CONSTRUCTORS/DESTRUCTOR.
    /// @brief  Constructor.
    /// @throws std::logic_error - Thrown if an instance of the game has already been created.
    explicit NoahArkGame();

    // COPY CONSTRUCTOR/ASSIGNMENT OPERATOR.
    NoahArkGame(const NoahArkGame& otherGame);	///< Private to disallow copying.
	NoahArkGame& operator=(const NoahArkGame& otherGame);	///< Private to disallow assignment.

    // HIGHEST-LEVEL PRIVATE HELPER METHODS.
    /// @brief  Initializes the game for use.  Must be called and return success before attempting to run the game.
    /// @return True if initialization succeeded and false otherwise.
    bool Initialize();
    /// @brief  Runs the main loop of the game.  Blocks until the game loop completes (either due to user interaction or an error).
    /// @return True if the game loop successfully completed and false otherwise.
    bool RunGameLoop();
    /// @brief  Shutsdown the game, releasing resources.  Should be called before completely exiting the game.
    /// @return True if shutdown succeeded and false otherwise.
    bool Shutdown();

    // INITIALIZATION HELPER FUNCTIONS.
    /// @brief  Initializes the window used to display te game.
    /// @return True if window initialization succeeded and false otherwise.
    bool InitializeWindow();
    /// @brief  Initializes subsystems outside of the internal game engine for use.
    /// @return True if subsystem initialization succeeded and false otherwise.
    bool InitializeSubsystems();

    // MAIN GAME LOOP HELPER FUNCTIONS.
    /// @brief      Updates the game for a single frame.
    /// @param[in]  elapsedTime - The elapsed time since the last frame update.
    void Update(const sf::Time& elapsedTime);
    /// @brief          Renders a single frame of the game.
    void Render();

    // MEMBER VARIABLES.
    std::shared_ptr<sf::RenderWindow> m_window; ///< The window displaying the game.
    std::shared_ptr<RESOURCES::ResourceManager> m_resourceManager;    ///< The resource manager.
    std::shared_ptr<GRAPHICS::GraphicsSystem> m_graphicsSystem; ///< The graphics system.
    std::shared_ptr<COLLISION::CollisionSystem> m_collisionSystem;  ///< The collision system.
    std::shared_ptr<STATES::StateManager> m_stateManager; ///< The game state manager.
};
