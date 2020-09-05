#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Audio/Speakers.h"
#include "Bible/BibleVerse.h"
#include "Graphics/Camera.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
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
    /// throughout the overworld and inside the ark (before, during, and after
    /// the flood).
    class GameplayState
    {
    public:
        // INITIALIZATION.
        bool Initialize(
            const std::shared_ptr<SavedGameData>& saved_game_data,
            const std::shared_ptr<MAPS::World>& world,
            GRAPHICS::Renderer& renderer);

        // UPDATING.
        GameState Update(
            HARDWARE::GamingHardware& gaming_hardware,
            const std::shared_ptr<MAPS::World>& world,
            GRAPHICS::Camera& camera,
            GRAPHICS::GUI::HeadsUpDisplay& hud);

        // RENDERING.
        sf::Sprite Render(
            const std::shared_ptr<MAPS::World>& world, 
            GRAPHICS::GUI::HeadsUpDisplay& hud,
            GRAPHICS::Renderer& renderer);

    private:
        // WORLD UPDATING.
        void UpdateMapGrid(
            const sf::Time& elapsed_time,
            const std::shared_ptr<MAPS::World>& world,
            INPUT_CONTROL::InputController& input_controller,
            GRAPHICS::Camera& camera,
            AUDIO::Speakers& speakers,
            MAPS::MultiTileMapGrid& map_grid,
            GRAPHICS::GUI::HeadsUpDisplay& hud);
        MAPS::ExitPoint* UpdatePlayerBasedOnInput(
            const sf::Time& elapsed_time,
            INPUT_CONTROL::InputController& input_controller,
            const std::shared_ptr<MAPS::World>& world,
            MAPS::TileMap& current_tile_map,
            MAPS::MultiTileMapGrid& map_grid,
            GRAPHICS::Camera& camera,
            AUDIO::Speakers& speakers);
        void MoveAnimals(
            const sf::Time& elapsed_time, 
            MAPS::TileMap& tile_map, 
            MAPS::MultiTileMapGrid& map_grid,
            const std::shared_ptr<MAPS::World>& world);
        void UpdateFallingFood(const sf::Time& elapsed_time, MAPS::TileMap& tile_map);

        // COLLISION DETECTION.
        void CollectWoodAndBibleVersesCollidingWithPlayer(
            MAPS::TileMap& tile_map,
            MAPS::MultiTileMapGrid& map_grid,
            const std::shared_ptr<MAPS::World>& world,
            AUDIO::Speakers& speakers,
            std::string& message_for_text_box);
        void CollectFoodCollidingWithPlayer(
            const std::shared_ptr<MAPS::World>& world, 
            MAPS::TileMap& tile_map, 
            AUDIO::Speakers& speakers);
        void CollectAnimalsCollidingWithPlayer(
            const std::shared_ptr<MAPS::World>& world, 
            MAPS::TileMap& tile_map, 
            AUDIO::Speakers& speakers);

        // CAMERA UPDATING.
        void UpdateCameraWorldView(
            const sf::Time& elapsed_time,
            const std::shared_ptr<MAPS::World>& world,
            GRAPHICS::Camera& camera,
            AUDIO::Speakers& speakers,
            INPUT_CONTROL::InputController& input_controller,
            MAPS::TileMap& current_tile_map);

        // PRIVATE MEMBER VARIABLES.
        /// The current map being displayed within the world.
        MAPS::MultiTileMapGrid* CurrentMapGrid = nullptr;
        /// Bible verses that still need to be found by the player.
        std::vector<BIBLE::BibleVerse> BibleVersesLeftToFind = {};
        /// The tile map editor GUI.
        MAPS::GUI::TileMapEditorGui TileMapEditorGui = {};
        /// Animals being transferred from following Noah into the ark.
        /// @todo   Probably best to not have this in the game state and instead just
        /// have animals have different "modes" of behavior.
        std::vector<std::shared_ptr<OBJECTS::Animal>> AnimalsGoingIntoArk = {};
        /// The random number generator.
        MATH::RandomNumberGenerator RandomNumberGenerator = {};
    };
}