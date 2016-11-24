#include "Core/NullChecking.h"
#include "Graphics/Color.h"
#include "Inventory/InventoryGui.h"

namespace INVENTORY
{
    // The colors right now are arbitrary.
    const GRAPHICS::Color InventoryGui::BIBLE_TAB_COLOR(255, 127, 0);
    const GRAPHICS::Color InventoryGui::ANIMALS_TAB_COLOR = GRAPHICS::Color::RED;
    const GRAPHICS::Color InventoryGui::FOOD_TAB_COLOR = GRAPHICS::Color::GREEN;

    /// Constructor.
    /// @param[in]  inventory - The inventory to display in the GUI.
    /// @throws std::exception - Thrown if a parameter is null.
    InventoryGui::InventoryGui(const std::shared_ptr<const INVENTORY::Inventory>& inventory) :
    Inventory(inventory),
    CurrentTab(TabType::BIBLE),
    BibleVerseTextBox(),
    BibleVerseListBox(inventory)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Inventory,
            "Null inventory provided to HUD.");
    }

    /// Has the inventory GUI respond to the provided user input
    /// @param[in]  input_controller - The controller on which to check user input.
    void InventoryGui::RespondToInput(const INPUT_CONTROL::KeyboardInputController& input_controller)
    {
        // CHECK WHICH TAB IS OPENED.
        switch (CurrentTab)
        {
            case TabType::BIBLE:
            {
                // CHECK WHICH BUTTON WAS PRESSED.
                if (input_controller.UpButtonWasPressed())
                {
                    // SELECT THE PREVIOUS BIBLE VERSE.
                    BibleVerseListBox.SelectPreviousVerse();
                }
                else if (input_controller.DownButtonWasPressed())
                {
                    // SELECT THE NEXT BIBLE VERSE.
                    BibleVerseListBox.SelectNextVerse();
                }
                else if (input_controller.RightButtonWasPressed())
                {
                    // SWITCH TO THE ANIMALS TAB ON THE RIGHT.
                    CurrentTab = TabType::ANIMALS;
                }
                break;
            }
            case TabType::ANIMALS:
            {
                // CHECK WHICH BUTTON WAS PRESSED.
                if (input_controller.LeftButtonWasPressed())
                {
                    // SWITCH TO THE BIBLE TAB ON THE LEFT.
                    CurrentTab = TabType::BIBLE;
                }
                else if (input_controller.RightButtonWasPressed())
                {
                    // SWITCH TO THE FOOD TAB ON THE RIGHT.
                    CurrentTab = TabType::FOOD;
                }
                break;
            }
            case TabType::FOOD:
            {
                // CHECK WHICH BUTTON WAS PRESSED.
                if (input_controller.LeftButtonWasPressed())
                {
                    // SWITCH TO THE ANIMALS TAB ON THE LEFT.
                    CurrentTab = TabType::ANIMALS;
                }
                break;
            }
            default:
                // Nothing to do.
                break;
        }
    }

    /// Renders the inventory GUI to the provided screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryGui::Render(GRAPHICS::Renderer& renderer) const
    {
        // RENDER A RECTANGLE FOR THE BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // top row of the HUD that is always displayed.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = renderer.Screen.HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            renderer.Screen.WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        // The background color is currently arbitrary.
        GRAPHICS::Color background_color;
        background_color.Red = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        background_color.Green = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        background_color.Blue = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        renderer.RenderScreenRectangle(
            background_rectangle,
            background_color);

        // RENDER A TAB FOR THE BIBLE PORTION OF THE GUI.
        // It should be positioned near the top-left of the GUI.
        float bible_tab_left_screen_position_in_pixels = background_rectangle.GetLeftXPosition();
        float bible_tab_top_screen_position_in_pixels = background_rectangle.GetTopYPosition();
        MATH::Vector2f bible_tab_top_left_screen_position_in_pixels(
            bible_tab_left_screen_position_in_pixels,
            bible_tab_top_screen_position_in_pixels);

        // The tab should be big enough to hold the text on the tab.
        std::string bible_tab_text = "Bible";
        unsigned int bible_tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS * bible_tab_text.length();

        MATH::FloatRectangle bible_tab_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            bible_tab_left_screen_position_in_pixels,
            bible_tab_top_screen_position_in_pixels,
            static_cast<float>(bible_tab_text_width_in_pixels),
            static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS));

        renderer.RenderScreenRectangle(
            bible_tab_rectangle,
            BIBLE_TAB_COLOR);

        renderer.RenderText(bible_tab_text, bible_tab_top_left_screen_position_in_pixels, GRAPHICS::Color::BLACK);

        // RENDER A TAB FOR THE ANIMAL PORTION OF THE GUI.
        /// @todo   Centralize tab rendering code in helper function.
        // It should be positioned near the center of the GUI.
        /// @todo   Clean-up calculation of the left position.
        const std::string ANIMALS_TAB_STRING = "Animals";
        unsigned int animals_tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS * ANIMALS_TAB_STRING.length();
        unsigned int animals_tab_text_half_width_in_pixels = animals_tab_text_width_in_pixels / 2;
        float animals_tab_left_screen_position_in_pixels = background_rectangle.GetCenterXPosition() - animals_tab_text_half_width_in_pixels;
        float animals_tab_top_screen_position_in_pixels = background_rectangle.GetTopYPosition();
        MATH::Vector2f animals_tab_top_left_screen_position_in_pixels(
            animals_tab_left_screen_position_in_pixels,
            animals_tab_top_screen_position_in_pixels);

        MATH::FloatRectangle animals_tab_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            animals_tab_left_screen_position_in_pixels,
            animals_tab_top_screen_position_in_pixels,
            static_cast<float>(animals_tab_text_width_in_pixels),
            static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS));

        renderer.RenderScreenRectangle(
            animals_tab_rectangle,
            ANIMALS_TAB_COLOR);

        renderer.RenderText(ANIMALS_TAB_STRING, animals_tab_top_left_screen_position_in_pixels, GRAPHICS::Color::BLACK);

        // RENDER A TAB FOR THE FOOD PORTION OF THE GUI.
        /// @todo   Centralize tab rendering code in helper function.
        // It should be positioned near the center of the GUI.
        /// @todo   Clean-up calculation of the left position.
        const std::string FOOD_TAB_STRING = "Food";
        unsigned int food_tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS * FOOD_TAB_STRING.length();
        float food_tab_left_screen_position_in_pixels = background_rectangle.GetRightXPosition() - food_tab_text_width_in_pixels;
        float food_tab_top_screen_position_in_pixels = background_rectangle.GetTopYPosition();
        MATH::Vector2f food_tab_top_left_screen_position_in_pixels(
            food_tab_left_screen_position_in_pixels,
            food_tab_top_screen_position_in_pixels);

        MATH::FloatRectangle food_tab_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            food_tab_left_screen_position_in_pixels,
            food_tab_top_screen_position_in_pixels,
            static_cast<float>(food_tab_text_width_in_pixels),
            static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS));

        renderer.RenderScreenRectangle(
            food_tab_rectangle,
            FOOD_TAB_COLOR);

        renderer.RenderText(FOOD_TAB_STRING, food_tab_top_left_screen_position_in_pixels, GRAPHICS::Color::BLACK);

        // RENDER THE CURRENTLY DISPLAYED PAGE.
        switch (CurrentTab)
        {
            case TabType::BIBLE:
                RenderBiblePage(renderer);
                break;
            case TabType::ANIMALS:
                RenderAnimalsPage(renderer);
                break;
            case TabType::FOOD:
                RenderFoodPage(renderer);
                break;
        }
    }

    /// Renders the page of the inventory for the Bible tab.
    /// This page allows browsing Bible verses in the inventory.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryGui::RenderBiblePage(GRAPHICS::Renderer& renderer) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = renderer.Screen.HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            renderer.Screen.WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        renderer.RenderScreenRectangle(
            background_rectangle,
            BIBLE_TAB_COLOR);

        // RENDER THE BOX FOR THE MAIN BIBLE VERSE DISPLAY.
        const BIBLE::BibleVerse* const selected_bible_verse = BibleVerseListBox.GetSelectedVerse();

        // The exact positioning/size of this box is tentative.
        const float BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS = 4.0f;
        float bible_verse_text_box_left_screen_position_in_pixels = background_rectangle.GetLeftXPosition() + BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS;
        float bible_verse_text_box_top_screen_position_in_pixels = background_rectangle.GetTopYPosition() + BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS;
        float bible_verse_text_box_width_in_pixels = background_rectangle.GetWidth() / 2.0f;
        const float BIBLE_VERSE_TEXT_BOX_BOTH_SIDES_PADDING_IN_PIXELS = 2.0f * BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS;
        float bible_verse_text_box_height_in_pixels = background_rectangle.GetHeight() - BIBLE_VERSE_TEXT_BOX_BOTH_SIDES_PADDING_IN_PIXELS;
        MATH::FloatRectangle bible_verse_text_box_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            bible_verse_text_box_left_screen_position_in_pixels,
            bible_verse_text_box_top_screen_position_in_pixels,
            bible_verse_text_box_width_in_pixels,
            bible_verse_text_box_height_in_pixels);

        BibleVerseTextBox.Render(
            selected_bible_verse, 
            bible_verse_text_box_rectangle, 
            renderer);

        // RENDER THE BOX FOR THE LIST OF ALL BIBLE VERSES.
        // The exact positioning/size of this box is tentative.
        float bible_verse_list_box_left_screen_position_in_pixels = 
            (bible_verse_text_box_rectangle.GetRightXPosition() + BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS);
        float bible_verse_list_box_top_screen_position_in_pixels = bible_verse_text_box_rectangle.GetTopYPosition();
        // Take up the remaining width (minus padding on the right).
        float bible_verse_list_box_width_in_pixels = 
            background_rectangle.GetWidth() - bible_verse_list_box_left_screen_position_in_pixels - BIBLE_VERSE_TEXT_BOX_SINGLE_SIDE_PADDING_IN_PIXELS;
        float bible_verse_list_box_height_in_pixels = bible_verse_text_box_rectangle.GetHeight();
        MATH::FloatRectangle bible_verse_list_box_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            bible_verse_list_box_left_screen_position_in_pixels,
            bible_verse_list_box_top_screen_position_in_pixels,
            bible_verse_list_box_width_in_pixels,
            bible_verse_list_box_height_in_pixels);

        BibleVerseListBox.Render(
            bible_verse_list_box_rectangle,
            renderer);
    }
    
    /// Renders the page of the inventory for the animals tab.
    /// This page allows browsing animals in the inventory.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryGui::RenderAnimalsPage(GRAPHICS::Renderer& renderer) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = renderer.Screen.HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            renderer.Screen.WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        renderer.RenderScreenRectangle(
            background_rectangle,
            ANIMALS_TAB_COLOR);
    }

    /// Renders the page of the inventory for the food tab.
    /// This page allows browsing food in the inventory.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryGui::RenderFoodPage(GRAPHICS::Renderer& renderer) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = renderer.Screen.HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            renderer.Screen.WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        renderer.RenderScreenRectangle(
            background_rectangle,
            FOOD_TAB_COLOR);
    }
}