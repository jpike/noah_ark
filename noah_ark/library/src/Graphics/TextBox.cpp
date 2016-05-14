#include <algorithm>
#include <stdexcept>
#include "Graphics/Text.h"
#include "Graphics/TextBox.h"

namespace GRAPHICS
{
    TextBox::TextBox(
        const std::shared_ptr<const GRAPHICS::Font>& font,
        const unsigned int width_in_pixels) :
    Font(font),
    IsVisible(false),
    WidthInPixels(width_in_pixels),
    /// @todo   Think about better way to calculate this.
    MaxCharacterCountPerLine(width_in_pixels / Glyph::WIDTH_IN_PIXELS - 2),
    TimeSinceLastCharacterDisplayedInSeconds(0.0f)
    {
        // MAKE SURE A FONT EXISTS.
        bool font_exists = (nullptr != Font);
        if (!font_exists)
        {
            throw std::invalid_argument("Font provided for text box must not be null.");
        }
    }

    void TextBox::StartDisplayingText(const std::string& text)
    {
        FullTextToDisplay = text;
        FirstDisplayedCharacterIndex = 0;
        LastDisplayedCharacterIndex = 0;
        TimeSinceLastCharacterDisplayedInSeconds = 0.0f;
        IsVisible = true;
    }

    void TextBox::Update(const float elapsed_time_in_seconds)
    {
        // CHECK IF THE TEXT BOX'S CURRENT LINES ARE FULL.
        bool text_box_currently_full = CurrentLinesOfTextFinishedBeingDisplayed();
        if (text_box_currently_full)
        {
            // The text box is currently awaiting user input to progress
            // to the next lines of text, so don't do anything to ensure
            // that the user can properly read the message.
            return;
        }

        // UPDATE THE TIME SINCE THE LAST CHARACTER WAS DISPLAYED.
        TimeSinceLastCharacterDisplayedInSeconds += elapsed_time_in_seconds;

        // CHECK IF ENOUGH TIME HAS PASSED FOR DISPLAYING THE NEXT CHARACTER.
        // This ensures a consistent speed in how fast text is displayed
        // along with making sure it doesn't get displayed too quickly.
        const float ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS = 0.1f;
        bool enough_time_elapsed_since_last_displayed_character = (TimeSinceLastCharacterDisplayedInSeconds >= ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS);
        if (!enough_time_elapsed_since_last_displayed_character)
        {
            // Don't do anything since enough time hasn't elapsed since the last character was displayed.
            return;
        }

        // INCLUDE THE NEXT CHARACTER IN THE DISPLAYED TEXT.
        /// @todo   Add a number of characters proportional to the elapsed time.
        ++LastDisplayedCharacterIndex;
        TimeSinceLastCharacterDisplayedInSeconds = 0.0f;

        // CLAMP THE LAST CHARACTER INDEX TO WITHIN THE VALID RANGE.
        bool all_text_displayed = (LastDisplayedCharacterIndex >= FullTextToDisplay.length());
        if (all_text_displayed)
        {
            LastDisplayedCharacterIndex = FullTextToDisplay.length() - 1;
        }
    }

    void TextBox::MoveToNextText()
    {
        // START DISPLAYING TEXT AT THE NEXT AVAILABLE CHARACTER.
        FirstDisplayedCharacterIndex = ++LastDisplayedCharacterIndex;

        // HIDE THE TEXT BOX IF THERE IS NO MORE TEXT.
        bool all_text_displayed = (FirstDisplayedCharacterIndex >= FullTextToDisplay.length());
        if (all_text_displayed)
        {
            IsVisible = false;
        }
    }

    bool TextBox::CurrentLinesOfTextFinishedBeingDisplayed() const
    {
        // CHECK IF ALL OF THE TEXT HAS BEEN DISPLAYED.
        unsigned int last_actual_character_index = FullTextToDisplay.length() - 1;
        bool all_text_displayed = (LastDisplayedCharacterIndex >= last_actual_character_index);
        if (all_text_displayed)
        {
            // If all of the text has been displayed, then all of the current lines
            // must have been displayed.
            return true;
        }

        // CHECK IF THE MAXIMUM NUMBER OF CHARACTERS FOR THE CURRENT SET OF LINES HAS BEEN DISPLAYED.
        unsigned int max_character_count_displayed_at_once = LINE_COUNT * MaxCharacterCountPerLine;
        unsigned int actual_displayed_character_count = (LastDisplayedCharacterIndex - FirstDisplayedCharacterIndex) + 1;
        bool current_lines_of_text_fully_displayed = (actual_displayed_character_count >= max_character_count_displayed_at_once);
        return current_lines_of_text_fully_displayed;
    }

