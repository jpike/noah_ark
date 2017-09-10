#pragma once

namespace STATES
{
    /// Enumerates the different states the game could be in.
    enum class GameState
    {
        /// The introductory sequence for the game.
        INTRO_SEQUENCE,
        /// The game's title screen.
        TITLE_SCREEN,
        /// The credits screen.
        CREDITS_SCREEN,
        /// The main pre-flood gameplay state for the game.
        PRE_FLOOD_GAMEPLAY,
        /// The main flood gameplay state for the game.
        FLOOD_GAMEPLAY,
    };
}
