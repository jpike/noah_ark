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
        /// The screen for selecting the game (saved or new) to play.
        GAME_SELECTION_SCREEN,
        /// The introductory sequence when starting a new game (new save file).
        NEW_GAME_INTRO_SEQUENCE,
        /// The cutscene that plays once the ark has been built and the world is flooding;
        /// serves to transition the main gameplay to the "inside ark" substate.
        FLOOD_CUTSCENE,
        /// The main gameplay state for the game.
        GAMEPLAY
    };
}
