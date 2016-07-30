#include <algorithm>
#include <iostream>
#include "Core/NullChecking.h"
#include "Graphics/Color.h"
#include "Graphics/Gui/Glyph.h"
#include "Graphics/Gui/InventoryBibleVerseListBox.h"
#include "Graphics/Gui/Text.h"
#include "Graphics/Rendering.h"

namespace GRAPHICS
{
namespace GUI
{
    InventoryBibleVerseListBox::InventoryBibleVerseListBox(
        const std::shared_ptr<const OBJECTS::Inventory>& inventory,
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font) :
    Inventory(inventory),
    Font(font),
    SelectedVerseIndex(0)
    {
        // MAKE SURE REQUIRED PARAMETERS WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Inventory,
            "Inventory cannot be null for Bible verse list box.");
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Font,
            "Font cannot be null for Bible verse list box.");
    }

    void InventoryBibleVerseListBox::Render(
        const MATH::FloatRectangle& bounding_rectangle,
        sf::RenderTarget& render_target) const
    {
        // RENDER THE BACKGROUND BOX.
        /// @todo   Centralize this color.
        GRAPHICS::Color background_color;
        background_color.Red = 128;
        background_color.Green = 64;
        background_color.Blue = 0;
        RenderScreenRectangle(
            bounding_rectangle,
            background_color,
            render_target);

        // RENDER A DARKER BOX FOR THE SELECTED VERSE.
        GRAPHICS::Color selected_verse_background_color = background_color;
        selected_verse_background_color.Green = 32;
        MATH::FloatRectangle selected_verse_background_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            bounding_rectangle.GetLeftXPosition(),
            bounding_rectangle.GetTopYPosition(),
            bounding_rectangle.GetWidth(),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));
        RenderScreenRectangle(
            selected_verse_background_rectangle,
            selected_verse_background_color,
            render_target);

        // DETERMINE THE LIST OF VERSES TO RENDER.
        // The list should start at the currently selected verse
        // and then fill the remaining space in the box.
        /// @todo   Wrap around to show some number of verses centered around the current one?
        unsigned int box_height_in_pixels = static_cast<unsigned int>(bounding_rectangle.GetHeight());
        const unsigned int ONE_LESS_VERSE_TO_AVOID_EXCEEDING_BOX_BOUNDS = 1;
        unsigned int verses_to_render_count = (box_height_in_pixels / Glyph::HEIGHT_IN_PIXELS) - ONE_LESS_VERSE_TO_AVOID_EXCEEDING_BOX_BOUNDS;
        unsigned int first_verse_to_render_index = SelectedVerseIndex;
        unsigned int last_verse_to_render_index = first_verse_to_render_index + verses_to_render_count;
        unsigned int last_valid_verse_index = BIBLE::BIBLE_VERSES.size() - 1;
        last_verse_to_render_index = std::min(last_verse_to_render_index, last_valid_verse_index);

        // RENDER THE LIST OF VERSES.
        MATH::Vector2ui current_verse_screen_top_left_position_in_pixels(
            static_cast<unsigned int>(bounding_rectangle.GetLeftXPosition()),
            static_cast<unsigned int>(bounding_rectangle.GetTopYPosition()));
        for (unsigned int verse_index = first_verse_to_render_index;
            verse_index <= last_verse_to_render_index;
            ++verse_index)
        {
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
                unsigned int box_width_in_pixels = static_cast<unsigned int>(bounding_rectangle.GetWidth());
                unsigned int character_count_per_line = (box_width_in_pixels / Glyph::WIDTH_IN_PIXELS);
                bible_verse_display_string.assign(character_count_per_line, '?');
            }

            // RENDER TEXT FOR THE VERSE.
            Text bible_verse_text(Font, bible_verse_display_string, current_verse_screen_top_left_position_in_pixels);
            bible_verse_text.Render(render_target);

            // MOVE TO A NEW LINE FOR THE NEXT VERSE.
            current_verse_screen_top_left_position_in_pixels.Y += Glyph::HEIGHT_IN_PIXELS;
        }
    }
    
    const BIBLE::BibleVerse* const InventoryBibleVerseListBox::GetSelectedVerse() const
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

    void InventoryBibleVerseListBox::SelectPreviousVerse()
    {
        // If subtracting 1 causes the verse index to go negative,
        // the number will actually be a larger positive number
        // (due to being an unsigned integer), so the min function
        // call will work to ensure the selected verse doesn't
        // go below the verse at index 0.
        unsigned int previous_verse_index = SelectedVerseIndex - 1;
        SelectedVerseIndex = std::min(SelectedVerseIndex, previous_verse_index);
        std::cout << "SelectedVerseIndex: " << SelectedVerseIndex << std::endl;
    }
    
    void InventoryBibleVerseListBox::SelectNextVerse()
    {
        unsigned int next_verse_index = SelectedVerseIndex + 1;
        unsigned int last_valid_verse_index = BIBLE::BIBLE_VERSES.size() - 1;
        SelectedVerseIndex = std::min(next_verse_index, last_valid_verse_index);
        std::cout << "SelectedVerseIndex: " << SelectedVerseIndex << std::endl;
    }
}
}
