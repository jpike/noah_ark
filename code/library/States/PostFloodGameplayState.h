#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/Camera.h"
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
            AUDIO::Speakers& speakers);
        void UpdateCameraWorldView(
            const sf::Time& elapsed_time,
            MAPS::World& world,
            GRAPHICS::Camera& camera,
            AUDIO::Speakers& speakers,
            INPUT_CONTROL::InputController& input_controller,
            MAPS::TileMap& current_tile_map,
            STATES::SavedGameData& current_game_data);
    };
}