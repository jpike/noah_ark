#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Bible/BibleVerses.h"
#include "Graphics/Gui/Font.h"
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
        /// Constructor.
        /// @param[in]  inventory - The inventory indicating which
        ///     Bible verses have been collected.
        /// @param[in]  font - The font to use for rendering text in the box.
        /// @throws std::exception - Thrown if any parameters are null.
        explicit InventoryBibleVerseListBox(
            const std::shared_ptr<const OBJECTS::Inventory>& inventory,
            const std::shared_ptr<const Font>& font);

        // RENDERING.
        /// Renders the text box, showing which Bible verse is selected
        /// along with a few surrounding verses.
        /// @param[in]  bounding_rectangle - The bounding rectangle
        ///     of this text box (in screen coordinates).
        /// @param[in,out]  render_target - The target to render to.
        void Render(
            const MATH::FloatRectangle& bounding_rectangle,
            sf::RenderTarget& render_target);

        // VERSE METHODS.
        /// Gets the currently selected verse, if available in the inventory.
        /// @return The currently selected verse, if available in the inventory;
        ///     null otherwise.
        const BIBLE::BibleVerse* const GetSelectedVerse() const;
        /// Selects the previous verse in the list, if one exists.
        /// If a previous verse does not exist, the currently selected verse remains.
        void SelectPreviousVerse();
        /// Selects the next verse in the list, if one exists.
        /// If a next verse does not exist, the currently selected verse remains.
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
