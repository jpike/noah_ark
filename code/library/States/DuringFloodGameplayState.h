#pragma once

#include <optional>
#include <SFML/Graphics.hpp>
#include "Graphics/Camera.h"
#include "Graphics/Gui/DuringFloodHeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Maps/MultiTileMapGrid.h"
#include "Maps/World.h"
#include "Objects/BirdSentFromArk.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// The gameplay state during the flood, where everyone is confined to the ark.
    class DuringFloodGameplayState
    {
    public:
        // LOADING.
        void Load(
            const SavedGameData& saved_game_data,
            MAPS::World& world,
            GRAPHICS::Renderer& renderer,
            HARDWARE::GamingHardware& gaming_hardware);

        // UPDATING.
        GameState Update(
            HARDWARE::GamingHardware& gaming_hardware,
            MAPS::World& world,
            GRAPHICS::Camera& camera,
            STATES::SavedGameData& current_game_data);

        // RENDERING.
        sf::Sprite Render(
            MAPS::World& world, 
            STATES::SavedGameData& current_game_data,
            GRAPHICS::Renderer& renderer);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The current map being displayed within the world.
        MAPS::MultiTileMapGrid* CurrentMapGrid = nullptr;

    private:
        // UPDATING HELPER FUNCTIONS.
        void UpdateMapGrid(
            HARDWARE::GamingHardware& gaming_hardware,
            MAPS::World& world,
            MAPS::MultiTileMapGrid& map_grid,
            GRAPHICS::Camera& camera,
            STATES::SavedGameData& current_game_data);
        MAPS::ExitPoint* UpdatePlayerBasedOnInput(
            const sf::Time& elapsed_time,
            INPUT_CONTROL::InputController& input_controller,
            MAPS::World& world,
            MAPS::TileMap& current_tile_map,
            MAPS::MultiTileMapGrid& map_grid,
            GRAPHICS::Camera& camera,
            AUDIO::Speakers& speakers,
            std::string& message_for_text_box);
        void UpdateCameraWorldView(
            const sf::Time& elapsed_time,
            MAPS::World& world,
            GRAPHICS::Camera& camera,
            AUDIO::Speakers& speakers,
            INPUT_CONTROL::InputController& input_controller,
            MAPS::TileMap& current_tile_map,
            STATES::SavedGameData& current_game_data);

        // PRIVATE MEMBER VARIABLES.
        /// The HUD for this game state.
        GRAPHICS::GUI::DuringFloodHeadsUpDisplay Hud = GRAPHICS::GUI::DuringFloodHeadsUpDisplay();
        /// The bird sent from the ark at certain points.
        /// Stored in this state rather than within the world since there is not a needed for it
        /// to be within the world and it is simpler to keep here.
        std::optional<OBJECTS::BirdSentFromArk> BirdSentFromArk = std::nullopt;
    };
}
