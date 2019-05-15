#pragma once

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
        void LoadSavedGames();

        GameState RespondToInput(const INPUT_CONTROL::InputController& input_controller);
        void Render(GRAPHICS::Renderer& renderer) const;

        /// The index of the saved game that is currently selected.
        std::size_t SelectedGameIndex = 0;
        /// The saved games available for the player to select.
        std::vector< std::unique_ptr<SavedGameData> > SavedGames = {};
    };
}
