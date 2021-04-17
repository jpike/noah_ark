#pragma once

#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Inventory/InventoryGui.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace GRAPHICS::GUI
{
    /// The heads-up-display (HUD) for the game during the flood.
    class DuringFloodHeadsUpDisplay
    {
    public:
        // PUBLIC METHODS.
        STATES::GameState Update(
            const STATES::SavedGameData& current_game_data,
            const HARDWARE::GamingHardware& gaming_hardware);

        void Render(
            const STATES::SavedGameData& current_game_data,
            GRAPHICS::Renderer& renderer);

        bool ModalComponentDisplayed() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// True if the pause menu box is visible; false otherwise.
        bool PauseMenuVisible = false;
        /// True if the full inventory GUI is displayed; false otherwise.
        bool InventoryOpened = false;
        /// The GUI for displaying the full inventory.
        INVENTORY::InventoryGui InventoryGui = {};
    };
}
