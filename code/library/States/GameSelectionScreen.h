#pragma once

#include <filesystem>
#include <memory>
#include <vector>
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// The screen for selecting the game to play
    /// (either a saved game file or a new game).
    class GameSelectionScreen
    {
    public:
        /// The possible substates of the game.
        enum class SubState
        {
            /// The state where all saved game options are being listed.
            LISTING_GAMES,
            /// The state where the player is entering a new saved game name.
            ENTERING_NEW_GAME
        };

        /// The path to the saved games folder.
        static const std::filesystem::path GameSelectionScreen::SAVED_GAMES_FOLDER_PATH;

        void LoadSavedGames();

        GameState RespondToInput(const INPUT_CONTROL::InputController& input_controller);
        void Render(GRAPHICS::Renderer& renderer) const;

        /// The current substate.
        SubState CurrentSubState = SubState::LISTING_GAMES;
        /// The current text being entered by the user for a new saved game filename.
        std::string CurrentNewGameFilenameText = "";
        /// The index of the saved game that is currently selected.
        std::size_t SelectedGameIndex = 0;
        /// The saved games available for the player to select.
        std::vector< std::shared_ptr<SavedGameData> > SavedGames = {};
    };
}
