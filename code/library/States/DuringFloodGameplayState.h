#pragma once

#include <SFML/Graphics.hpp>
#include "Graphics/Camera.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Maps/MultiTileMapGrid.h"
#include "Maps/World.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace STATES
{
    /// The gameplay state during the flood, where everyone is confined to the ark.
    class DuringFloodGameplayState
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
        /// The HUD.
        GRAPHICS::GUI::HeadsUpDisplay Hud = GRAPHICS::GUI::HeadsUpDisplay();
    };
}
