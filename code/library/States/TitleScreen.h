#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "States/GameState.h"

namespace STATES
{
    /// The title screen for the game.  Displays the game's title
    /// along with menu options related to the game.
    class TitleScreen
    {
    public:
        // OTHER METHODS.
        GameState Update(const HARDWARE::GamingHardware& gaming_hardware);
        sf::Sprite Render(GRAPHICS::Renderer& renderer) const;

    private:
        // MEMBER VARIABLES.
        /// The total time that the title screen has been shown.
        sf::Time ElapsedTime = sf::Time::Zero;
        /// The index to the currently selected menu option.
        unsigned int SelectedMenuOptionIndex = 0;
        /// The game states corresponding to menu options displayed on the title screen.
        std::vector<GameState> MenuOptions = { GameState::GAME_SELECTION_SCREEN, GameState::CREDITS_SCREEN, GameState::NOTICE_SCREEN };
    };
}
