#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/TextPage.h"

namespace GRAPHICS
{
namespace GUI
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
        explicit TextBox(const std::shared_ptr<const Font>& font);

        // OTHER PUBLIC METHODS.
        void StartDisplayingText(const std::string& text);

        void Update(const float elapsed_time_in_seconds);

        void MoveToNextPage();

        bool CurrentPageOfTextFinishedBeingDisplayed() const;

        void Render(sf::RenderTarget& render_target) const;
        void Render(std::ostream& output_stream) const;

        // PUBLIC MEMBER VARIABLES.
        /// True if the text box is visible; false otherwise.
        bool IsVisible = false;

    private:
        // PRIVATE MEMBER VARIABLES.
        /// The font used for rendering text.
        std::shared_ptr<const Font> Font = nullptr;
        /// The pages of text currently in the text box.
        std::vector<TextPage> Pages = std::vector<TextPage>();
        /// The index of the current page of text displayed in the text box.
        unsigned int CurrentPageIndex = 0;
    };
}
}
