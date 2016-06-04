#include <cassert>
#include <stdexcept>
#include "Core/String.h"
#include "Graphics/TextBox.h"

namespace GRAPHICS
{
    TextBox::TextBox(const std::shared_ptr<const GRAPHICS::Font>& font) :
    Font(font)
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
        // CLEAR ANY DATA FROM THE LAST TIME TEXT WAS DISPLAYED.
        Pages.clear();

        // SPLIT THE TEXT INTO WORDS.
        // In order to avoid having a word broken up onto multiple lines,
        // word boundaries need to be determined.  Splitting the text
        // into words is a simple way to do that.  This will discard
        // multiple sequences of spaces, but that is acceptable
        // since space in the text box is more efficiently used by
        // only having one space between words.  There also is not
        // a use case yet where having multiple sequential spaces
        // is valuable.
        std::deque<std::string> words = CORE::String::SplitIntoWords(text, TextPage::MAX_CHARACTER_COUNT);

        bool words_exist = !words.empty();
        if (!words_exist)
        {
            // With no words, additional work doesn't need to be performed
            // since there is nothing to display.
            return;
        }

        // DIVIDE THE WORDS INTO APPROPRIATE PAGES.
        // The text might not all fit onto a single page, so multiple pages
        // of text might be needed.  At least one initial page will be needed.
        Pages.push_back(TextPage());
        TextPage* current_page = &Pages.back();
        CurrentPageIndex = 0;
        while (!words.empty())
        {
            // TRY TO ADD THE CURRENT WORD TO THE CURRENT PAGE.
            const std::string& current_word = words.front();
            bool word_added = current_page->Add(current_word);

            // ADD THE WORD TO A NEW PAGE IF IT WASN'T ADDED TO THE CURRENT PAGE.
            // If it wasn't added, there wasn't room on the current page.
            if (!word_added)
            {
                Pages.push_back(TextPage());
                current_page = &Pages.back();
                word_added = current_page->Add(current_word);   
            }

            // The current word has been added to a page, so it is no longer needed.
            assert(word_added);
            words.pop_front();
        }

        // MAKE THE TEXT BOX VISIBLE SO THAT TEXT CAN BE RENDERED.
        IsVisible = true;
    }

    void TextBox::Update(const float elapsed_time_in_seconds)
    {
        // CHECK IF ANY PAGES OF TEXT EXIST.
        bool text_pages_exist = !Pages.empty();
        if (!text_pages_exist)
        {
            // No updates are required if no pages exist.
            return;
        }

        // UPDATE THE CURRENT PAGE OF TEXT.
        auto& current_text_page = Pages[CurrentPageIndex];
        current_text_page.Update(elapsed_time_in_seconds);
    }

    void TextBox::MoveToNextPage()
    {
        // MOVE TO THE NEXT PAGE OF TEXT.
        ++CurrentPageIndex;

        // HIDE THE TEXT BOX IF THERE IS NO MORE TEXT.
        bool all_text_displayed = (CurrentPageIndex >= Pages.size());
        if (all_text_displayed)
        {
            IsVisible = false;

            // CLEAR THE PAGES OF TEXT.
            CurrentPageIndex = 0;
            Pages.clear();
        }
    }

    bool TextBox::CurrentPageOfTextFinishedBeingDisplayed() const
    {
        // CHECK IF ANY PAGES OF TEXT EXIST.
        bool text_pages_exist = !Pages.empty();
        if (!text_pages_exist)
        {
            // All text has been displayed if no pages exist.
            return true;
        }

        // CHECK IF ALL TEXT HAS BEEN DISPLAYED FOR THE CURRENT PAGE.
        auto& current_text_page = Pages[CurrentPageIndex];
        bool current_page_of_text_fully_displayed = current_text_page.AllTextDisplayed();
        return current_page_of_text_fully_displayed;
    }

    void TextBox::Render(sf::RenderTarget& render_target)
    {
        // CHECK IF ANY PAGES OF TEXT EXIST.
        bool text_pages_exist = !Pages.empty();
        if (!text_pages_exist)
        {
            // There is nothing to render if no pages exist.
            return;
        }

        // DRAW A BOX TO HOLD THE TEXT.
        sf::RectangleShape box;
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        const float OUTLINE_THICKNESS_IN_PIXELS = 4.0f;
        box.setOutlineThickness(OUTLINE_THICKNESS_IN_PIXELS);

        const float VERTICAL_PADDING_IN_PIXELS = 16.0f;
        float height_in_pixels = (TextPage::LINE_COUNT * Glyph::HEIGHT_IN_PIXELS) + VERTICAL_PADDING_IN_PIXELS - OUTLINE_THICKNESS_IN_PIXELS * 2;
        float width_in_pixels = (Screen::WIDTH_IN_PIXELS) - OUTLINE_THICKNESS_IN_PIXELS * 2;
        box.setSize(sf::Vector2f(width_in_pixels, height_in_pixels));

        const sf::Vector2i SCREEN_TOP_LEFT_CORNER(0, 0);
        sf::Vector2f top_left_corner_world_position = render_target.mapPixelToCoords(SCREEN_TOP_LEFT_CORNER);
        top_left_corner_world_position.x += OUTLINE_THICKNESS_IN_PIXELS;
        top_left_corner_world_position.y += OUTLINE_THICKNESS_IN_PIXELS;
        box.setPosition(top_left_corner_world_position);

        render_target.draw(box);

        // DRAW THE CURRENT PAGE OF TEXT.
        auto& current_text_page = Pages[CurrentPageIndex];

        const unsigned int FIRST_LINE_VERTICAL_PADDING_IN_PIXELS = static_cast<unsigned int>(VERTICAL_PADDING_IN_PIXELS / 2.0f);
        MATH::Vector2ui text_page_top_left_screen_position_in_pixels;
        text_page_top_left_screen_position_in_pixels.X = SCREEN_TOP_LEFT_CORNER.x + Glyph::WIDTH_IN_PIXELS;
        text_page_top_left_screen_position_in_pixels.Y = SCREEN_TOP_LEFT_CORNER.y + FIRST_LINE_VERTICAL_PADDING_IN_PIXELS;

        current_text_page.Render(text_page_top_left_screen_position_in_pixels, Font, render_target);
    }

    void TextBox::Render(std::ostream& output_stream)
    {
        // CHECK IF ANY PAGES OF TEXT EXIST.
        bool text_pages_exist = !Pages.empty();
        if (!text_pages_exist)
        {
            // There is nothing to render if no pages exist.
            return;
        }

        // DRAW THE CURRENT PAGE OF TEXT.
        auto& current_text_page = Pages[CurrentPageIndex];
        current_text_page.Render(output_stream);
    }
}