    void TextBox::Render(sf::RenderTarget& render_target)
    {
        /// @todo   Rework this class to used an approach that uses fixed-size buffers
        /// for characters.  That seems like it would be simpler, particularly when
        /// trying to account for not breaking up words onto separate lines in the middle.

        // DRAW A BOX TO HOLD THE TEXT.
        sf::RectangleShape box;
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        const float OUTLINE_THICKNESS_IN_PIXELS = 4.0f;
        box.setOutlineThickness(OUTLINE_THICKNESS_IN_PIXELS);
        
        const float VERTICAL_PADDING_IN_PIXELS = 16.0f;
        float height_in_pixels = (LINE_COUNT * Glyph::HEIGHT_IN_PIXELS) + VERTICAL_PADDING_IN_PIXELS - OUTLINE_THICKNESS_IN_PIXELS * 2;
        float width_in_pixels = static_cast<float>(WidthInPixels) - OUTLINE_THICKNESS_IN_PIXELS * 2;
        box.setSize(sf::Vector2f(width_in_pixels, height_in_pixels));
        
        const sf::Vector2i SCREEN_TOP_LEFT_CORNER(0, 0);
        sf::Vector2f top_left_corner_world_position = render_target.mapPixelToCoords(SCREEN_TOP_LEFT_CORNER);
        top_left_corner_world_position.x += OUTLINE_THICKNESS_IN_PIXELS;
        top_left_corner_world_position.y += OUTLINE_THICKNESS_IN_PIXELS;
        box.setPosition(top_left_corner_world_position);

        render_target.draw(box);

        // DRAW THE FIRST LINE OF TEXT.
        /// @todo   Handle not breaking words up across multiple lines.
        unsigned int actual_character_count_to_display = (LastDisplayedCharacterIndex - FirstDisplayedCharacterIndex) + 1;
        unsigned int character_count_for_first_line = std::min(MaxCharacterCountPerLine, actual_character_count_to_display);
        std::string first_line_to_display = FullTextToDisplay.substr(FirstDisplayedCharacterIndex, character_count_for_first_line);

        const unsigned int FIRST_LINE_VERTICAL_PADDING_IN_PIXELS = static_cast<unsigned int>(VERTICAL_PADDING_IN_PIXELS / 2.0f);
        MATH::Vector2ui first_line_top_left_screen_position_in_pixels;
        first_line_top_left_screen_position_in_pixels.X = SCREEN_TOP_LEFT_CORNER.x + Glyph::WIDTH_IN_PIXELS;
        first_line_top_left_screen_position_in_pixels.Y = SCREEN_TOP_LEFT_CORNER.y + FIRST_LINE_VERTICAL_PADDING_IN_PIXELS;
        Text first_line(Font, first_line_to_display, first_line_top_left_screen_position_in_pixels);
        first_line.Render(render_target);

        // DRAW THE SECOND LINE OF TEXT, IF NEEDED.
        bool second_line_exists = (actual_character_count_to_display > character_count_for_first_line);
        if (second_line_exists)
        {
            unsigned int second_line_first_character_start_index = FirstDisplayedCharacterIndex + character_count_for_first_line;
            unsigned int character_count_for_second_line = (LastDisplayedCharacterIndex - second_line_first_character_start_index) + 1;
            std::string second_line_to_display = FullTextToDisplay.substr(second_line_first_character_start_index, character_count_for_second_line);

            MATH::Vector2ui second_line_top_left_screen_position_in_pixels = first_line_top_left_screen_position_in_pixels;
            second_line_top_left_screen_position_in_pixels.Y += Glyph::HEIGHT_IN_PIXELS;
            Text second_line(Font, second_line_to_display, second_line_top_left_screen_position_in_pixels);
            second_line.Render(render_target);
        }

        /// @todo   Draw upside down triangle to let user know to press button to continue.
    }
}
