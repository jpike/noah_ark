#pragma once

#include <memory>
#include <random>
#include <vector>
#include <SFML/System.hpp>
#include "Bible/BibleVerse.h"
#include "Graphics/Camera.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Input/KeyboardInputController.h"
#include "Maps/Overworld.h"
#include "Resources/Assets.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// The main state of the game featuring the main gameplay
    /// of the player moving Noah throughout the overworld.
    class GameplayState
    {
    public:
        // CONSTRUCTION.
        explicit GameplayState(const std::shared_ptr<RESOURCES::Assets>& assets);

        // INITIALIZATION.
        bool Initialize(
            const unsigned int screen_width_in_pixels,
            const SavedGameData& saved_game_data,
            std::unique_ptr<MAPS::Overworld>&& overworld);

        // UPDATING.
        void Update(
            const sf::Time& elapsed_time, 
            INPUT_CONTROL::KeyboardInputController& input_controller,
            GRAPHICS::Camera& camera);

        // RENDERING.
        void Render(const sf::Time& elapsed_time, GRAPHICS::Renderer& renderer);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The main overworld.
        std::unique_ptr<MAPS::Overworld> Overworld;

    private:
        // INITIALIZATION.
        std::unique_ptr<OBJECTS::Noah> InitializePlayer(const SavedGameData& saved_game_data);
        std::unique_ptr<GRAPHICS::GUI::HeadsUpDisplay> InitializeHud(
            const unsigned int screen_width_in_pixels,
            const MAPS::Overworld& overworld);

        // MEMBER VARIABLES.
        /// The random number generator.
        std::random_device RandomNumberGenerator;
        /// Bible verses that still need to be found by the player.
        std::vector<BIBLE::BibleVerse> BibleVersesLeftToFind;
        /// The assets to be used during gameplay.
        std::shared_ptr<RESOURCES::Assets> Assets;
        /// The heads-up display.
        std::unique_ptr<GRAPHICS::GUI::HeadsUpDisplay> Hud;
    };
}