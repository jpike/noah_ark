#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Maps/World.h"
#include "States/CreditsScreen.h"
#include "States/FloodCutscene.h"
#include "States/GameplayState.h"
#include "States/GameSelectionScreen.h"
#include "States/GameState.h"
#include "States/IntroSequence.h"
#include "States/NewGameIntroSequence.h"
#include "States/TitleScreen.h"

namespace STATES
{
    /// All of the different states the game can be in.
    /// This class helps encapsulate all of them and ease use and switching of different states.
    class GameStates
    {
    public:
        GameState Update(
            HARDWARE::GamingHardware& gaming_hardware,
            GRAPHICS::Camera& camera);
        sf::Sprite Render(HARDWARE::GamingHardware& gaming_hardware, GRAPHICS::Renderer& renderer);
        void SwitchStatesIfChanged(
            const GameState& new_state, 
            const std::shared_ptr<MAPS::World>& world,
            GRAPHICS::Renderer& renderer);

        /// The current state the game is in.
        GameState CurrentGameState = GameState::INTRO_SEQUENCE;
        /// The intro sequence for the game.
        IntroSequence IntroSequence = {};
        /// The title screen for the game.
        TitleScreen TitleScreen = {};
        /// The credits screen for the game.
        CreditsScreen CreditsScreen = {};
        /// The game selection screen.
        GameSelectionScreen GameSelectionScreen = {};
        /// The new game intro sequence.
        NewGameIntroSequence NewGameIntroSequence = {};
        /// The main gameplay state.
        /// @todo   Split this into pre-flood and during-flood states?
        GameplayState GameplayState = {};
        /// The flood cutscene in-between the first and second halves of gameplay.
        FloodCutscene FloodCutscene = {};
    };
}
