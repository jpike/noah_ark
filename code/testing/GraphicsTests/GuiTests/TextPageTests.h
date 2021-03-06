#pragma once

#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>
#include "String/String.h"
#include "Graphics/Gui/Text.h"
#include "Graphics/Gui/TextPage.h"

/// A namespace for testing the TextPage class.
namespace TEST_TEXT_PAGE
{
    using namespace GRAPHICS::GUI;

    TEST_CASE("An empty string can be rendered.", "[TextPage]")
    {
        // ADD AN EMPTY STRING TO A TEXT PAGE.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        std::shared_ptr<Font> font = Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        TextPage text_page(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS, font);

        const std::string EMPTY_STRING = "";
        bool text_added = text_page.Add(EMPTY_STRING);
        REQUIRE(text_added);

        // ELAPSE ENOUGH TIME FOR A SINGLE CHARACTER TO BE DISPLAYED.
        text_page.Update(sf::seconds(TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS));

        // RENDER THE TEXT PAGE.
        std::stringstream output_stream;
        text_page.Render(output_stream);
        
        // VERIFY THAT THE RENDERED TEXT IS EMPTY.
        std::string rendered_text = output_stream.str();
        REQUIRE(EMPTY_STRING == rendered_text);
    }

    TEST_CASE("No characters are rendered if no time has elapsed.", "[TextPage]")
    {
        // ADD A SINGLE CHARACTER TO A TEXT PAGE.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        std::shared_ptr<Font> font = Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        TextPage text_page(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS, font);

        const std::string SINGLE_CHARACTER = "a";
        bool text_added = text_page.Add(SINGLE_CHARACTER);
        REQUIRE(text_added);

        // RENDER THE TEXT PAGE WITHOUT ANY TIME ELAPSING.
        std::stringstream output_stream;
        text_page.Render(output_stream);

        // VERIFY THAT THE NO CHARACTERS WERE RENDERED.
        const std::string EXPECTED_TEXT = "";
        std::string rendered_text = output_stream.str();
        REQUIRE(EXPECTED_TEXT == rendered_text);
    }

    TEST_CASE("A single character can be rendered.", "[TextPage]")
    {
        // ADD A SINGLE CHARACTER TO A TEXT PAGE.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        std::shared_ptr<Font> font = Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        TextPage text_page(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS, font);

        const std::string SINGLE_CHARACTER = "a";
        bool text_added = text_page.Add(SINGLE_CHARACTER);
        REQUIRE(text_added);

        // ELAPSE ENOUGH TIME FOR THE SINGLE CHARACTER TO BE DISPLAYED.
        text_page.Update(sf::seconds(TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS));

        // RENDER THE TEXT PAGE.
        std::stringstream output_stream;
        text_page.Render(output_stream);

        // VERIFY THAT THE SINGLE CHARACTER WAS RENDERED.
        // Since the text page renders text by lines, each line should end with a newline.
        const std::string EXPECTED_TEXT = SINGLE_CHARACTER + "\n";
        std::string rendered_text = output_stream.str();
        REQUIRE(EXPECTED_TEXT == rendered_text);
    }

    TEST_CASE("A single word can be rendered on a single line.", "[TextPage]")
    {
        // ADD A SINGLE WORD TO A TEXT PAGE.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        std::shared_ptr<Font> font = Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        TextPage text_page(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS, font);

        const std::string SINGLE_WORD = "word";
        bool text_added = text_page.Add(SINGLE_WORD);
        REQUIRE(text_added);

        // VERIFY EACH LETTER CAN BE RENDERED INDIVIDUALLY BASED ON TIME.
        for (unsigned int character_index = 0; character_index < SINGLE_WORD.length(); ++character_index)
        {
            // ELAPSE ENOUGH TIME FOR THIS NEXT CHARACTER TO BE DISPLAYED.
            text_page.Update(sf::seconds(TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS));

            // RENDER THE TEXT PAGE.
            std::stringstream output_stream;
            text_page.Render(output_stream);

            // VERIFY THAT THE EXPECTED TEXT WAS RENDERED.
            // Since the text page renders text by lines, each line should end with a newline.
            const unsigned int START_OF_STRING = 0;
            unsigned int current_character_count = character_index + 1;
            std::string expected_text = SINGLE_WORD.substr(START_OF_STRING, current_character_count) + "\n";
            std::string rendered_text = output_stream.str();
            REQUIRE(expected_text == rendered_text);
        }
    }

