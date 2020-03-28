#include <algorithm>
#include "Core/NullChecking.h"
#include "Graphics/Color.h"
#include "Graphics/Gui/Glyph.h"
#include "Graphics/Renderer.h"
#include "Inventory/InventoryBibleVerseListBox.h"

namespace INVENTORY
{
    /// Constructor.
    /// @param[in]  inventory - The inventory indicating which
    ///     Bible verses have been collected.
    /// @throws std::exception - Thrown if any parameters are null.
    InventoryBibleVerseListBox::InventoryBibleVerseListBox(const std::shared_ptr<const INVENTORY::Inventory>& inventory) :
    Inventory(inventory),
    SelectedVerseIndex(0)
    {
        // MAKE SURE REQUIRED PARAMETERS WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Inventory,
            "Inventory cannot be null for Bible verse list box.");
    }

    /// Renders the text box, showing which Bible verse is selected
    /// along with a few surrounding verses.
    /// @param[in]  bounding_rectangle - The bounding rectangle
    ///     of this text box (in screen coordinates).
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryBibleVerseListBox::Render(
        const MATH::FloatRectangle& bounding_rectangle,
        GRAPHICS::Renderer& renderer) const
    {
        // RENDER THE BACKGROUND BOX.
        GRAPHICS::Color background_color = GRAPHICS::Color::BROWN;
        renderer.RenderScreenRectangle(
            bounding_rectangle,
            background_color);

        // DETERMINE THE LIST OF VERSES TO RENDER.
        // The list should generally have the currently selected verse in the middle and
        // show about an even number of verses on each side to fill the remaining space in the box.
        unsigned int box_height_in_pixels = static_cast<unsigned int>(bounding_rectangle.Height());
        const unsigned int ONE_LESS_VERSE_TO_AVOID_EXCEEDING_BOX_BOUNDS = 1;
        unsigned int verses_to_render_count = (box_height_in_pixels / GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS) - ONE_LESS_VERSE_TO_AVOID_EXCEEDING_BOX_BOUNDS;
        unsigned int verses_to_render_half_count = verses_to_render_count / 2;

        // If there aren't enough verses before the selected verse, just start rendering
        // with the first possible verse.  This effectively prevents "scrolling" within
        // the list box from occurring until it is becomes more necessary to start showing
        // more verses.
        unsigned int first_verse_to_render_index = 0;
        bool enough_previous_verses = (SelectedVerseIndex > verses_to_render_half_count);
        if (enough_previous_verses)
        {
            // Start rendering verses prior to the selected one so that the selected verse
            // appears in the middle, allowing players to more easily see what surrounding
            // verses might exist.
            first_verse_to_render_index = SelectedVerseIndex - verses_to_render_half_count;
        }

        size_t last_verse_to_render_index = first_verse_to_render_index + verses_to_render_count;
        size_t last_valid_verse_index = BIBLE::BIBLE_VERSES.size() - 1;
        last_verse_to_render_index = std::min(last_verse_to_render_index, last_valid_verse_index);

        // RENDER THE LIST OF VERSES.
        MATH::Vector2f current_verse_screen_top_left_position_in_pixels = bounding_rectangle.LeftTop;
        for (unsigned int verse_index = first_verse_to_render_index;
            verse_index <= last_verse_to_render_index;
            ++verse_index)
        {
            // CHECK IF THE CURRENT VERSE IS THE SELECTED VERSE.
            bool current_verse_is_selected = (SelectedVerseIndex == verse_index);
            if (current_verse_is_selected)
            {
                // RENDER A DARKER BOX FOR THE SELECTED VERSE.
                GRAPHICS::Color selected_verse_background_color = GRAPHICS::Color::RED_BROWN;
                MATH::FloatRectangle selected_verse_background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    bounding_rectangle.LeftTop.X,
                    current_verse_screen_top_left_position_in_pixels.Y,
                    bounding_rectangle.Width(),
                    static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS));
                renderer.RenderScreenRectangle(
                    selected_verse_background_rectangle,
                    selected_verse_background_color);
            }

            // GET THE TEXT TO RENDER FOR THE VERSE.
            // The Bible's citation string should only be visible if
            // the player has collected the vese.
            const BIBLE::BibleVerse& bible_verse = BIBLE::BIBLE_VERSES.at(verse_index);
            std::string bible_verse_display_string;
            auto verse_from_inventory = Inventory->BibleVerses.find(bible_verse);
            bool player_collected_verse = (verse_from_inventory != Inventory->BibleVerses.cend());
            if (player_collected_verse)
            {
                // GET THE CITATION STRING.
                bible_verse_display_string = bible_verse.GetCitationString();
            }
            else
            {
                // USE QUESTION MARKS FOR THE BIBLE VERSE.
                unsigned int box_width_in_pixels = static_cast<unsigned int>(bounding_rectangle.Width());
                unsigned int character_count_per_line = (box_width_in_pixels / GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS);
                bible_verse_display_string.assign(character_count_per_line, '?');
            }

            // RENDER TEXT FOR THE VERSE.
            renderer.RenderText(
                bible_verse_display_string, 
                RESOURCES::AssetId::FONT_TEXTURE, 
                current_verse_screen_top_left_position_in_pixels, 
                GRAPHICS::Color::BLACK);

            // MOVE TO A NEW LINE FOR THE NEXT VERSE.
            current_verse_screen_top_left_position_in_pixels.Y += GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS;
        }
    }
    
    /// Gets the currently selected verse, if available in the inventory.
    /// @return The currently selected verse, if available in the inventory;
    ///     null otherwise.
    const BIBLE::BibleVerse* InventoryBibleVerseListBox::GetSelectedVerse() const
    {
        // MAKE SURE THE SELECTED VERSE IS IN THE PLAYER'S INVENTORY.
        // Players need to collect verses before they can be fully viewed
        // via the inventory GUI.
        const BIBLE::BibleVerse& selected_verse = BIBLE::BIBLE_VERSES.at(SelectedVerseIndex);
        auto verse_from_inventory = Inventory->BibleVerses.find(selected_verse);
        bool verse_in_inventory = (verse_from_inventory != Inventory->BibleVerses.cend());
        if (verse_in_inventory)
        {
            return &selected_verse;
        }
        else
        {
            return nullptr;
        }
    }

    /// Selects the previous verse in the list, if one exists.
    /// If a previous verse does not exist, the currently selected verse remains.
    void InventoryBibleVerseListBox::SelectPreviousVerse()
    {
        // If subtracting 1 causes the verse index to go negative,
        // the number will actually be a larger positive number
        // (due to being an unsigned integer), so the min function
        // call will work to ensure the selected verse doesn't
        // go below the verse at index 0.
        unsigned int previous_verse_index = SelectedVerseIndex - 1;
        SelectedVerseIndex = std::min(SelectedVerseIndex, previous_verse_index);
    }
    
    /// Selects the next verse in the list, if one exists.
    /// If a next verse does not exist, the currently selected verse remains.
    void InventoryBibleVerseListBox::SelectNextVerse()
    {
        size_t next_verse_index = SelectedVerseIndex + 1;
        size_t last_valid_verse_index = BIBLE::BIBLE_VERSES.size() - 1;
        SelectedVerseIndex = static_cast<unsigned int>(std::min(next_verse_index, last_valid_verse_index));
    }
}
