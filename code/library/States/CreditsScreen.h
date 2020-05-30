#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
#include "States/GameState.h"

namespace STATES
{
    /// The credits screen for the game.
    class CreditsScreen
    {
    public:
        // METHODS.
        GameState Update(
            const sf::Time& elapsed_time,
            const INPUT_CONTROL::InputController& input_controller);
        sf::Sprite Render(GRAPHICS::Renderer& renderer) const;

        // MEMBER VARIABLES.
        /// The total time that the credits screen has been shown.
        sf::Time ElapsedTime = sf::Time::Zero;
    };
}
