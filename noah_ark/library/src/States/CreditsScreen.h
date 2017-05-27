#pragma once

#include "Graphics/Renderer.h"
#include "Input/KeyboardInputController.h"
#include "States/GameState.h"

namespace STATES
{
    /// The credits screen for the game.
    class CreditsScreen
    {
    public:
        GameState RespondToInput(const INPUT_CONTROL::KeyboardInputController& input_controller) const;
        void Render(GRAPHICS::Renderer& renderer) const;
    };
}
