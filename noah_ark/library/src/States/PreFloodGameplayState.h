#pragma once

#include <memory>
#include <vector>
#include <SFML/System.hpp>
#include "Audio/Speakers.h"
#include "Bible/BibleVerse.h"
#include "Graphics/Camera.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
#include "Maps/Gui/TileMapEditorGui.h"
#include "Maps/MapType.h"
#include "Maps/TileMap.h"
#include "Maps/World.h"
#include "Math/RandomNumberGenerator.h"
#include "Objects/Animal.h"
#include "Resources/Assets.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// The main pre-flood state of the game featuring the main gameplay
    /// of the player moving Noah throughout the overworld, prior to the flood.
    class PreFloodGameplayState
    {
    public:
        // CONSTRUCTION.
        explicit PreFloodGameplayState(
            const std::shared_ptr<AUDIO::Speakers>& speakers,
            const std::shared_ptr<RESOURCES::Assets>& assets);

        // INITIALIZATION.
        bool Initialize(
            const unsigned int screen_width_in_pixels,
            const SavedGameData& saved_game_data,
            const std::shared_ptr<MAPS::World>& world);

        // UPDATING.
        void Update(
            const sf::Time& elapsed_time, 
            INPUT_CONTROL::InputController& input_controller,
            GRAPHICS::Camera& camera);

        // RENDERING.
        void Render(GRAPHICS::Renderer& renderer);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The main game world.
        std::shared_ptr<MAPS::World> World;
        /// Noah (the player) character within the game.
        std::shared_ptr<OBJECTS::Noah> NoahPlayer;

    private:
        // INITIALIZATION.
        std::shared_ptr<OBJECTS::Noah> InitializePlayer(const SavedGameData& saved_game_data);
        std::unique_ptr<GRAPHICS::GUI::HeadsUpDisplay> InitializeHud(
            const unsigned int screen_width_in_pixels,
            const std::shared_ptr<MAPS::World>& world,
            const std::shared_ptr<OBJECTS::Noah>& noah_player);

        // WORLD UPDATING.
        void UpdateOverworld(
            const sf::Time& elapsed_time,
            INPUT_CONTROL::InputController& input_controller,
            GRAPHICS::Camera& camera);
        void UpdateArkInterior(
            const INPUT_CONTROL::InputController& input_controller,
            GRAPHICS::Camera& camera);
        void UpdatePlayerBasedOnInput(
            const MAPS::TileMap& current_tile_map,
            const sf::Time& elapsed_time,
            INPUT_CONTROL::InputController& input_controller,
            GRAPHICS::Camera& camera);
        void MoveAnimals(const sf::Time& elapsed_time, MAPS::TileMap& tile_map);
        void UpdateFallingFood(const sf::Time& elapsed_time, MAPS::TileMap& tile_map);

        // COLLISION DETECTION.
        void CollectWoodAndBibleVersesCollidingWithPlayer(
            MAPS::TileMap& tile_map, 
            std::string& message_for_text_box);
        void CollectFoodCollidingWithPlayer(MAPS::TileMap& tile_map);
        void CollectAnimalsCollidingWithPlayer(MAPS::TileMap& tile_map);
        void ChangeMapIfPlayerOnMapExit(MAPS::TileMap& current_tile_map, GRAPHICS::Camera& camera);

        // CAMERA UPDATING.
        void UpdateCameraWorldView(
            const sf::Time& elapsed_time,
            GRAPHICS::Camera& camera,
            INPUT_CONTROL::InputController& input_controller,
            MAPS::TileMap& current_tile_map);

        // MEMBER VARIABLES.
        /// The random number generator.
        MATH::RandomNumberGenerator RandomNumberGenerator;
        /// The speakers out of which audio is played.
        std::shared_ptr<AUDIO::Speakers> Speakers;
        /// Bible verses that still need to be found by the player.
        std::vector<BIBLE::BibleVerse> BibleVersesLeftToFind;
        /// The assets to be used during gameplay.
        std::shared_ptr<RESOURCES::Assets> Assets;
        /// The heads-up display.
        std::unique_ptr<GRAPHICS::GUI::HeadsUpDisplay> Hud;
        /// The current type of map being displayed.
        MAPS::MapType CurrentMap;
        /// The tile map editor GUI.
        MAPS::GUI::TileMapEditorGui TileMapEditorGui;
    };
}