#pragma once

#include <memory>
#include <SFML/System.hpp>
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/TextBox.h"
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
#include "Inventory/InventoryGui.h"
#include "Maps/World.h"
#include "Objects/Noah.h"
#include "Resources/Assets.h"
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
        explicit HeadsUpDisplay(
            const std::shared_ptr<STATES::SavedGameData>& saved_game_data,
            const std::shared_ptr<MAPS::World>& world,
            const std::shared_ptr<OBJECTS::Noah>& noah_player,
            const unsigned int main_text_box_width_in_pixels,
            const unsigned int main_text_box_height_in_pixels,
            const std::shared_ptr<RESOURCES::Assets>& assets);

        // UPDATE.
        void Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller);

        // RENDERING.
        void Render(GRAPHICS::Renderer& renderer) const;

        // OTHER METHODS.
        bool ModalComponentDisplayed() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The main text box for displaying messages to the player.
        GRAPHICS::GUI::TextBox MainTextBox;
        /// The color for text in the main HUD.  This doesn't affect
        /// text displayed in the main text box or the inventory GUI
        /// but only other text displayed directly by this HUD.
        GRAPHICS::Color TextColor;
        /// True if the full inventory GUI is displayed; false otherwise.
        bool InventoryOpened;
        /// The GUI for displaying the full inventory.
        INVENTORY::InventoryGui InventoryGui;

    private:
        // PRIVATE MEMBER VARIABLES.
        /// True if the save dialog box is visible; false otherwise.
        bool SaveDialogBoxVisible;
        /// The current player's saved game.
        std::shared_ptr<STATES::SavedGameData> SavedGame;
        /// The assets to use for the HUD.
        std::shared_ptr<RESOURCES::Assets> Assets;
        /// The world whose information is being displayed in the HUD.
        std::shared_ptr<MAPS::World> World;
        /// The player whose information is being displayed in the HUD.
        std::shared_ptr<OBJECTS::Noah> NoahPlayer;
    };
}
}
