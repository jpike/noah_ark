#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Graphics/Font.h"

namespace GRAPHICS
{
    /// A text box for displaying messages to a player.
    /// The text box can display up to 2 lines of text at a time.
    /// If more text needs to be displayed, then the player
    /// can press a button to progress to the next set of lines.
    /// Text is rendered 1 character at a time.
    class TextBox
    {
    public:
        // CONSTRUCTION.
        /// Constructor.  The text box is invisible by default.
        /// @param[in]  font - The font to use for rendering text.
        /// @param[in]  width_in_pixels - The width of the entire text box, in pixels.
        /// @throws std::exception - Thrown if the font is null.
        explicit TextBox(
            const std::shared_ptr<const Font>& font,
            const unsigned int width_in_pixels);

        // OTHER PUBLIC METHODS.
        /// Configures the text box to start displaying the provided text.
        /// The text box will be made visible.
        /// @param[in]  text - The text to start displaying.
        void StartDisplayingText(const std::string& text);

        /// Updates the text displayed in the text box based on the provided
        /// elapsed amount of time.
        /// @param[in]  elapsed_time_in_seconds - The amount of time elapsed
        ///     since the last update of the text box.
        void Update(const float elapsed_time_in_seconds);

        /// Moves to the next set of 2 lines of text for the text box.
        /// If there is no more text, the text box will be made invisible.
        void MoveToNextText();

        /// Determines if the current 2 lines of text have finished being displayed
        /// in the text box.  Once these 2 lines have been displayed, a player can
        /// press a button to move to the next set of lines.
        /// @return True if the current 2 lines of text have finished being displayed; false otherwise.
        bool CurrentLinesOfTextFinishedBeingDisplayed() const;

        /// Renders the text box to the provided render target.
        /// @param[in,out]  render_target - The render target to render to.
        void Render(sf::RenderTarget& render_target);

        // PUBLIC MEMBER VARIABLES.
        /// True if the text box is visible; false otherwise.
        bool IsVisible = false;

    private:
        // PRIVATE CONSTANTS.
        /// The maximum number of lines that can be displayed in the text box at once.
        const unsigned int LINE_COUNT = 2;

        // PRIVATE MEMBER VARIABLES.
        /// The font used for rendering text.
        std::shared_ptr<const Font> Font = nullptr;
        /// The total width of the text box, in pixels.
        unsigned int WidthInPixels = 0;
        /// The full text to be displayed.
        std::string FullTextToDisplay = "";
        /// The index of the first character in the full text to display.
        unsigned int FirstDisplayedCharacterIndex = 0;
        /// The index of the last character in the full text to display.
        unsigned int LastDisplayedCharacterIndex = 0;
        /// The maximum number of characters that can be displayed within a single line of the text box.
        unsigned int MaxCharacterCountPerLine = 0;
        /// The amount of time elapsed since a character was last displayed.
        float TimeSinceLastCharacterDisplayedInSeconds = 0.0f;
    };
}
