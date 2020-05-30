#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Audio/Speakers.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
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
        /// @todo   Revisit whether we can get rid of this constructor.
        explicit GameStates(
            const std::shared_ptr<AUDIO::Speakers>& speakers,
            const std::shared_ptr<RESOURCES::Assets>& assets) :
        GameplayState(speakers, assets)
        {}

        GameState Update(
            const sf::Time& elapsed_time, 
            INPUT_CONTROL::InputController& input_controller, 
            GRAPHICS::Camera& camera,
            AUDIO::Speakers& speakers);
        sf::Sprite Render(const sf::Time& total_elapsed_time, GRAPHICS::Renderer& renderer);
        void SwitchStatesIfChanged(const GameState& new_state, const std::shared_ptr<MAPS::World>& world, GRAPHICS::Renderer& renderer);

        /// The current state the game is in.
        GameState CurrentGameState = GameState::INTRO_SEQUENCE;
        /// The intro sequence for the game.
        IntroSequence IntroSequence;
        /// The title screen for the game.
        TitleScreen TitleScreen;
        /// The credits screen for the game.
        CreditsScreen CreditsScreen;
        /// The game selection screen.
        GameSelectionScreen GameSelectionScreen;
        /// The new game intro sequence.
        NewGameIntroSequence NewGameIntroSequence;
        /// The main gameplay state.
        /// @todo   Split this into pre-flood and during-flood states?
        GameplayState GameplayState;
        /// The flood cutscene in-between the first and second halves of gameplay.
        FloodCutscene FloodCutscene;
    };
}
