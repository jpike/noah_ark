#pragma once

#include "Maps/OverworldMap.h"
#include "States/IGameState.h"

namespace STATES
{
    ///////////////////////////////////////////////////////////
    /// \brief  The main gameplay state that involves the player (Noah)
    ///         moving over an overworld.
    ///////////////////////////////////////////////////////////
    class OverworldState : public IGameState
    {
    public:
        /// @brief  Constructor.
        explicit OverworldState();
        /// @brief  Destructor.
        virtual ~OverworldState();

    private:
        OverworldState(const OverworldState& stateToCopy);  ///< Private to disallow copying.
        OverworldState& operator= (const OverworldState& rhsState); ///< Private to disallow assignment.

        MAPS::OverworldMap m_overworldMap;  ///< The overworld map of the game.
    };
}