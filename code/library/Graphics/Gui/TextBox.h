#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <SFML/System.hpp>
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/TextPage.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"

namespace GRAPHICS::GUI
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
        explicit TextBox() = default;
        explicit TextBox(
            const unsigned int width_in_pixels, 
            const unsigned int height_in_pixels,
            const std::shared_ptr<Font>& font);

        // OTHER PUBLIC METHODS.
        void StartDisplayingText(const std::string& text);

        void Update(const sf::Time& elapsed_time);

        void MoveToNextPage();

        bool CurrentPageOfTextFinishedBeingDisplayed() const;

        void Render(GRAPHICS::Renderer& renderer) const;
        void Render(std::ostream& output_stream) const;

        // PUBLIC MEMBER VARIABLES.
        /// True if the text box is visible; false otherwise.
        bool IsVisible = false;
        /// The font used for rendering text.
        std::shared_ptr<Font> Font = nullptr;
        /// The pages of text currently in the text box.
        std::vector<TextPage> Pages = {};

    private:
        // PRIVATE MEMBER VARIABLES.
        /// The width of the text box (excluding borders) in pixels.
        unsigned int WidthInPixels = GRAPHICS::Screen::DEFAULT_WIDTH_IN_PIXELS;
        /// The height of the text box (excluding borders) in pixels.
        unsigned int HeightInPixels = GRAPHICS::Screen::DEFAULT_HEIGHT_IN_PIXELS;
        /// The index of the current page of text displayed in the text box.
        unsigned int CurrentPageIndex = 0;
        /// True if the arrow indicating that the user can progress the text box is visible; false otherwise.
        bool ContinueArrowVisible = false;
        /// The total elapsed time (in seconds) since the last time the arrow
        /// in the text box was shown or hidden.  Used to make the arrow blink.
        float TotalElapsedTimeInSecondsSinceLastArrowBlink = 0;
    };
}
