#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Maps/World.h"
#include "States/CreditsScreen.h"
#include "States/DuringFloodGameplayState.h"
#include "States/EndingCreditsScreen.h"
#include "States/EnteringArkCutscene.h"
#include "States/FloodCutscene.h"
#include "States/GameSelectionScreen.h"
#include "States/GameState.h"
#include "States/IntroSequence.h"
#include "States/NewGameInstructionSequence.h"
#include "States/NewGameIntroSequence.h"
#include "States/NoticeScreen.h"
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
            MAPS::World& world,
            GRAPHICS::Camera& camera,
            HARDWARE::GamingHardware& gaming_hardware);
        sf::Sprite Render(
            MAPS::World& world,
            GRAPHICS::Renderer& renderer,
            HARDWARE::GamingHardware& gaming_hardware);
        void SwitchStatesIfChanged(
            const GameState& new_state, 
            MAPS::World& world,
            GRAPHICS::Renderer& renderer,
            HARDWARE::GamingHardware& gaming_hardware);

        /// The current saved game being used.
        SavedGameData CurrentSavedGame = SavedGameData::DefaultSavedGameData();
        /// The intro sequence for the game.
        IntroSequence IntroSequence = {};
        /// The title screen for the game.
        TitleScreen TitleScreen = {};
        /// The credits screen for the game.
        CreditsScreen CreditsScreen = {};
        /// The notice screen for the game.
        NoticeScreen NoticeScreen = {};
        /// The game selection screen.
        GameSelectionScreen GameSelectionScreen = {};
        /// The new game intro sequence.
        NewGameIntroSequence NewGameIntroSequence = {};
        /// The new game instruction sequence.
        NewGameInstructionSequence NewGameInstructionSequence = STATES::NewGameInstructionSequence();
        /// The main gameplay state.
        PreFloodGameplayState PreFloodGameplayState = {};
        /// A short cutscene showing Noah and family entering the ark.
        EnteringArkCutscene EnteringArkCutscene = {};
        /// The flood cutscene in-between the first and second halves of gameplay.
        FloodCutscene FloodCutscene = STATES::FloodCutscene();
        /// The gameplay state during the flood.
        DuringFloodGameplayState DuringFloodGameplayState = {};
        /// The gameplay state after the flood.
        PostFloodGameplayState PostFloodGameplayState = {};
        /// The ending credits screen.
        EndingCreditsScreen EndingCreditsScreen = {};
    };
}
