#pragma once

#include <memory>
#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryBibleVerseListBox.h"
#include "Inventory/InventoryBibleVerseTextBox.h"

namespace INVENTORY
{
    /// The page of the inventory GUI that displays collected Bible verses.
    class InventoryBiblePage
    {
    public:
        // STATIC CONSTANTS.
        /// The background color of the Bible page.
        static const GRAPHICS::Color BACKGROUND_COLOR;

        // CONSTRUCTION.
        explicit InventoryBiblePage(const std::shared_ptr<const Inventory>& inventory);

        // RENDERING.
        void Render(GRAPHICS::Renderer& renderer) const;

        // VERSE SELECTION.
        void SelectPreviousVerse();
        void SelectNextVerse();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The GUI text box for displaying the currently selected Bible verse
        /// in the player's inventory.
        InventoryBibleVerseTextBox BibleVerseTextBox;
        /// The GUI list box for displaying a list of Bible verses in the
        /// player's inventory.
        InventoryBibleVerseListBox BibleVerseListBox;
    };
}