    TEST_CASE("Multiple words can be rendered on a single line.", "[TextPage]")
    {
        // ADD MULTIPLE WORDS TO A TEXT PAGE.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        std::shared_ptr<Font> font = Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        TextPage text_page(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS, font);

        const std::vector<std::string> WORDS =
        {
            "Hello",
            "world"
        };
        for (const std::string& word : WORDS)
        {
            bool word_added = text_page.Add(word);
            REQUIRE(word_added);
        }

        // VERIFY EACH WORD CAN BE RENDERED PROPERLY.
        std::string previously_rendered_words;
        for (auto word = WORDS.cbegin(); word != WORDS.cend(); ++word)
        {
            // VERIFY EACH LETTER CAN BE RENDERED INDIVIDUALLY BASED ON TIME.
            for (unsigned int character_index = 0; character_index < word->length(); ++character_index)
            {
                // ELAPSE ENOUGH TIME FOR THIS NEXT CHARACTER TO BE DISPLAYED.
                text_page.Update(sf::seconds(TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS));

                // RENDER THE TEXT PAGE.
                std::stringstream output_stream;
                text_page.Render(output_stream);

                // VERIFY THAT THE EXPECTED TEXT WAS RENDERED.
                const unsigned int START_OF_STRING = 0;
                unsigned int current_character_count = character_index + 1;
                std::string current_word_rendered_characters_so_far = word->substr(START_OF_STRING, current_character_count);
                // Since the text page renders text by lines, each line should end with a newline.
                std::string expected_text = (previously_rendered_words + current_word_rendered_characters_so_far + "\n");
                std::string rendered_text = output_stream.str();
                REQUIRE(expected_text == rendered_text);
            }

            // KEEP TRACK OF PREVIOUSLY RENDERED WORDS.
            // This is a simple way to keep including previously rendered words in the expected text verified above.
            previously_rendered_words += (*word);

            // ADD A SPACE IF THERE ARE MORE WORDS TO BE RENDERD.
            // Since words are separated by a single space, a space also needs to be added
            // if more words are going to be rendered.
            bool more_words_to_render = (WORDS.cend() != std::next(word));
            if (more_words_to_render)
            {
                // ADD THE SPACE.
                previously_rendered_words += " ";

                // ELAPSE ENOUGH TIME FOR THE SPACE CHARACTER TO HAVE BEEN DISPLAYED.
                text_page.Update(sf::seconds(TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS));

                // VERIFY THAT THE SPACE CHARACTER WAS RENDERED.
                std::stringstream output_stream_with_space;
                text_page.Render(output_stream_with_space);

                // VERIFY THAT THE EXPECTED TEXT WAS RENDERED.
                // Since the text page renders text by lines, each line should end with a newline.
                std::string expected_text_with_space = (previously_rendered_words + "\n");
                std::string rendered_text_with_space = output_stream_with_space.str();
                REQUIRE(expected_text_with_space == rendered_text_with_space);
            }   
        }
    }

    TEST_CASE("A full line of text can be rendered on a single line.", "[TextPage]")
    {
        // ADD A FULL LINE OF TEXT TO A TEXT PAGE.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        std::shared_ptr<Font> font = Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        TextPage text_page(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS, font);

        unsigned int max_character_count_per_line = static_cast<unsigned int>(
            static_cast<float>(text_page.MaxAvailableWidthForTextInPixels) / static_cast<float>(Glyph::DEFAULT_WIDTH_IN_PIXELS));
        std::string full_line_of_text(max_character_count_per_line, 'x');
        bool text_added = text_page.Add(full_line_of_text);
        REQUIRE(text_added);

        // VERIFY EACH LETTER CAN BE RENDERED INDIVIDUALLY BASED ON TIME.
        for (unsigned int character_index = 0; character_index < full_line_of_text.length(); ++character_index)
        {
            // ELAPSE ENOUGH TIME FOR THIS NEXT CHARACTER TO BE DISPLAYED.
            // Due to numerical precision issues, a small amount of buffer padding is added
            // by multiplying the elapsed time to be somewhere between the time for 1 and 2
            // characters.  Note that this time isn't entirely stable, so this "fix" hasn't
            // been propagated to every test case.  Each test case will only be modified as
            // needed.
            float elapsed_time_in_seconds_for_next_character = 1.0001f * TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS;
            text_page.Update(sf::seconds(elapsed_time_in_seconds_for_next_character));

            // RENDER THE TEXT PAGE.
            std::stringstream output_stream;
            text_page.Render(output_stream);

            // VERIFY THAT THE EXPECTED TEXT WAS RENDERED.
            // Since the text page renders text by lines, each line should end with a newline.
            const unsigned int START_OF_STRING = 0;
            unsigned int current_character_count = character_index + 1;
            std::string expected_text = full_line_of_text.substr(START_OF_STRING, current_character_count) + "\n";
            std::string rendered_text = output_stream.str();
            REQUIRE(expected_text == rendered_text);
        }
    }

