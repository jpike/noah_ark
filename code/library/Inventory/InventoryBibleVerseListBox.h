#pragma once

#include <memory>
#include <set>
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
        // RENDERING.
        void Render(
            const std::set<BIBLE::BibleVerse>& collected_bible_verses,
            const MATH::FloatRectangle& bounding_rectangle,
            GRAPHICS::Renderer& renderer) const;

        // VERSE METHODS.
        void SelectPreviousVerse();
        void SelectNextVerse();

        // MEMBER VARIABLES.
        /// The index of the currently selected Bible verse in the global list.
        unsigned int SelectedVerseIndex = 0;
    };
}
