#pragma once

#include <memory>
#include "Bible/BibleVerses.h"
#include "Graphics/Renderer.h"
#include "Inventory/Inventory.h"
#include "Math/Rectangle.h"

namespace INVENTORY
{
    /// A list box for listing all Bible verses.  Those in a
    /// player's inventory have their book, chapter, and verse
    /// number displayed, but other verses that haven't been
    /// collected yet are just displayed as question marks.
    class InventoryBibleVerseListBox
    {
    public:
        // CONSTRUCTION.
        explicit InventoryBibleVerseListBox(const std::shared_ptr<const Inventory>& inventory);

        // RENDERING.
        void Render(
            const MATH::FloatRectangle& bounding_rectangle,
            GRAPHICS::Renderer& renderer) const;

        // VERSE METHODS.
        const BIBLE::BibleVerse* const GetSelectedVerse() const;
        void SelectPreviousVerse();
        void SelectNextVerse();

    private:
        // MEMBER VARIABLES.
        /// The inventory holding collected Bible verses.
        std::shared_ptr<const Inventory> Inventory;
        /// The index of the currently selected Bible verse in the global list.
        unsigned int SelectedVerseIndex;
    };
}
