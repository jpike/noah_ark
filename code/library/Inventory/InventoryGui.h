#pragma once

#include <memory>
#include <string>
#include <SFML/System.hpp>
#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryAnimalsPage.h"
#include "Inventory/InventoryBiblePage.h"
#include "Inventory/InventoryFoodPage.h"
#include "Math/Vector2.h"

namespace INVENTORY
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
    ///
    /// A user can press a certain key to bring up the main inventory screen,
    /// which fills the screen, except for the standard HUD stuff above
    /// (which will still be visible at the top of the screen).
    ///
    /// There are be 3 tabs in the main inventory screen that a user can
    /// switch between: Bible, Animals, and Food.
    ///
    /// The Bible tab shows text for verses in a pane on the left and
    /// a list of verse on the right.
    ///
    /// The Animals tab will show a grid of animal icon images, along with
    /// counts of how many of each animal type have been found.  An animal
    /// can be selected to bring up more detailed information.
    ///
    /// The Food tab will show a grid of food icon images, along with
    /// counts of how many of each food type have been found.  Hover-over-style
    /// tooltips will show slightly more information for each food icon.
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
        explicit InventoryGui(const std::shared_ptr<const Inventory>& inventory);

        // UPDATING.
        void Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller);

        // RENDERING.
        void Render(GRAPHICS::Renderer& renderer) const;

    private:
        // RENDERING.
        void RenderTab(
            const std::string& tab_text,
            const MATH::Vector2f& left_top_screen_position_in_pixels,
            const GRAPHICS::Color& background_color,
            GRAPHICS::Renderer& renderer) const;

        // MEMBER VARIABLES.
        /// The player's inventory to display in the GUI.
        std::shared_ptr<const Inventory> Inventory;
        /// The type of tab currently being displayed.
        TabType CurrentTab;
        /// The page of the inventory GUI displaying collected Bible verses.
        InventoryBiblePage BiblePage;
        /// The page of the inventory GUI displaying collected animals.
        InventoryAnimalsPage AnimalsPage;
        /// The page of the inventory GUI displaying collected food.
        InventoryFoodPage FoodPage;
    };
}