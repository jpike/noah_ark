#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Graphics/Gui/TextBox.h"
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
        void Initialize(MAPS::World& world);
        GameState Update(const HARDWARE::GamingHardware& gaming_hardware);
        sf::Sprite Render(
            const HARDWARE::GamingHardware& gaming_hardware, 
            MAPS::World& world,
            GRAPHICS::Renderer& renderer);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The instructional text to be displayed for a new game.
        std::string NewGameInstructionText = "";
        /// The text box displaying the instructions.
        GRAPHICS::GUI::TextBox InstructionTextBox;
    };
}