    TEST_CASE("2 full lines of text can be rendered on a separate lines.", "[TextPage]")
    {
        // ADD 2 FULL LINES OF TEXT TO A TEXT PAGE.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        std::shared_ptr<Font> font = Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        TextPage text_page(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS, font);

        unsigned int max_character_count_per_line = static_cast<unsigned int>(
            static_cast<float>(text_page.MaxAvailableWidthForTextInPixels) / static_cast<float>(Glyph::DEFAULT_WIDTH_IN_PIXELS));
        std::vector<std::string> lines =
        {
            std::string(max_character_count_per_line, 'x'),
            std::string(max_character_count_per_line, 'y'),
        };
        for (const std::string& line : lines)
        {
            bool line_added = text_page.Add(line);
            REQUIRE(line_added);
        }

        // VERIFY EACH LINE CAN BE RENDERED PROPERLY.
        std::string previously_rendered_lines;
        for (auto line = lines.cbegin(); line != lines.cend(); ++line)
        {
            // VERIFY EACH LETTER CAN BE RENDERED INDIVIDUALLY BASED ON TIME.
            for (unsigned int character_index = 0; character_index < line->length(); ++character_index)
            {
                // ELAPSE ENOUGH TIME FOR THIS NEXT CHARACTER TO BE DISPLAYED.
                // Due to numerical precision issues, a small amount of buffer padding is added
                // by multiplying the elapsed time to be somewhere between the time for 1 and 2
                // characters.  Note that this time isn't entirely stable, so this "fix" hasn't
                // been propagated to every test case.  Each test case will only be modified as
                // needed.
                float elapsed_time_in_seconds_for_next_character = 1.0001f * TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS;
                text_page.Update(sf::seconds(elapsed_time_in_seconds_for_next_character));

                // RENDER THE TEXT PAGE.
                std::stringstream output_stream;
                text_page.Render(output_stream);

                // VERIFY THAT THE EXPECTED TEXT WAS RENDERED.
                const unsigned int START_OF_STRING = 0;
                unsigned int current_character_count = character_index + 1;
                std::string current_word_rendered_characters_so_far = line->substr(START_OF_STRING, current_character_count);
                // Since the text page renders text by lines, each line should end with a newline.
                std::string expected_text = (previously_rendered_lines + current_word_rendered_characters_so_far + "\n");
                std::string rendered_text = output_stream.str();
                REQUIRE(expected_text == rendered_text);
            }

            // ADD THE LINE OF TEXT THAT JUST FINISHED BEING RENDERED.
            // A newline is also needed before the next line can be rendered.
            previously_rendered_lines += (*line) + "\n";
        }
    }

