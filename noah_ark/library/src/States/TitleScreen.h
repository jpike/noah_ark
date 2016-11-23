#pragma once

#include "Graphics/Renderer.h"
#include "Input/KeyboardInputController.h"

namespace STATES
{
    /// The title screen for the game.  Displays the game's title
    /// along with menu options related to beginning the main gameplay.
    class TitleScreen
    {
    public:
        bool RespondToInput(const INPUT_CONTROL::KeyboardInputController& input_controller) const;
        void Render(GRAPHICS::Renderer& renderer) const;
    };
}
