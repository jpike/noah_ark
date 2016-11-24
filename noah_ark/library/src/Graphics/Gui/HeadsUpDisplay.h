#pragma once

#include <memory>
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/TextBox.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Input/KeyboardInputController.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryGui.h"

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
            const std::shared_ptr<const INVENTORY::Inventory>& inventory,
            const unsigned int main_text_box_width_in_pixels,
            const unsigned int main_text_box_height_in_pixels,
            const std::shared_ptr<const Texture>& axe_texture,
            const std::shared_ptr<const Texture>& wood_texture);

        // INPUT.
        void RespondToInput(const INPUT_CONTROL::KeyboardInputController& input_controller);

        // RENDERING.
        void Render(GRAPHICS::Renderer& renderer) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The main text box for displaying messages to the player.
        GRAPHICS::GUI::TextBox MainTextBox;
        /// True if the full inventory GUI is displayed; false otherwise.
        bool InventoryOpened;
        /// The GUI for displaying the full inventory.
        INVENTORY::InventoryGui InventoryGui;

    private:
        // PRIVATE MEMBER VARIABLES.
        /// The texture to use for rendering an axe icon.
        std::shared_ptr<const Texture> AxeTexture;
        /// The texture to use for rendering a wood icon.
        std::shared_ptr<const Texture> WoodTexture;
        
        /// The player's inventory to display in the HUD.
        std::shared_ptr<const INVENTORY::Inventory> Inventory;
    };
}
}
