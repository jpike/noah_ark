#pragma once

#include <memory>
#include "Bible/BibleVerses.h"
#include "Graphics/Gui/Font.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Math/Rectangle.h"
#include "Objects/Inventory.h"

namespace GRAPHICS
{
namespace GUI
{
    /// A list box for listing all Bible verses.  Those in a
    /// player's inventory have their book, chapter, and verse
    /// number displayed, but other verses that haven't been
    /// collected yet are just displayed as question marks.
    class InventoryBibleVerseListBox
    {
    public:
        // CONSTRUCTION.
        explicit InventoryBibleVerseListBox(
            const std::shared_ptr<const OBJECTS::Inventory>& inventory,
            const std::shared_ptr<const Font>& font);

        // RENDERING.
        void Render(
            const MATH::FloatRectangle& bounding_rectangle,
            Renderer& renderer) const;

        // VERSE METHODS.
        const BIBLE::BibleVerse* const GetSelectedVerse() const;
        void SelectPreviousVerse();
        void SelectNextVerse();

    private:
        // MEMBER VARIABLES.
        /// The inventory holding collected Bible verses.
        std::shared_ptr<const OBJECTS::Inventory> Inventory;
        /// The font to use for rendering text.
        std::shared_ptr<const Font> Font;
        /// The index of the currently selected Bible verse in the global list.
        unsigned int SelectedVerseIndex;
    };
}
}
