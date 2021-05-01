#pragma once

#include <memory>
#include <tuple>
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/TextBox.h"
#include "Graphics/Renderer.h"
#include "Hardware/GamingHardware.h"
#include "Inventory/InventoryGui.h"
#include "Objects/Food.h"
#include "States/GameState.h"
#include "States/SavedGameData.h"

namespace GRAPHICS::GUI
{
    /// Special actions that can be performed on specific days of the flood.
    /// @todo   Factor out to separate file?
    enum class SpecialDayAction
    {
        NONE = 0,
        CURRENT_DAY_ACTION_COMPLETED, // Similar to "none" but indicates one of the other actions has been completed.
        SEND_OUT_RAVEN_FIRST_TIME,
        SEND_OUT_DOVE_FIRST_TIME,
        SEND_OUT_DOVE_SECOND_TIME,
        SEND_OUT_DOVE_FINAL_TIME
    };

    /// The heads-up-display (HUD) for the game during the flood.
    class DuringFloodHeadsUpDisplay
    {
    public:
        // CONSTRUCTION.
        explicit DuringFloodHeadsUpDisplay() = default;
        DuringFloodHeadsUpDisplay(
            const std::shared_ptr<GRAPHICS::GUI::Font>& font,
            const unsigned int main_text_box_width_in_pixels,
            const unsigned int main_text_box_height_in_pixels);

        // OTHER PUBLIC METHODS.
        std::tuple<STATES::GameState, OBJECTS::Food::TypeId> Update(
            const STATES::SavedGameData& current_game_data,
            const HARDWARE::GamingHardware& gaming_hardware);

        void Render(
            const STATES::SavedGameData& current_game_data,
            GRAPHICS::Renderer& renderer);

        bool ModalComponentDisplayed() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The main text box for displaying messages to the player.
        GRAPHICS::GUI::TextBox MainTextBox = TextBox();
        /// The curreny action to take place on a specific day (such as sending the raven/dove out).
        SpecialDayAction CurrentSpecialDayAction = SpecialDayAction::NONE;
        /// True if the pause menu box is visible; false otherwise.
        bool PauseMenuVisible = false;
        /// True if the full inventory GUI is displayed; false otherwise.
        bool InventoryOpened = false;
        /// The GUI for displaying the full inventory.
        INVENTORY::InventoryGui InventoryGui = {};
    };
}
