#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Audio/Speakers.h"
#include "Bible/BibleVerse.h"
#include "Graphics/Camera.h"
#include "Graphics/Gui/PreFloodHeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Input/InputController.h"
#include "Maps/ExitPoint.h"
#include "Maps/Gui/TileMapEditorGui.h"
#include "Maps/TileMap.h"
#include "Maps/World.h"
#include "Math/RandomNumberGenerator.h"
#include "Objects/Animal.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// The state of the game featuring the main gameplay of the player moving Noah
    /// throughout the overworld and inside the ark (before the flood).
    class PreFloodGameplayState
    {
    public:
        // INITIALIZATION.
        void Load(
            const SavedGameData& saved_game_data,
            MAPS::World& world,
            GRAPHICS::Renderer& renderer,
            MATH::RandomNumberGenerator& random_number_generator);

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

    private:
        // WORLD UPDATING.
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
            STATES::SavedGameData& game_data);

        // COLLISION DETECTION.
        void CollectWoodAndBibleVersesCollidingWithPlayer(
            MAPS::TileMap& tile_map,
            MAPS::MultiTileMapGrid& map_grid,
            MAPS::World& world,
            AUDIO::Speakers& speakers,
            STATES::SavedGameData& current_game_data,
            std::string& message_for_text_box);
        void CollectFoodCollidingWithPlayer(
            MAPS::World& world, 
            MAPS::TileMap& tile_map, 
            AUDIO::Speakers& speakers);
        void CollectAnimalsCollidingWithPlayer(
            MAPS::World& world, 
            MAPS::TileMap& tile_map, 
            AUDIO::Speakers& speakers,
            STATES::SavedGameData& current_game_data);

        // CAMERA UPDATING.
        void UpdateCameraWorldView(
            const sf::Time& elapsed_time,
            MAPS::World& world,
            GRAPHICS::Camera& camera,
            AUDIO::Speakers& speakers,
            INPUT_CONTROL::InputController& input_controller,
            MAPS::TileMap& current_tile_map,
            STATES::SavedGameData& current_game_data);

        // PRIVATE MEMBER VARIABLES.
        /// The current map being displayed within the world.
        MAPS::MultiTileMapGrid* CurrentMapGrid = nullptr;
        /// The HUD.
        GRAPHICS::GUI::PreFloodHeadsUpDisplay Hud = GRAPHICS::GUI::PreFloodHeadsUpDisplay();
        /// The tile map editor GUI.
        MAPS::GUI::TileMapEditorGui TileMapEditorGui = {};
        /// The random number generator.
        MATH::RandomNumberGenerator RandomNumberGenerator = {};
    };
}