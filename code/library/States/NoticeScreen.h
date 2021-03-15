#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "States/GameState.h"

namespace STATES
{
    /// A screen that gives a notice about the usage of
    /// Bible verses in the game.
    class NoticeScreen
    {
    public:
        GameState Update(const HARDWARE::GamingHardware& gaming_hardware);
        sf::Sprite Render(GRAPHICS::Renderer& renderer) const;
    };
}
