#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Camera.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Maps/World.h"
#include "States/CreditsScreen.h"
#include "States/DuringFloodGameplayState.h"
#include "States/EndingCreditsScreen.h"
#include "States/FloodCutscene.h"
#include "States/GameSelectionScreen.h"
#include "States/GameState.h"
#include "States/IntroSequence.h"
#include "States/NewGameInstructionSequence.h"
#include "States/NewGameIntroSequence.h"
#include "States/PostFloodGameplayState.h"
#include "States/PreFloodGameplayState.h"
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
            MAPS::World& world,
            GRAPHICS::GUI::HeadsUpDisplay& hud,
            GRAPHICS::Camera& camera);
        sf::Sprite Render(
            HARDWARE::GamingHardware& gaming_hardware, 
            MAPS::World& world,
            GRAPHICS::GUI::HeadsUpDisplay& hud,
            GRAPHICS::Renderer& renderer);
        void SwitchStatesIfChanged(
            const GameState& new_state, 
            MAPS::World& world,
            HARDWARE::GamingHardware& gaming_hardware,
            GRAPHICS::Renderer& renderer,
            GRAPHICS::GUI::HeadsUpDisplay& hud);

        /// The current saved game being used.
        SavedGameData CurrentSavedGame = SavedGameData::DefaultSavedGameData();
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
        /// The new game instruction sequence.
        NewGameInstructionSequence NewGameInstructionSequence = {};
        /// The main gameplay state.
        PreFloodGameplayState PreFloodGameplayState = {};
        /// The flood cutscene in-between the first and second halves of gameplay.
        FloodCutscene FloodCutscene = {};
        /// The gameplay state during the flood.
        DuringFloodGameplayState DuringFloodGameplayState = {};
        /// The gameplay state after the flood.
        PostFloodGameplayState PostFloodGameplayState = {};
        /// The ending credits screen.
        EndingCreditsScreen EndingCreditsScreen = {};
    };
}
