#pragma once

#include <memory>
#include <hge.h>
#include "Graphics/Camera.h"
#include "Input/MapScrollingDebugInputController.h"
#include "Maps/OverworldMap.h"
#include "Maps/OverworldMapSpecification.h"
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

        /// @brief      Updates the overworld by loading the new surrounding maps after scrolling.
        /// @param[in]  scrollStartPoint - The starting point of the scrolling that just finished.
        /// @param[in]  scrollEndPoint - The ending point of the scrolling that just finished.
        void LoadSurroundingMapsAfterScrolling(const MATH::Vector2f& scrollStartPoint, const MATH::Vector2f& scrollEndPoint);

        /// @brief      Loads tile maps surrounding the provided center map.
        /// @param[in]  centerTileMap - The center tile map from which to load surrounding maps.
        /// @param[out] topTileMap - The new map above the center tile map, if one exists.
        /// @param[out] bottomTileMap - The new map below the center tile map, if one exists.
        /// @param[out] leftTileMap - The new map left of the center tile map, if one exists.
        /// @param[out] rightTileMap - The new map right of the center tile map, if one exists.
        void LoadSurroundingMaps(
            const std::shared_ptr<const MAPS::TileMap>& centerTileMap,
            std::shared_ptr<MAPS::TileMap>& topTileMap,
            std::shared_ptr<MAPS::TileMap>& bottomTileMap,
            std::shared_ptr<MAPS::TileMap>& leftTileMap,
            std::shared_ptr<MAPS::TileMap>& rightTileMap);

        /// @brief      Handles user input for a single frame.
        /// @param[in]  inputController - The controller supplying user input.
        void HandleUserInput(const INPUT_CONTROL::IDebugInputController& inputController);

        GRAPHICS::Camera m_camera;  ///< The camera indicating the portion of the world being shown.
        std::unique_ptr<PROCESSES::ScrollProcess> m_scrollProcess;  ///< The process controlling scrolling of tile maps in the overworld map, if active (nullptr otherwise).

        std::shared_ptr<GRAPHICS::GraphicsSystem> m_graphicsSystem; ///< The graphics system.
        INPUT_CONTROL::MapScrollingDebugInputController m_inputController;  ///< The controller supplying user input.

        MAPS::OverworldMapSpecification m_overworldSpec;    ///< The specification of the overworld map.
        MAPS::OverworldMap m_overworldMap;  ///< The overworld map of the game.
    };
}