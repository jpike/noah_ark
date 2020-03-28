#pragma once

#include <vector>
#include <SFML/System.hpp>
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
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
        GameState Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller);
        void Render(GRAPHICS::Renderer& renderer) const;

    private:
        // MEMBER VARIABLES.
        /// The total time that the title screen has been shown.
        sf::Time ElapsedTime = sf::Time::Zero;
        /// The index to the currently selected menu option.
        unsigned int SelectedMenuOptionIndex;
        /// The game states corresponding to menu options displayed on the title screen.
        std::vector<GameState> MenuOptions;
    };
}
