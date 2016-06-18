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
        /// Constructor.  The text box is invisible by default.
        /// @param[in]  font - The font to use for rendering text.
        /// @throws std::exception - Thrown if the font is null.
        explicit TextBox(const std::shared_ptr<const Font>& font);

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

        /// Moves to the next page of text for the text box.
        /// If there is no more text, the text box will be made invisible.
        void MoveToNextPage();

        /// Determines if the current page of text has finished being displayed
        /// in the text box.  Once a page has been displayed, a player can
        /// press a button to move to the next page of text.
        /// @return True if the page of text has finished being displayed; false otherwise.
        bool CurrentPageOfTextFinishedBeingDisplayed() const;

        /// Renders the text box to the provided render target.
        /// @param[in,out]  render_target - The render target to render to.
        void Render(sf::RenderTarget& render_target);
        /// Renders the current text page to the provided stream.
        /// @param[in,out]  output_stream - The output stream to render to.
        /// @todo   Create a textual render target?
        void Render(std::ostream& output_stream);

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
