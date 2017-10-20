#pragma once

#include <memory>
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/TextBox.h"
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
#include "Inventory/InventoryGui.h"
#include "Maps/Overworld.h"
#include "Objects/Noah.h"
#include "Resources/Assets.h"

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
        explicit HeadsUpDisplay(
            const std::shared_ptr<MAPS::Overworld>& overworld,
            const std::shared_ptr<OBJECTS::Noah>& noah_player,
            const unsigned int main_text_box_width_in_pixels,
            const unsigned int main_text_box_height_in_pixels,
            const std::shared_ptr<RESOURCES::Assets>& assets);

        // INPUT.
        void RespondToInput(const INPUT_CONTROL::InputController& input_controller);

        // RENDERING.
        void Render(GRAPHICS::Renderer& renderer) const;

        // OTHER METHODS.
        bool ModalComponentDisplayed() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The main text box for displaying messages to the player.
        GRAPHICS::GUI::TextBox MainTextBox;
        /// True if the full inventory GUI is displayed; false otherwise.
        bool InventoryOpened;
        /// The GUI for displaying the full inventory.
        INVENTORY::InventoryGui InventoryGui;

    private:
        // PRIVATE MEMBER VARIABLES.
        /// True if the save dialog box is visible; false otherwise.
        bool SaveDialogBoxVisible;
        /// The assets to use for the HUD.
        std::shared_ptr<RESOURCES::Assets> Assets;
        /// The overworld whose information is being displayed in the HUD.
        std::shared_ptr<MAPS::Overworld> Overworld;
        /// The player whose information is being displayed in the HUD.
        std::shared_ptr<OBJECTS::Noah> NoahPlayer;
    };
}
}
