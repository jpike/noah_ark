#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "States/GameState.h"

namespace STATES
{
    /// The ending credits screen.
    class EndingCreditsScreen
    {
    public:
        // METHODS.
        GameState Update(const HARDWARE::GamingHardware& gaming_hardware);
        sf::Sprite Render(GRAPHICS::Renderer& renderer) const;

        // MEMBER VARIABLES.
        /// The total time that the credits screen has been shown.
        sf::Time ElapsedTime = sf::Time::Zero;
    };
}