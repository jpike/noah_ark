#pragma once

#include <memory>
#include "Graphics/Color.h"
#include "Graphics/Gui/Font.h"
/// @todo  Create inventory sub-namespace?
#include "Graphics/Gui/InventoryBibleVerseListBox.h"
#include "Graphics/Gui/InventoryBibleVerseTextBox.h"
#include "Graphics/Screen.h"
#include "Input/KeyboardInputController.h"
#include "Objects/Inventory.h"

namespace GRAPHICS
{
namespace GUI
{
    /// A GUI for displaying a player's inventory.
    /// The GUI is divided into 3 distinct tabs
    /// for different collections in the inventory:
    ///
    /// --------------------------
    /// | Bible | Animals | Food |
    /// --------------------------
    /// | Content for current    |
    /// | tab is displayed here. |
    /// --------------------------
    class InventoryGui
    {
    public:
        // INNER TYPES.
        /// The different types of tabs that may be displayed
        /// in the GUI.
        enum class TabType
        {
            /// The tab for displaying Bible verses.
            /// This is the first enum value since it is also
            /// the default tab that should appear when opening
            /// the GUI.
            BIBLE,
            /// The tab for displaying animals.
            ANIMALS,
            /// The tab for displaying food.
            FOOD
        };

        // CONSTRUCTION.
        explicit InventoryGui(
            const std::shared_ptr<const OBJECTS::Inventory>& inventory,
            const std::shared_ptr<const GRAPHICS::GUI::Font>& font);

        // INPUT.
        void RespondToInput(const INPUT_CONTROL::KeyboardInputController& input_controller);

        // RENDERING.
        void Render(Screen& screen) const;

    private:
        // RENDERING.        
        void RenderBiblePage(Screen& screen) const;
        void RenderAnimalsPage(Screen& screen) const;
        void RenderFoodPage(Screen& screen) const;

        // MEMBER VARIABLES.
        /// The font to use for rendering text.
        std::shared_ptr<const GRAPHICS::GUI::Font> Font;
        /// The player's inventory to display in the GUI.
        std::shared_ptr<const OBJECTS::Inventory> Inventory;
        /// The type of tab currently being displayed.
        TabType CurrentTab;
        /// @todo   Make the colors below constants.
        /// The color of the Bible tab and page.
        GRAPHICS::Color BibleTabColor;
        /// The color of the animals tab and page.
        GRAPHICS::Color AnimalsTabColor;
        /// The color of the food tab and page.
        GRAPHICS::Color FoodTabColor;
        /// The GUI text box for displaying the currently selected Bible verse
        /// in the player's inventory.
        /// @todo   Factor out to some Bible page class?
        InventoryBibleVerseTextBox BibleVerseTextBox;
        /// The GUI list box for displaying a list of Bible verses in the
        /// player's inventory.
        InventoryBibleVerseListBox BibleVerseListBox;
    };
}
}