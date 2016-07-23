#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Bible/BibleVerses.h"
#include "Graphics/Gui/Font.h"
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
        /// Constructor.
        /// @param[in]  font - The font to use for rendering text in the box.
        /// @throws std::exception - Thrown if the font is null.
        explicit InventoryBibleVerseTextBox(
            const std::shared_ptr<const Font>& font);

        // RENDERING.
        /// Renders the text box, with the provided verse's
        /// text (if available).  Otherwise, just an empty
        /// box is rendered.
        /// @param[in]  bible_verse - The Bible verse to render;
        ///     null if no verse should be rendered.
        /// @param[in]  bounding_rectangle - The bounding rectangle
        ///     of this text box (in screen coordinates).
        /// @param[in,out]  render_target - The target to render to.
        void Render(
            const BIBLE::BibleVerse* const bible_verse,
            const MATH::FloatRectangle& bounding_rectangle,
            sf::RenderTarget& render_target);

    private:
        // MEMBER VARIABLES.
        /// The font to use for rendering text.
        std::shared_ptr<const Font> Font;
    };
}
}
