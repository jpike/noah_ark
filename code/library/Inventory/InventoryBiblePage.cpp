#include "Inventory/InventoryBiblePage.h"

namespace INVENTORY
{
    // The exact color is currently arbitrary.
    const GRAPHICS::Color InventoryBiblePage::BACKGROUND_COLOR = GRAPHICS::Color::ORANGE;

    /// Renders the inventory GUI Bible page to the provided screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryBiblePage::Render(const Inventory& inventory, GRAPHICS::Renderer& renderer) const
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
        const BIBLE::BibleVerse* selected_bible_verse = &BIBLE::BIBLE_VERSES.at(BibleVerseListBox.SelectedVerseIndex);
        auto verse_from_inventory = inventory.BibleVerses.find(*selected_bible_verse);
        bool verse_in_inventory = (verse_from_inventory != inventory.BibleVerses.cend());
        if (!verse_in_inventory)
        {
            selected_bible_verse = nullptr;
        }

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
        constexpr float BIBLE_VERSE_LIST_BOX_ADDITIONAL_SPACE_FOR_SCROLLBAR_IN_PIXELS = 4.0f;
        float bible_verse_list_box_width_in_pixels = (
            background_rectangle.Width() - 
            bible_verse_list_box_left_screen_position_in_pixels - 
            BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS - 
            BIBLE_VERSE_LIST_BOX_ADDITIONAL_SPACE_FOR_SCROLLBAR_IN_PIXELS);
        float bible_verse_list_box_height_in_pixels = bible_verse_text_box_rectangle.Height();
        MATH::FloatRectangle bible_verse_list_box_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            bible_verse_list_box_left_screen_position_in_pixels,
            bible_verse_list_box_top_screen_position_in_pixels,
            bible_verse_list_box_width_in_pixels,
            bible_verse_list_box_height_in_pixels);

        BibleVerseListBox.Render(
            inventory.BibleVerses,
            bible_verse_list_box_rectangle,
            renderer);
    }
}
