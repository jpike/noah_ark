#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Color.h"
#include "Graphics/Gui/Font.h"
/// #@todo  Create inventory sub-namespace?
#include "Graphics/Gui/InventoryBibleVerseListBox.h"
#include "Graphics/Gui/InventoryBibleVerseTextBox.h"
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
        /// Constructor.
        /// @param[in]  inventory - The inventory to display in the GUI.
        /// @param[in]  font - The font to use for rendering text on the GUI.
        /// @throws std::exception - Thrown if a parameter is null.
        /// @todo   Re-think how we pass assets to this class.
        explicit InventoryGui(
            const std::shared_ptr<const OBJECTS::Inventory>& inventory,
            const std::shared_ptr<const GRAPHICS::GUI::Font>& font);

        // INPUT.
        /// Has the inventory GUI respond to the provided key being pressed.
        /// @param[in]  key - The key that was pressed.
        void RespondToInput(const sf::Keyboard::Key key);

        // RENDERING.
        /// Renders the inventory GUI to the provided target.
        /// @param[in]  render_target - The target to render to.
        void Render(sf::RenderTarget& render_target) const;

    private:
        // RENDERING.
        /// Renders the page of the inventory for the Bible tab.
        /// This page allows browsing Bible verses in the inventory.
        /// @param[in]  render_target - The target to render to.
        void RenderBiblePage(sf::RenderTarget& render_target) const;
        /// Renders the page of the inventory for the animals tab.
        /// This page allows browsing animals in the inventory.
        /// @param[in]  render_target - The target to render to.
        void RenderAnimalsPage(sf::RenderTarget& render_target) const;
        /// Renders the page of the inventory for the food tab.
        /// This page allows browsing food in the inventory.
        /// @param[in]  render_target - The target to render to.
        void RenderFoodPage(sf::RenderTarget& render_target) const;

        // MEMBER VARIABLES.
        /// The font to use for rendering text.
        std::shared_ptr<const GRAPHICS::GUI::Font> Font;
        /// The player's inventory to display in the GUI.
        std::shared_ptr<const OBJECTS::Inventory> Inventory;
        /// The type of tab currently being displayed.
        TabType CurrentTab;
        /// \todo   Make the colors below constants.
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