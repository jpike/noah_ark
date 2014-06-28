#pragma once

#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Maps/OverworldMap.h"
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
        explicit OverworldState(std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem);
        /// @brief  Destructor.
        virtual ~OverworldState();

        /// @copydoc    IGameState::Update(const float elapsedTimeInSeconds)
        virtual bool Update(const float elapsedTimeInSeconds);

    private:
        OverworldState(const OverworldState& stateToCopy);  ///< Private to disallow copying.
        OverworldState& operator= (const OverworldState& rhsState); ///< Private to disallow assignment.

        std::shared_ptr<GRAPHICS::GraphicsSystem> m_graphicsSystem; ///< The graphics system.
        MAPS::OverworldMap m_overworldMap;  ///< The overworld map of the game.
    };
}