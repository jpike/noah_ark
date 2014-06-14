#pragma once

#include <memory>
#include <hge.h>
#include "Graphics/Camera.h"
#include "Input/MapScrollingDebugInputController.h"
#include "Maps/OverworldMap.h"
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

        /// @brief      Updates the state for a single frame of the game.
        /// @return     True if the game should terminate and no longer update;
        ///             false if the game should continue updating.
        virtual bool Update();

    private:
        TileMapScrollingTestState(const TileMapScrollingTestState& stateToCopy);  ///< Private to disallow copying.
        TileMapScrollingTestState& operator= (const TileMapScrollingTestState& rhsState); ///< Private to disallow assignment.
        
        /// @brief      Attempts to load the overworld map from the provided specification file.
        /// @param[in]  overworldSpecFilepath - The filepath to the overworld map specification.
        /// @return     True if the overworld was loaded successfully; false otherwise.
        bool LoadOverworldMap(const std::string& overworldSpecFilepath);

        /// @brief      Handles user input for a single frame.
        /// @param[in]  inputController - The controller supplying user input.
        void HandleUserInput(const INPUT_CONTROL::IDebugInputController& inputController);

        GRAPHICS::Camera m_camera;  ///< The camera indicating the portion of the world being shown.

        std::shared_ptr<GRAPHICS::GraphicsSystem> m_graphicsSystem; ///< The graphics system.
        INPUT_CONTROL::MapScrollingDebugInputController m_inputController;  ///< The controller supplying user input.

        MAPS::OverworldMap m_overworldMap;  ///< The overworld map of the game.
    };
}