    TEST_CASE("A multiline Bible verse can be properly rendered on multiple lines.", "[TextPage]")
    {
        // DEFINE THE TIME NEEDED TO DISPLAY A SINGLE CHARACTER.
        // Due to numerical precision issues, a small amount of buffer padding is added
        // by multiplying the elapsed time to be somewhere between the time for 1 and 2
        // characters.  Note that this time isn't entirely stable, so this "fix" hasn't
        // been propagated to every test case.  Each test case will only be modified as
        // needed.
        float elapsed_time_in_seconds_for_next_character = 1.0001f * TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS;

        // ADD WORDS FROM A BIBLE VERSE TO A TEXT PAGE.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        std::shared_ptr<Font> font = Font::LoadSystemDefaultFont(SYSTEM_FIXED_FONT);
        TextPage text_page(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS, font);

        // The Bible verse was chosen to fit within a single text page.
        unsigned int max_character_count_per_line = static_cast<unsigned int>(
            static_cast<float>(text_page.MaxAvailableWidthForTextInPixels) / static_cast<float>(Glyph::DEFAULT_WIDTH_IN_PIXELS));
        std::deque<std::string> words = STRING::String::SplitIntoWords(
            "But Noah found grace in the eyes of the Lord.",
            max_character_count_per_line);
        for (const std::string& word : words)
        {
            bool word_added = text_page.Add(word);
            REQUIRE(word_added);
        }

        // VERIFY EACH WORD CAN BE RENDERED PROPERLY.
        std::string previously_rendered_words;
        std::string previously_rendered_words_on_current_line;
        for (auto word = words.cbegin(); word != words.cend(); ++word)
        {
            // VERIFY EACH LETTER CAN BE RENDERED INDIVIDUALLY BASED ON TIME.
            for (unsigned int character_index = 0; character_index < word->length(); ++character_index)
            {
                // ELAPSE ENOUGH TIME FOR THIS NEXT CHARACTER TO BE DISPLAYED.
                text_page.Update(sf::seconds(elapsed_time_in_seconds_for_next_character));

                // RENDER THE TEXT PAGE.
                std::stringstream output_stream;
                text_page.Render(output_stream);

                // VERIFY THAT THE EXPECTED TEXT WAS RENDERED.
                const unsigned int START_OF_STRING = 0;
                unsigned int current_character_count = character_index + 1;
                std::string current_word_rendered_characters_so_far = word->substr(START_OF_STRING, current_character_count);
                // Since the text page renders text by lines, each line should end with a newline.
                std::string expected_text = (previously_rendered_words + current_word_rendered_characters_so_far + "\n");
                std::string rendered_text = output_stream.str();
                REQUIRE(expected_text == rendered_text);
            }

            // KEEP TRACK OF PREVIOUSLY RENDERED WORDS.
            // This is a simple way to keep including previously rendered words in the expected text verified above.
            previously_rendered_words += (*word);
            previously_rendered_words_on_current_line += (*word);

            // CHECK IF THERE ARE MORE WORDS TO RENDER.
            auto next_word = std::next(word);
            bool more_words_to_render = (words.cend() != next_word);
            if (more_words_to_render)
            {
                // CHECK IF THE CURRENT LINE HAS ROOM FOR A SPACE.
                // It will be added for the next word if there is room.
                std::string previously_rendered_words_on_current_line_plus_space = previously_rendered_words_on_current_line + " ";
                Text previous_text = { .String = previously_rendered_words_on_current_line_plus_space };
                std::size_t previously_rendered_words_on_current_line_plus_space_width_in_pixels = previous_text.Width<std::size_t>(*font);
                std::size_t max_line_width_in_pixels = max_character_count_per_line * GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS;
                bool space_fits_on_current_line = (previously_rendered_words_on_current_line_plus_space_width_in_pixels <= max_line_width_in_pixels);
                if (space_fits_on_current_line)
                {
                    // ADD THE SPACE BEFORE THE NEXT WORD.
                    previously_rendered_words += " ";
                    previously_rendered_words_on_current_line += " ";
                }

                // CHECK IF THE NEXT WORD FITS ON CURRENT LINE.
                std::string current_line_with_next_word = previously_rendered_words_on_current_line + *next_word;
                Text text_with_next_word = { .String = current_line_with_next_word };
                std::size_t current_line_with_next_word_width_in_pixels = text_with_next_word.Width<std::size_t>(*font);
                bool next_word_fits_on_current_line = (current_line_with_next_word_width_in_pixels <= max_line_width_in_pixels);
                if (next_word_fits_on_current_line)
                {
                    // VERIFY RENDERING OF THE SPACE IF ONE WAS ADDED.
                    if (space_fits_on_current_line)
                    {
                        // ELAPSE ENOUGH TIME FOR THE SPACE CHARACTER TO HAVE BEEN DISPLAYED.
                        text_page.Update(sf::seconds(elapsed_time_in_seconds_for_next_character));

                        // VERIFY THAT THE SPACE CHARACTER WAS RENDERED.
                        std::stringstream output_stream_with_space;
                        text_page.Render(output_stream_with_space);

                        // VERIFY THAT THE EXPECTED TEXT WAS RENDERED.
                        // Since the text page renders text by lines, each line should end with a newline.
                        std::string expected_text_with_space = (previously_rendered_words + "\n");
                        std::string rendered_text_with_space = output_stream_with_space.str();
                        REQUIRE(expected_text_with_space == rendered_text_with_space);
                    }
                }
                else
                {
                    // REMOVE THE SPACE IF THE WORD DOESN'T FIT.
                    // If the next word doesn't fit on the current line, then there's no point in rendering a space.
                    if (space_fits_on_current_line)
                    {
                        previously_rendered_words.pop_back();
                        previously_rendered_words_on_current_line.pop_back();
                    }

                    // ADD THE LINE OF TEXT THAT JUST FINISHED BEING RENDERED.
                    // A newline is also needed before the next line can be rendered.
                    previously_rendered_words += "\n";
                    
                    // CLEAR THE PREVIOUSLY RENDERED WORDS FOR THE CURRENT LINE.
                    // Since we're about to start a new line, this should be empty.
                    previously_rendered_words_on_current_line.clear();
                }
            }
        }
    }
}
