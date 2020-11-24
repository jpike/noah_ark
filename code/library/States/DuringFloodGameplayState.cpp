#include "States/DuringFloodGameplayState.h"

namespace STATES
{
    /// Updates the state of the gameplay based on elapsed time and player input.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output for the update.
    /// @param[in,out]  world - The world to update based on gameplay.
    /// @param[in,out]  camera - The camera to be updated based on player actions during this frame.
    /// @return The next game state after updating.
    GameState DuringFloodGameplayState::Update(
        HARDWARE::GamingHardware& gaming_hardware,
        MAPS::World& world,
        GRAPHICS::Camera& camera,
        STATES::SavedGameData& current_game_data)
    {
        /// @todo
        gaming_hardware;
        world;
        camera;
        current_game_data;
        return GameState::DURING_FLOOD_GAMEPLAY;
    }

    /// Renders the current frame of the gameplay state.
    /// @param[in]  world - The world to render.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @return The rendered gameplay state.
    sf::Sprite DuringFloodGameplayState::Render(
        MAPS::World& world,
        STATES::SavedGameData& current_game_data,
        GRAPHICS::Renderer& renderer)
    {
        /// @todo
        world;
        current_game_data;

        // RENDER CONTENT SPECIFIC TO THE CURRENT MAP.
        renderer.Render(*CurrentMapGrid);

        // RENDER THE FINAL SCREEN WITH TIME-OF-DAY LIGHTING.
        /// @todo   Darker due to flood?
        sf::Sprite screen = renderer.RenderFinalScreenWithTimeOfDayShading(); 
        return screen;
    }
}
