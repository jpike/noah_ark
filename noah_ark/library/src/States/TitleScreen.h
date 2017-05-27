#pragma once

#include <vector>
#include "Graphics/Renderer.h"
#include "Input/KeyboardInputController.h"
#include "States/GameState.h"

namespace STATES
{
    /// The title screen for the game.  Displays the game's title
    /// along with menu options related to the game.
    class TitleScreen
    {
    public:
        // CONSTRUCTION.
        explicit TitleScreen();

        // OTHER METHODS.
        GameState RespondToInput(const INPUT_CONTROL::KeyboardInputController& input_controller);
        void Render(GRAPHICS::Renderer& renderer) const;

    private:
        // MEMBER VARIABLES.
        /// The index to the currently selected menu option.
        unsigned int SelectedMenuOptionIndex;
        /// The game states corresponding to menu options displayed on the title screen.
        std::vector<GameState> MenuOptions;
    };
}
