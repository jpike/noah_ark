#include <algorithm>
#include <cassert>
#include "Graphics/Gui/Text.h"
#include "Graphics/Gui/TextPage.h"

namespace GRAPHICS
{
namespace GUI
{
    const float TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS = 0.05f;

    bool TextPage::Add(const std::string& word)
    {
        // MAKE SURE THERE IS ROOM FOR THE NEW WORD.
        bool all_text_lines_full = (LastUnfilledLineIndex >= LINE_COUNT);
        if (all_text_lines_full)
        {
            return false;
        }

        // ADD A SPACE BEFORE THE NEW WORD IF THE WORD ISN'T THE FIRST WORD ON THE LINE.
        // Spaces should separate consecutive words, but putting a space before
        // the first word on a line would waste a character slot.
        std::vector<char>& first_unfilled_line_of_text = LinesOfText[LastUnfilledLineIndex];
        std::vector<char>* current_line_of_text = &first_unfilled_line_of_text;
        bool first_word = current_line_of_text->empty();
        if (!first_word)
        {
            // ADD THE SPACE IF THERE IS ROOM ON THE CURRENT LINE.
            const unsigned int CHARACTER_COUNT_FOR_SPACE = 1;
            unsigned int current_line_character_count_with_space = current_line_of_text->size() + CHARACTER_COUNT_FOR_SPACE;
            bool current_line_can_hold_space = (current_line_character_count_with_space <= MAX_CHARACTER_COUNT_PER_LINE);
            if (current_line_can_hold_space)
            {
                const char SPACE = ' ';
                current_line_of_text->push_back(SPACE);
            }
            else
            {
                // If the current line can't hold a space, it can't hold the new word.
                // That means that that the code below will move to the next available line
                // to add the new word, so we shouldn't return early here.
            }
        }

        // CHECK IF THE CURRENT UNFILLED LINE OF TEXT CAN HOLD THE NEW WORD.
        unsigned int current_line_character_count_with_new_word = current_line_of_text->size() + word.size();
        bool current_line_can_hold_new_word = (current_line_character_count_with_new_word <= MAX_CHARACTER_COUNT_PER_LINE);
        if (!current_line_can_hold_new_word)
        {
            // MOVE TO THE NEXT LINE.
            ++LastUnfilledLineIndex;
            bool next_line_available = (LastUnfilledLineIndex < LINE_COUNT);
            if (!next_line_available)
            {
                return false;
            }

            // USE THE NEXT LINE TO HOLD THE NEW WORD.
            current_line_of_text = &LinesOfText[LastUnfilledLineIndex];
        }

        // ADD THE WORD TO THE LINE.
        current_line_of_text->insert(
            current_line_of_text->end(),
            word.cbegin(),
            word.cend());

        // The word was successfully added.
        return true;
    }

    bool TextPage::AllTextDisplayed() const
    {
        // CHECK IF THE ALL CHARACTERS HAVE BEEN DISPLAYED.
        unsigned int displayed_character_count = GetDisplayedCharacterCount();
        unsigned int total_character_count = GetTotalCharacterCount();
        bool all_text_displayed = (displayed_character_count >= total_character_count);
        return all_text_displayed;
    }

    void TextPage::Update(const float elapsed_time_in_seconds)
    {
        // CHECK IF THE PAGE OF TEXT HAS BEEN FULLY DISPLAYED.
        bool all_text_displayed = AllTextDisplayed();
        if (all_text_displayed)
        {
            // All text has been displayed, so there is nothing to do.
            // It is expected that the game is waiting on user input
            // to progress to another page of text.
            return;
        }

        // UPDATE THE TOTAL TIME TEXT HAS BEEN DISPLAYED IN THIS PAGE.
        TotalElapsedTimeInSecondsTextHasBeenDisplayed += elapsed_time_in_seconds;
    }

    void TextPage::Render(
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
        sf::RenderTarget& render_target)
    {
        // MAKE SURE THE FONT EXISTS.
        assert(font);

        // RENDER EACH LINE OF TEXT.
        MATH::Vector2ui current_line_top_left_screen_position_in_pixels = top_left_screen_position_in_pixels;
        for (unsigned int line_index = 0; line_index < LINE_COUNT; ++line_index)
        {
            // GET THE CURRENT LINE OF TEXT.
            std::string current_line_characters = GetTextLine(line_index);
            bool line_exists = !current_line_characters.empty();
            if (!line_exists)
            {
                // If no line exists, then there is nothing to render.
                return;
            }

            // RENDER THE CURRENT LINE.
            /// @todo   Make a version of text that takes in a font parameters for rendering?
            Text current_line(font, current_line_characters, current_line_top_left_screen_position_in_pixels);
            current_line.Render(render_target);

            // CALCULATE THE POSITION FOR THE NEXT LINE OF TEXT.
            current_line_top_left_screen_position_in_pixels.Y += Glyph::HEIGHT_IN_PIXELS;
        }
    }
    void TextPage::Render(std::ostream& output_stream)
    {
        // RENDER EACH LINE OF TEXT.
        for (unsigned int line_index = 0; line_index < LINE_COUNT; ++line_index)
        {
            // GET THE CURRENT LINE OF TEXT.
            std::string current_line_characters = GetTextLine(line_index);
            bool line_exists = !current_line_characters.empty();
            if (!line_exists)
            {
                // If no line exists, then there is nothing to render.
                return;
            }

            // RENDER THE CURRENT LINE.
            output_stream << current_line_characters << std::endl;
        }
    }

