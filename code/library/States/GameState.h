#pragma once

#include <string>

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
        /// The notice screen.
        NOTICE_SCREEN,
        /// The screen for selecting the game (saved or new) to play.
        GAME_SELECTION_SCREEN,
        /// The introductory sequence when starting a new game (new save file).
        NEW_GAME_INTRO_SEQUENCE,
        /// The instruction sequence when starting a new game (new save file).
        NEW_GAME_INSTRUCTION_SEQUENCE,
        /// The main gameplay state for the game (prior to the flood).
        PRE_FLOOD_GAMEPLAY,
        /// The cutscene that plays once the ark has been built and the world is flooding;
        /// serves to transition the main gameplay to the "inside ark" substate.
        FLOOD_CUTSCENE,
        /// The gameplay state during the flood (where everyone is confined to the ark).
        DURING_FLOOD_GAMEPLAY,
        /// The gameplay state after the flood (a bit of gameplay + minor wrap-up cutscenes).
        POST_FLOOD_GAMEPLAY,
        /// The ending credits screen (plays special background music).
        ENDING_CREDITS_SCREEN
    };
}

namespace std
{
    string to_string(const STATES::GameState game_state);
}
