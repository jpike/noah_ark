#include "Core/NullChecking.h"
#include "Inventory/InventoryBiblePage.h"

namespace INVENTORY
{
    // The exact color is currently arbitrary.
    const GRAPHICS::Color InventoryBiblePage::BACKGROUND_COLOR = GRAPHICS::Color::ORANGE;

    /// Constructor.
    /// @param[in]  inventory - The inventory to display in the GUI.
    /// @throws std::exception - Thrown if a parameter is null.
    InventoryBiblePage::InventoryBiblePage(const std::shared_ptr<const Inventory>& inventory) :
    BibleVerseTextBox(),
    BibleVerseListBox(inventory)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(inventory, "Null inventory provided to inventory Bible page.");
    }

    /// Renders the inventory GUI Bible page to the provided screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryBiblePage::Render(GRAPHICS::Renderer& renderer) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = renderer.Screen->HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            renderer.Screen->WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        renderer.RenderScreenRectangle(
            background_rectangle,
            BACKGROUND_COLOR);

        // RENDER THE BOX FOR THE MAIN BIBLE VERSE DISPLAY.
        const BIBLE::BibleVerse* const selected_bible_verse = BibleVerseListBox.GetSelectedVerse();

        // The exact positioning/size of this box is tentative.
        const float BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS = 4.0f;
        float bible_verse_text_box_left_screen_position_in_pixels = background_rectangle.LeftTop.X + BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS;
        float bible_verse_text_box_top_screen_position_in_pixels = background_rectangle.LeftTop.Y + BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS;
        float bible_verse_text_box_width_in_pixels = background_rectangle.Width() / 2.0f;
        const float BIBLE_VERSE_TEXT_BOX_BOTH_SIDES_PADDING_IN_PIXELS = 2.0f * BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS;
        float bible_verse_text_box_height_in_pixels = background_rectangle.Height() - BIBLE_VERSE_TEXT_BOX_BOTH_SIDES_PADDING_IN_PIXELS;
        MATH::FloatRectangle bible_verse_text_box_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            bible_verse_text_box_left_screen_position_in_pixels,
            bible_verse_text_box_top_screen_position_in_pixels,
            bible_verse_text_box_width_in_pixels,
            bible_verse_text_box_height_in_pixels);

        BibleVerseTextBox.Render(
            selected_bible_verse,
            bible_verse_text_box_rectangle,
            renderer);

        // RENDER THE BOX FOR THE LIST OF ALL BIBLE VERSES.
        // The exact positioning/size of this box is tentative.
        float bible_verse_list_box_left_screen_position_in_pixels =
            (bible_verse_text_box_rectangle.RightBottom.X + BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS);
        float bible_verse_list_box_top_screen_position_in_pixels = bible_verse_text_box_rectangle.LeftTop.Y;
        // Take up the remaining width (minus padding on the right).
        float bible_verse_list_box_width_in_pixels =
            background_rectangle.Width() - bible_verse_list_box_left_screen_position_in_pixels - BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS;
        float bible_verse_list_box_height_in_pixels = bible_verse_text_box_rectangle.Height();
        MATH::FloatRectangle bible_verse_list_box_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            bible_verse_list_box_left_screen_position_in_pixels,
            bible_verse_list_box_top_screen_position_in_pixels,
            bible_verse_list_box_width_in_pixels,
            bible_verse_list_box_height_in_pixels);

        BibleVerseListBox.Render(
            bible_verse_list_box_rectangle,
            renderer);
    }
}
