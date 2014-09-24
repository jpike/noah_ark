#pragma once

#include <memory>
#include "Graphics/Camera.h"
#include "Input/MapScrollingDebugInputController.h"
#include "Maps/OverworldMapSpecification.h"
#include "Maps/ScrollableOverworldMap.h"
#include "Objects/Noah.h"
#include "States/IGameState.h"

namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// @brief  A state for testing tile map scrolling in an overworld map.
    ///////////////////////////////////////////////////////////
    class TileMapScrollingTestState : public IGameState
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  graphicsSystem - The graphics system.
        /// @throws     std::runtime_error - Thrown if the overworld can't be loaded.
        explicit TileMapScrollingTestState(
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);
        /// @brief  Destructor.
        virtual ~TileMapScrollingTestState();

        /// @copydoc    IGameState::Update(const float elapsedTimeInSeconds)
        virtual bool Update(const float elapsedTimeInSeconds);

    private:
        TileMapScrollingTestState(const TileMapScrollingTestState& stateToCopy);  ///< Private to disallow copying.
        TileMapScrollingTestState& operator= (const TileMapScrollingTestState& rhsState); ///< Private to disallow assignment.
        
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
        void HandleUserInput(const INPUT_CONTROL::IDebugInputController& inputController, const float elapsedTimeInSeconds);

        GRAPHICS::Camera m_camera;  ///< The camera indicating the portion of the world being shown.

        std::shared_ptr<GRAPHICS::GraphicsSystem> m_graphicsSystem; ///< The graphics system.
        INPUT_CONTROL::MapScrollingDebugInputController m_inputController;  ///< The controller supplying user input.

        MAPS::OverworldMapSpecification m_overworldSpec;    ///< The specification of the overworld map.
        std::shared_ptr<MAPS::OverworldMap> m_overworldMap; ///< The overworld map of the game.
        std::unique_ptr<MAPS::ScrollableOverworldMap> m_scrollingOverworld;  ///< Wraps the overworld map to manage scrolling.

        OBJECTS::Noah m_noahPlayer; ///< Noah, the main character controlled by the player.
    };
}