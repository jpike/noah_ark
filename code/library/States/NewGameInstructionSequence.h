#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Maps/World.h"
#include "States/GameState.h"

namespace STATES
{
    /// The game state for a new game where the player is in the overworld
    /// and receiving instructions from God for the task.
    class NewGameInstructionSequence
    {
    public:
        void Initialize(const std::shared_ptr<MAPS::World>& world, GRAPHICS::GUI::HeadsUpDisplay& hud);
        GameState Update(const HARDWARE::GamingHardware& gaming_hardware, GRAPHICS::GUI::HeadsUpDisplay& hud);
        sf::Sprite Render(const HARDWARE::GamingHardware& gaming_hardware, GRAPHICS::GUI::HeadsUpDisplay& hud, GRAPHICS::Renderer& renderer);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// True if the instructions that are displayed in a text box at the start
        /// of a new game are completed; false otherwise.
        bool NewGameInstructionsCompleted = false;
        /// The instructional text to be displayed for a new game.
        std::string NewGameInstructionText = "";
    };
}
