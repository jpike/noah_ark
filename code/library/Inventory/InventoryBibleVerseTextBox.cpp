#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Inventory/InventoryBibleVerseTextBox.h"

namespace INVENTORY
{
    /// Renders the text box, with the provided verse's
    /// text (if available).  Otherwise, just an empty
    /// box is rendered.
    /// @param[in]  bible_verse - The Bible verse to render;
    ///     null if no verse should be rendered.
    /// @param[in]  bounding_rectangle - The bounding rectangle
    ///     of this text box (in screen coordinates).
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryBibleVerseTextBox::Render(
        const BIBLE::BibleVerse* const bible_verse,
        const MATH::FloatRectangle& bounding_rectangle,
        GRAPHICS::Renderer& renderer) const
    {
        // RENDER THE BACKGROUND BOX.
        GRAPHICS::Color background_color = GRAPHICS::Color::BROWN;
        renderer.RenderScreenRectangle(
            bounding_rectangle,
            background_color);

        // CHECK IF A BIBLE VERSE EXISTS FOR RENDERING.
        bool bible_verse_exists = (nullptr != bible_verse);
        if (!bible_verse_exists)
        {
            // No verse should be rendered.
            return;
        }

        // RENDER THE BIBLE VERSE'S TEXT.
        renderer.RenderText(bible_verse->Text, RESOURCES::AssetId::FONT_TEXTURE, bounding_rectangle, GRAPHICS::Color::BLACK);
    }
}
