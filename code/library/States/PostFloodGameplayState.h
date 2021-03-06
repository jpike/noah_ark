#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/Camera.h"
#include "Graphics/Gui/TextBox.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Maps/MultiTileMapGrid.h"
#include "Maps/World.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// The state for gameplay taking place after the flood.
    class PostFloodGameplayState
    {
    public:
        // ENUMS.
        /// The different substates of this post-flood gameplay state the game can be in.
        enum class Substate
        {
            /// The state is fading in from the previous state.
            FADING_IN = 0,
            /// The state where Noah and family have just exited the ark,
            /// and Noah needs to build an altar to progress to the next substate.
            JUST_EXITED_ARK,
            /// God is speaking before He puts a rainbow in the sky.
            GOD_SPEAKING_BEFORE_RAINBOW,
            /// God is speaking after He puts a rainbow in the sky.
            GOD_SPEAKING_DURING_RAINBOW,
            /// The state is fading out to the next state.
            FADING_OUT
        };

        // INITIALIZATION METHODS.
        void Load(const STATES::SavedGameData& saved_game_data, MAPS::World& world, GRAPHICS::Renderer& renderer, HARDWARE::GamingHardware& gaming_hardware);

        // UPDATING.
        GameState Update(
            HARDWARE::GamingHardware& gaming_hardware,
            MAPS::World& world,
            GRAPHICS::Camera& camera,
            STATES::SavedGameData& current_game_data);

        // RENDERING.
        sf::Sprite Render(
            MAPS::World& world, 
            GRAPHICS::Renderer& renderer,
            HARDWARE::GamingHardware& gaming_hardware);

    private:
        // CONSTANTS.
        /// The max time for fading in/out of the cutscene, in seconds.
        static constexpr float FADING_MAX_TIME_IN_SECONDS = 2.0f;

        // UPDATING HELPER FUNCTIONS.
        MATH::Vector2f GetAltarBuildPosition(const OBJECTS::Noah& noah_player) const;
        void UpdateMapGrid(
            HARDWARE::GamingHardware& gaming_hardware,
            MAPS::World& world,
            MAPS::MultiTileMapGrid& map_grid,
            GRAPHICS::Camera& camera,
            STATES::SavedGameData& current_game_data);
        void UpdatePlayerBasedOnInput(
            MAPS::World& world,
            MAPS::TileMap& current_tile_map,
            MAPS::MultiTileMapGrid& map_grid,
            GRAPHICS::Camera& camera,
            HARDWARE::GamingHardware& gaming_hardware);
        void UpdateCameraWorldView(
            const sf::Time& elapsed_time,
            GRAPHICS::Camera& camera,
            INPUT_CONTROL::InputController& input_controller,
            MAPS::TileMap& current_tile_map);

        // PRIVATE MEMBER VARIABLES.
        /// The current substate.
        Substate CurrentSubstate = Substate::FADING_IN;
        /// The elapsed time for the current substate.
        sf::Time ElapsedTimeForCurrentSubstate = sf::Time::Zero;
        /// The text box for displaying instructions.
        GRAPHICS::GUI::TextBox TextBox = GRAPHICS::GUI::TextBox();
        /// The current map being displayed within the world.
        MAPS::MultiTileMapGrid* CurrentMapGrid = nullptr;
    };
}