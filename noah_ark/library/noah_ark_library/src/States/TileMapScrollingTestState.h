#pragma once

#include <memory>
#include <hge.h>
#include "Graphics/Camera.h"
#include "Input/MapScrollingDebugInputController.h"
#include "Maps/OverworldMap.h"
#include "Maps/OverworldMapSpecification.h"
#include "Maps/SurroundingTileMapLoader.h"
#include "Objects/Noah.h"
#include "Processes/ScrollProcess.h"
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
        /// @param[in]  pGameEngine - The HGE game engine.  Must not be nullptr.
        /// @param[in]  graphicsSystem - The graphics system.
        /// @throws     std::runtime_error - Thrown if the overworld can't be loaded.
        explicit TileMapScrollingTestState(
            HGE* const pGameEngine,
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

        /// @brief      Updates the overworld for the new surrounding maps after scrolling has finished.
        /// @param[in]  direction - The direction of the scrolling that just finished.
        void UpdateSurroundingMapsAfterScrolling(const PROCESSES::ScrollProcess::ScrollDirection direction);

        GRAPHICS::Camera m_camera;  ///< The camera indicating the portion of the world being shown.
        std::unique_ptr<PROCESSES::ScrollProcess> m_scrollProcess;  ///< The process controlling scrolling of tile maps in the overworld map, if active (nullptr otherwise).

        std::shared_ptr<GRAPHICS::GraphicsSystem> m_graphicsSystem; ///< The graphics system.
        INPUT_CONTROL::MapScrollingDebugInputController m_inputController;  ///< The controller supplying user input.

        MAPS::OverworldMapSpecification m_overworldSpec;    ///< The specification of the overworld map.
        MAPS::OverworldMap m_overworldMap;  ///< The overworld map of the game.
        std::unique_ptr<MAPS::SurroundingTileMapLoader> m_surroundingMapLoader;  ///< The loader of maps surrounding the center one.

        OBJECTS::Noah m_noahPlayer; ///< Noah, the main character controlled by the player.
    };
}