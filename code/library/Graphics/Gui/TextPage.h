#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Graphics/Renderer.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
namespace GUI
{
    /// A single page of text displayed in a text box.
    /// A page encompasses text that may be displayed in
    /// the text box at a given time.  If more text is
    /// supplied to the text box than can fit on a single
    /// page, then the text will be split up across
    /// multiple pages.
    class TextPage
    {
    public:
        // CONSTANTS.
        /// The number of characters of padding per line of text in the page.
        /// In order to prevent the characters on each line from running up against the border
        /// of the text box, one character on each side of each line is reserved to provide
        /// padding.
        static const unsigned int ONE_CHARACTER_OF_PADDING_ON_EACH_SIDE_OF_LINE = 2;
        /// The time that must elapse for a single character to be displayed in the page.
        /// The displayed character count is time-based to ensure a
        /// a consistent speed in how fast text is displayed along
        /// with making sure it doesn't get displayed too quickly.
        static const float ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS;
        /// The scale ratio for text in the text box.
        static const float TEXT_SCALE_RATIO;

        // CONSTRUCTION.
        explicit TextPage(const unsigned int width_in_pixels, const unsigned int height_in_pixels);

        // OTHER PUBLIC METHODS.
        bool Add(const std::string& word);

        bool AllTextDisplayed() const;

        void Update(const sf::Time& elapsed_time);

        void Render(
            const MATH::Vector2ui& top_left_screen_position_in_pixels,
            GRAPHICS::Renderer& renderer) const;
        void Render(std::ostream& output_stream) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The maximum number of lines of text that can be displayed in the page at once.
        const unsigned int LineCount;
        /// The maximum number of characters that can be displayd per line.
        const unsigned int MaxCharacterCountPerLine;

    private:
        // PRIVATE METHODS.
        unsigned int GetTotalCharacterCount() const;
        unsigned int GetCharacterCountForPreviousLines(const unsigned int line_index) const;
        unsigned int GetDisplayedCharacterCount() const;
        unsigned int GetDisplayedCharacterCount(const unsigned int line_index) const;
        std::string GetTextLine(const unsigned int line_index) const;

        // MEMBER VARIABLES.
        /// The sequence of characters displayed on each line in the page.
        std::vector< std::vector<char> > LinesOfText;
        /// The index of the last line of text that hasn't been completely filled.
        unsigned int LastUnfilledLineIndex;
        /// The total elapsed time (in seconds) that text in this page has been displayed.
        float TotalElapsedTimeInSecondsTextHasBeenDisplayed;
    };
}
}