    unsigned int TextPage::GetTotalCharacterCount() const
    {
        unsigned int total_character_count = 0;

        // COUNT THE CHARACTERS ON EACH LINE.
        for (const auto& line : LinesOfText)
        {
            total_character_count += line.size();
        }

        return total_character_count;
    }

    unsigned int TextPage::GetCharacterCountForPreviousLines(const unsigned int line_index) const
    {
        unsigned int character_count_for_previous_lines = 0;

        // COUNT CHARACTERS FROM ALL PREVIOUS LINES.
        for (unsigned int previous_line_index = 0; previous_line_index < line_index; ++previous_line_index)
        {
            character_count_for_previous_lines += LinesOfText[previous_line_index].size();
        }

        return character_count_for_previous_lines;
    }

    unsigned int TextPage::GetDisplayedCharacterCount() const
    {
        unsigned int displayed_character_count = 0;

        // COUNT THE CHARACTERS ON EACH LINE.
        for (unsigned int line_index = 0; line_index < LINE_COUNT; ++line_index)
        {
            displayed_character_count += GetDisplayedCharacterCount(line_index);
        }

        return displayed_character_count;
    }

    unsigned int TextPage::GetDisplayedCharacterCount(const unsigned int line_index) const
    {
        /// @todo   This method is way to messy and complicated.
        /// Think about ways to simplify it, possibly by storing
        /// information per-line?

        // CALCULATE THE TOTAL NUMBER OF DISPLAYED CHARACTERS IN THE PAGE BASED ON THE ELAPSED TIME.
        // It's okay for any fractional components to be truncated.
        // That simply means the "next" character isn't fully ready
        // to be displayed yet.
        unsigned int displayed_character_count_for_page = static_cast<unsigned int>(
            TotalElapsedTimeInSecondsTextHasBeenDisplayed / ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS);

        // MAKE SURE THE CHARACTER COUNT IS WITHIN THE VALID RANGE.
        unsigned int total_character_count = GetTotalCharacterCount();
        displayed_character_count_for_page = std::min(displayed_character_count_for_page, total_character_count);

        // CHECK IF THE TEXT FOR ALL PREVIOUS LINES HAS BEEN DISPLAYED YET.
        unsigned int character_count_for_previous_lines = GetCharacterCountForPreviousLines(line_index);
        bool previous_lines_finished_being_displayed = (displayed_character_count_for_page > character_count_for_previous_lines);
        if (!previous_lines_finished_being_displayed)
        {
            // If text from all previous lines has not been displayed, the
            // current line should have not any text displayed.
            return 0;
        }

        // CALCULATE THE DISPLAYED CHARACTER COUNT FOR THE CURRENT LINE.
        unsigned int displayed_character_count_for_current_line = (displayed_character_count_for_page - character_count_for_previous_lines);
        
        // MAKE SURE THE DISPLAYED CHARACTER COUNT IS IN THE VALID RANGE FOR THE LINE.
        const auto& current_line = LinesOfText[line_index];
        displayed_character_count_for_current_line = std::min(displayed_character_count_for_current_line, current_line.size());
        
        return displayed_character_count_for_current_line;
    }

    std::string TextPage::GetTextLine(const unsigned int line_index) const
    {
        // MAKE SURE THE TEXT LINE IS VALID.
        bool line_index_in_range = (line_index < LinesOfText.size());
        if (!line_index_in_range)
        {
            // No text line exists at the specified index.
            return "";
        }

        // GET THE CURRENT LINE OF TEXT.
        const std::vector<char>& current_line_characters = LinesOfText[line_index];
        unsigned int displayed_character_count = GetDisplayedCharacterCount(line_index);

        auto first_displayed_character = current_line_characters.cbegin();
        auto end_of_displayed_characters = first_displayed_character + displayed_character_count;

        std::string current_line(first_displayed_character, end_of_displayed_characters);

        return current_line;
    }
}
}
