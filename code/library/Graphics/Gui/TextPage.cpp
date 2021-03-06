#include <algorithm>
#include "Graphics/Gui/Text.h"
#include "Graphics/Gui/TextPage.h"

namespace GRAPHICS::GUI
{
    /// Constructor.
    /// @param[in]  width_in_pixels - The width of the text page, in pixels.
    /// @param[in]  height_in_pixels - The height of the text page, in pixels.
    /// @param[in]  font - The font to use for rendering text in the page.
    TextPage::TextPage(const unsigned int width_in_pixels, const unsigned int height_in_pixels, const std::shared_ptr<GRAPHICS::GUI::Font>& font) :
        MaxAvailableWidthForTextInPixels(width_in_pixels - (Glyph::DEFAULT_WIDTH_IN_PIXELS * ONE_CHARACTER_OF_PADDING_ON_EACH_SIDE_OF_LINE)),
        LineCount(height_in_pixels / Glyph::DEFAULT_HEIGHT_IN_PIXELS),
        Font(font),
        LinesOfText(LineCount),
        LastUnfilledLineIndex(0),
        TotalElapsedTimeInSecondsTextHasBeenDisplayed(0.0f)
    {}

    /// Adds the provided word to the text page.
    /// If this is not the first word being added to the page, 
    /// a space will be added before the new word.
    /// @param[in]  word - The word to add.
    /// @return True if the word is added; false otherwise.
    bool TextPage::Add(const std::string& word)
    {
        // MAKE SURE THERE IS ROOM FOR THE NEW WORD.
        bool all_text_lines_full = (LastUnfilledLineIndex >= LineCount);
        if (all_text_lines_full)
        {
            return false;
        }

        // DETERMINE IF A SPACE NEEDS TO BE ADDED BEFORE THE NEW WORD.
        // Spaces should separate consecutive words, but putting a space before
        // the first word on a line would waste a character slot.
        const char SPACE = ' ';
        std::vector<char>& first_unfilled_line_of_text = LinesOfText[LastUnfilledLineIndex];
        std::vector<char>* current_line_of_text = &first_unfilled_line_of_text;
        std::string current_line_as_string = std::string(current_line_of_text->cbegin(), current_line_of_text->cend());
        bool first_word = current_line_of_text->empty();
        if (!first_word)
        {
            current_line_as_string += SPACE;
        }

        // CHECK IF THE CURRENT UNFILLED LINE OF TEXT CAN HOLD THE NEW WORD.
        std::string current_line_with_new_word = current_line_as_string + word;
        Text current_line_text_with_new_word = { .String = current_line_with_new_word };
        std::size_t current_line_with_new_word_width_in_pixels = current_line_text_with_new_word.Width<std::size_t>(*Font);
        bool current_line_can_hold_new_word = (current_line_with_new_word_width_in_pixels <= MaxAvailableWidthForTextInPixels);
        if (current_line_can_hold_new_word)
        {
            // ADD THE SPACE BEFORE THE NEW WORD.
            // The above only added the space to a temporary string, but it needs to be
            // added to the official line if this isn't the first word on the line.
            if (!first_word)
            {
                current_line_of_text->push_back(SPACE);
            }
        }
        else
        {
            // MOVE TO THE NEXT LINE.
            ++LastUnfilledLineIndex;
            bool next_line_available = (LastUnfilledLineIndex < LineCount);
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

    /// Checks if all text in this page has been displayed.
    /// @return True if all text in the page has been displayed; false otherwise.
    bool TextPage::AllTextDisplayed() const
    {
        // CHECK IF THE ALL CHARACTERS HAVE BEEN DISPLAYED.
        unsigned int displayed_character_count = GetDisplayedCharacterCount();
        unsigned int total_character_count = GetTotalCharacterCount();
        bool all_text_displayed = (displayed_character_count >= total_character_count);
        return all_text_displayed;
    }

    /// Updates the text displayed in the text page based on the provided
    /// elapsed amount of time.
    /// @param[in]  elapsed_time - The amount of time elapsed
    ///     since the last update of the text page.
    void TextPage::Update(const sf::Time& elapsed_time)
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
        float elapsed_time_in_seconds = elapsed_time.asSeconds();
        TotalElapsedTimeInSecondsTextHasBeenDisplayed += elapsed_time_in_seconds;
    }

    /// Renders the text page to the provided screen.
    /// @param[in]  top_left_screen_position_in_pixels - The top-left screen position of
    ///     the text page (in pixels).
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void TextPage::Render(
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        GRAPHICS::Renderer& renderer) const
    {
        // RENDER EACH LINE OF TEXT.
        MATH::Vector2f current_line_top_left_screen_position_in_pixels(
            static_cast<float>(top_left_screen_position_in_pixels.X), 
            static_cast<float>(top_left_screen_position_in_pixels.Y));
        for (unsigned int line_index = 0; line_index < LineCount; ++line_index)
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
            renderer.RenderText(
                current_line_characters, 
                RESOURCES::AssetId::FONT_TEXTURE, 
                current_line_top_left_screen_position_in_pixels, 
                GRAPHICS::Color::BLACK);

            // CALCULATE THE POSITION FOR THE NEXT LINE OF TEXT.
            current_line_top_left_screen_position_in_pixels.Y += Glyph::DEFAULT_HEIGHT_IN_PIXELS;
        }
    }

