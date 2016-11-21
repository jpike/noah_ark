#include "Core/NullChecking.h"
#include "Core/String.h"
#include "Graphics/Color.h"
#include "Graphics/Gui/InventoryBibleVerseTextBox.h"
#include "Graphics/Renderer.h"

namespace GRAPHICS
{
namespace GUI
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
        /// @todo   Centralize this color.
        GRAPHICS::Color background_color;
        background_color.Red = 128;
        background_color.Green = 64;
        background_color.Blue = 0;
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

        // SPLIT THE VERSE TEXT INTO SEPARATE WORDS.
        unsigned int box_width_in_pixels = static_cast<unsigned int>(bounding_rectangle.GetWidth());
        unsigned int max_character_count_per_line = (box_width_in_pixels / Glyph::WIDTH_IN_PIXELS);
        std::deque<std::string> words = CORE::String::SplitIntoWords(
            bible_verse->Text,
            max_character_count_per_line);

        // GATHER THE WORDS ONTO SEPARATE LINES FOR RENDERING.
        // This helps avoid words from being split up across lines.
        // The list of lines is initialized to have at least 1 line.
        std::vector<std::string> lines = { "" };
        for (const std::string& word : words)
        {
            // CHECK IF THE CURRENT LINE IF THE LINE HAS ROOM FOR THE CURRENT WORD.
            std::string& current_line = lines.back();
            
            unsigned int SPACE_BETWEEN_WORDS_CHARACTER_COUNT = 1;
            unsigned int current_line_with_current_word_character_count = 
                current_line.length() + 
                SPACE_BETWEEN_WORDS_CHARACTER_COUNT + 
                word.length();
            bool current_line_has_room_for_current_word = (current_line_with_current_word_character_count <= max_character_count_per_line);
            if (current_line_has_room_for_current_word)
            {
                // ADD A SPACE AFTER ANY PREVIOUS WORDS BEFORE ADDING THE NEW WORD.
                // A space should be added if the current line is not empty.
                bool current_line_already_has_words = !current_line.empty();
                if (current_line_already_has_words)
                {
                    const char SPACE_BETWEEN_WORDS = ' ';
                    current_line += SPACE_BETWEEN_WORDS;
                }

                // ADD THE CURRENT WORD TO THE CURRENT LINE.
                current_line += word;
            }
            else
            {
                // ADD THE WORD TO A NEW LINE.
                lines.push_back(word);
            }
        }

        // RENDER EACH LINE OF TEXT.
        MATH::Vector2f current_line_screen_top_left_position_in_pixels(
            bounding_rectangle.GetLeftXPosition(),
            bounding_rectangle.GetTopYPosition());
        for (const std::string& line : lines)
        {
            // RENDER THE CURRENT LINE.
            renderer.RenderText(line, current_line_screen_top_left_position_in_pixels, GRAPHICS::Color::BLACK);

            // MOVE TO THE NEXT LINE.
            current_line_screen_top_left_position_in_pixels.Y += Glyph::HEIGHT_IN_PIXELS;
        }
    }
}
}
