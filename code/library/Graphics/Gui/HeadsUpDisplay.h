#pragma once

#include <memory>
#include <SFML/System.hpp>
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/TextBox.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Input/InputController.h"
#include "Inventory/InventoryGui.h"
#include "Maps/World.h"
#include "Objects/Noah.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace GRAPHICS
{
namespace GUI
{
    /// The main heads-up-display (HUD) of the game.
    /// Responsible for displaying helpful information
    /// to the player, including information about the
    /// player's inventory.
    class HeadsUpDisplay
    {
    public:
        // CONSTRUCTION.
        explicit HeadsUpDisplay() = default;
        explicit HeadsUpDisplay(
            const std::shared_ptr<GRAPHICS::GUI::Font>& font,
            const unsigned int main_text_box_width_in_pixels,
            const unsigned int main_text_box_height_in_pixels);

        // UPDATE.
        STATES::GameState Update(
            const HARDWARE::GamingHardware& gaming_hardware,
            STATES::SavedGameData& current_game_data,
            const MAPS::World& world);

        // RENDERING.
        void Render(
            const STATES::SavedGameData& current_game_data,
            const GRAPHICS::Color& main_text_color,
            GRAPHICS::Renderer& renderer) const;

        // OTHER METHODS.
        bool ModalComponentDisplayed() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The main text box for displaying messages to the player.
        GRAPHICS::GUI::TextBox MainTextBox = TextBox();
        /// True if the pause menu box is visible; false otherwise.
        bool PauseMenuVisible = false;
        /// True if the full inventory GUI is displayed; false otherwise.
        bool InventoryOpened = false;
        /// The GUI for displaying the full inventory.
        INVENTORY::InventoryGui InventoryGui = {};
    };
}
}
