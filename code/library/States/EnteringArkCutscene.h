#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/Gui/TextBox.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Maps/TileMap.h"
#include "Maps/World.h"
#include "States/GameState.h"

namespace STATES
{
    /// The game state for a cutscene that shows God instructing Noah to enter the ark
    /// and then him and his family doing so.
    class EnteringArkCutscene
    {
    public:
        // ENUMS.
        /// Different substates this cutscene can be in.
        enum class Substate
        {
            /// The cutscene is fading in.
            FADING_IN = 0,
            /// God's giving instructions to enter into the ark.
            DISPLAYING_INSTRUCTIONS,
            /// Noah and family are entering into ark.
            ENTERING_INTO_ARK,
            /// The cutscene is fading out.
            FADING_OUT
        };

        // INITIALIZATION METHODS.
        void Load(MAPS::World& world, GRAPHICS::Renderer& renderer, HARDWARE::GamingHardware& gaming_hardware);

        // FRAME UPDATING METHODS.
        GameState Update(HARDWARE::GamingHardware& gaming_hardware, MAPS::World& world, STATES::SavedGameData& current_game_data);
        sf::Sprite Render(const MAPS::World& world, GRAPHICS::Renderer& renderer);

    private:
        // CONSTANTS.
        /// The max time for fading in/out of the cutscene, in seconds.
        static constexpr float FADING_MAX_TIME_IN_SECONDS = 2.0f;

        // MEMBER VARIABLES.
        /// The current substate the game is in.
        Substate CurrentSubstate = Substate::FADING_IN;
        /// The elapsed time for the current substate.
        sf::Time ElapsedTimeForCurrentSubstate = sf::Time::Zero;
        /// The text box for displaying instructions.
        GRAPHICS::GUI::TextBox TextBox = GRAPHICS::GUI::TextBox();
        /// The tile map displaying the entrance into the ark.
        MAPS::TileMap* ArkEntranceTileMap = nullptr;
    };
}
