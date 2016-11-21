#pragma once

#include <memory>
#include "Bible/BibleVerses.h"
#include "Graphics/Gui/Font.h"
#include "Graphics/Renderer.h"
#include "Math/Rectangle.h"

namespace GRAPHICS
{
namespace GUI
{
    /// A text box for displaying Bible verses in the player's
    /// inventory.  The text box supports wrapping text
    /// automatically across multiple lines.
    class InventoryBibleVerseTextBox
    {
    public:
        // CONSTRUCTION.
        explicit InventoryBibleVerseTextBox(
            const std::shared_ptr<const Font>& font);

        // RENDERING.
        void Render(
            const BIBLE::BibleVerse* const bible_verse,
            const MATH::FloatRectangle& bounding_rectangle,
            GRAPHICS::Renderer& renderer) const;

    private:
        // MEMBER VARIABLES.
        /// The font to use for rendering text.
        std::shared_ptr<const Font> Font;
    };
}
}