    /// Renders the text page to the provided stream.
    /// @param[in,out]  output_stream - The output stream to render to.
    void TextPage::Render(std::ostream& output_stream) const
    {
        // RENDER EACH LINE OF TEXT.
        for (unsigned int line_index = 0; line_index < LineCount; ++line_index)
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

    /// Gets the total number of characters in the page, including both those that
    /// have and haven't been displayed.
    /// @return The total number of characters in the page.
    unsigned int TextPage::GetTotalCharacterCount() const
    {
        size_t total_character_count = 0;

        // COUNT THE CHARACTERS ON EACH LINE.
        for (const auto& line : LinesOfText)
        {
            total_character_count += line.size();
        }

        return static_cast<unsigned int>(total_character_count);
    }

    /// Gets the number of characters that have been displayed so far on previous lines.
    /// @param[in]  line_index - Lines with an index less than this value will have their
    ///     character counts included in the returned sum.
    /// @return The number of characters displayed so far on previous lines.
    unsigned int TextPage::GetCharacterCountForPreviousLines(const unsigned int line_index) const
    {
        size_t character_count_for_previous_lines = 0;

        // COUNT CHARACTERS FROM ALL PREVIOUS LINES.
        for (unsigned int previous_line_index = 0; previous_line_index < line_index; ++previous_line_index)
        {
            character_count_for_previous_lines += LinesOfText[previous_line_index].size();
        }

        return static_cast<unsigned int>(character_count_for_previous_lines);
    }

    /// Gets the number of characters that have been displayed in the text page so far.
    /// @return The number of characters displayed so far in the text page.
    unsigned int TextPage::GetDisplayedCharacterCount() const
    {
        unsigned int displayed_character_count = 0;

        // COUNT THE CHARACTERS ON EACH LINE.
        for (unsigned int line_index = 0; line_index < LineCount; ++line_index)
        {
            displayed_character_count += GetDisplayedCharacterCount(line_index);
        }

        return displayed_character_count;
    }

    /// Gets the number of characters that have been displayed on the specified line so far.
    /// @param[in]  line_index - The index of the line whose displayed character count to retrieve.
    /// @return The number of characters displayed so far on the specified line.
    unsigned int TextPage::GetDisplayedCharacterCount(const unsigned int line_index) const
    {
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
        size_t displayed_character_count_for_current_line = (displayed_character_count_for_page - character_count_for_previous_lines);
        
        // MAKE SURE THE DISPLAYED CHARACTER COUNT IS IN THE VALID RANGE FOR THE LINE.
        const auto& current_line = LinesOfText[line_index];
        displayed_character_count_for_current_line = std::min(displayed_character_count_for_current_line, current_line.size());
        
        return static_cast<unsigned int>(displayed_character_count_for_current_line);
    }

    /// Gets the line of text for the specified line index.
    /// @param[in]  line_index - The index of the line for which to get the text.
    /// @return The line of text at the specified line; empty if no text.
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
