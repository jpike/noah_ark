#pragma once

#include <memory>
#include <string>
#include "Graphics/GraphicsSystem.h"
#include "Input/KeyboardInputController.h"
#include "Maps/OverworldMap.h"
#include "Maps/ScrollableOverworldMap.h"
#include "Objects/Noah.h"
#include "Physics/Collision/CollisionSystem.h"
#include "States/IGameState.h"

namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// @brief  The main gameplay state that involves the player (Noah)
    ///         moving over an overworld.
    ///////////////////////////////////////////////////////////
    class OverworldState : public IGameState
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  graphicsSystem - The graphics system.
        /// @param[in]  collisionSystem - The collision system.
        /// @throws     std::runtime_error - Thrown if the an error occurs during initialization.
        explicit OverworldState(
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem,
            std::shared_ptr<PHYSICS::COLLISION::CollisionSystem>& collisionSystem);
        /// @brief  Destructor.
        virtual ~OverworldState();

        /// @copydoc    IGameState::Update(const float elapsedTimeInSeconds)
        virtual bool Update(const float elapsedTimeInSeconds);

    private:
        OverworldState(const OverworldState& stateToCopy);  ///< Private to disallow copying.
        OverworldState& operator= (const OverworldState& rhsState); ///< Private to disallow assignment.

        /// @brief      Attempts to load the overworld map from the provided specification file.
        /// @param[in]  overworldSpecFilepath - The filepath to the overworld map specification.
        /// @return     True if the overworld was loaded successfully; false otherwise.
        bool LoadOverworldMap(const std::string& overworldSpecFilepath);

        /// @brief      Initializes the player object in the provided Noah character.
        /// @param[out] noahPlayer - The player Noah character, if successfully initialized.
        /// @return     True if the player is successfully initialized; false otherwise.
        bool InitializePlayer(OBJECTS::Noah& noahPlayer);

        /// @brief      Handles user input for a single frame.
        /// @param[in]  inputController - The controller supplying user input.
        /// @param[in]  elapsedTimeInSeconds - The elapsed time for the frame to handle input for.
        void HandleUserInput(const INPUT_CONTROL::KeyboardInputController& inputController, const float elapsedTimeInSeconds);

        GRAPHICS::Camera m_camera;  ///< The camera indicating the portion of the world being shown.

        std::shared_ptr<GRAPHICS::GraphicsSystem> m_graphicsSystem; ///< The graphics system.
        INPUT_CONTROL::KeyboardInputController m_inputController;  ///< The controller supplying user input.

        std::shared_ptr<PHYSICS::COLLISION::CollisionSystem> m_collisionSystem; ///< The collision system.

        MAPS::OverworldMapSpecification m_overworldSpec;    ///< The specification of the overworld map.
        std::shared_ptr<MAPS::OverworldMap> m_overworldMap; ///< The overworld map of the game.
        std::unique_ptr<MAPS::ScrollableOverworldMap> m_scrollingOverworld;  ///< Wraps the overworld map to manage scrolling.

        OBJECTS::Noah m_noahPlayer; ///< Noah, the main character controlled by the player.
    };
}