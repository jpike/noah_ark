#include "Core/NullChecking.h"
#include "Graphics/Color.h"
#include "Graphics/Gui/InventoryGui.h"
#include "Graphics/Gui/Text.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"

namespace GRAPHICS
{
namespace GUI
{
    /// Constructor.
    /// @param[in]  inventory - The inventory to display in the GUI.
    /// @param[in]  font - The font to use for rendering text on the GUI.
    /// @throws std::exception - Thrown if a parameter is null.
    /// @todo   Re-think how we pass assets to this class.
    InventoryGui::InventoryGui(
        const std::shared_ptr<const OBJECTS::Inventory>& inventory,
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font) :
    Font(font),
    Inventory(inventory),
    CurrentTab(TabType::BIBLE),
    BibleVerseTextBox(font),
    BibleVerseListBox(inventory, font)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Inventory,
            "Null inventory provided to HUD.");
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Font,
            "Null font provided to HUD.");

        // INITIALIZE THE COLORS.
        // The Bible tab color is currently an arbitrary orange.
        BibleTabColor.Red = 255;
        BibleTabColor.Green = 127;
        BibleTabColor.Blue = 0;

        // The animals tab color is currently an arbitrary red.
        AnimalsTabColor.Red = 255;
        AnimalsTabColor.Green = 0;
        AnimalsTabColor.Blue = 0;
        
        // The food tab color is currently an arbitrary green.
        FoodTabColor.Red = 0;
        FoodTabColor.Green = 255;
        FoodTabColor.Blue = 0;
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
    /// @param[in,out]  screen - The screen to render to.
    void InventoryGui::Render(Renderer& renderer, Screen& screen) const
    {
        // RENDER A RECTANGLE FOR THE BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // top row of the HUD that is always displayed.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = screen.HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            screen.WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        // The background color is currently arbitrary.
        GRAPHICS::Color background_color;
        background_color.Red = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        background_color.Green = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        background_color.Blue = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        Renderer::RenderScreenRectangle(
            background_rectangle,
            background_color,
            screen);

        // RENDER A TAB FOR THE BIBLE PORTION OF THE GUI.
        // It should be positioned near the top-left of the GUI.
        float bible_tab_left_screen_position_in_pixels = background_rectangle.GetLeftXPosition();
        float bible_tab_top_screen_position_in_pixels = background_rectangle.GetTopYPosition();
        MATH::Vector2ui bible_tab_top_left_screen_position_in_pixels(
            static_cast<unsigned int>(bible_tab_left_screen_position_in_pixels),
            static_cast<unsigned int>(bible_tab_top_screen_position_in_pixels));

        // The tab should be big enough to hold the text on the tab.
        Text bible_tab_text(Font, "Bible", bible_tab_top_left_screen_position_in_pixels);
        unsigned int bible_tab_text_width_in_pixels = bible_tab_text.GetWidthInPixels();;

        MATH::FloatRectangle bible_tab_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            bible_tab_left_screen_position_in_pixels,
            bible_tab_top_screen_position_in_pixels,
            static_cast<float>(bible_tab_text_width_in_pixels),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));

        Renderer::RenderScreenRectangle(
            bible_tab_rectangle,
            BibleTabColor,
            screen);

        bible_tab_text.Render(renderer, screen);

        // RENDER A TAB FOR THE ANIMAL PORTION OF THE GUI.
        /// @todo   Centralize tab rendering code in helper function.
        // It should be positioned near the center of the GUI.
        /// @todo   Clean-up calculation of the left position.
        const std::string ANIMALS_TAB_STRING = "Animals";
        unsigned int animals_tab_text_width_in_pixels = Glyph::WIDTH_IN_PIXELS * ANIMALS_TAB_STRING.length();
        unsigned int animals_tab_text_half_width_in_pixels = animals_tab_text_width_in_pixels / 2;
        float animals_tab_left_screen_position_in_pixels = background_rectangle.GetCenterXPosition() - animals_tab_text_half_width_in_pixels;
        float animals_tab_top_screen_position_in_pixels = background_rectangle.GetTopYPosition();
        MATH::Vector2ui animals_tab_top_left_screen_position_in_pixels(
            static_cast<unsigned int>(animals_tab_left_screen_position_in_pixels),
            static_cast<unsigned int>(animals_tab_top_screen_position_in_pixels));

        // The tab should be big enough to hold the text on the tab.
        Text animals_tab_text(Font, ANIMALS_TAB_STRING, animals_tab_top_left_screen_position_in_pixels);

        MATH::FloatRectangle animals_tab_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            animals_tab_left_screen_position_in_pixels,
            animals_tab_top_screen_position_in_pixels,
            static_cast<float>(animals_tab_text_width_in_pixels),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));

        Renderer::RenderScreenRectangle(
            animals_tab_rectangle,
            AnimalsTabColor,
            screen);

        animals_tab_text.Render(renderer, screen);

        // RENDER A TAB FOR THE FOOD PORTION OF THE GUI.
        /// @todo   Centralize tab rendering code in helper function.
        // It should be positioned near the center of the GUI.
        /// @todo   Clean-up calculation of the left position.
        const std::string FOOD_TAB_STRING = "Food";
        unsigned int food_tab_text_width_in_pixels = Glyph::WIDTH_IN_PIXELS * FOOD_TAB_STRING.length();
        float food_tab_left_screen_position_in_pixels = background_rectangle.GetRightXPosition() - food_tab_text_width_in_pixels;
        float food_tab_top_screen_position_in_pixels = background_rectangle.GetTopYPosition();
        MATH::Vector2ui food_tab_top_left_screen_position_in_pixels(
            static_cast<unsigned int>(food_tab_left_screen_position_in_pixels),
            static_cast<unsigned int>(food_tab_top_screen_position_in_pixels));

        // The tab should be big enough to hold the text on the tab.
        Text food_tab_text(Font, FOOD_TAB_STRING, food_tab_top_left_screen_position_in_pixels);

        MATH::FloatRectangle food_tab_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            food_tab_left_screen_position_in_pixels,
            food_tab_top_screen_position_in_pixels,
            static_cast<float>(food_tab_text_width_in_pixels),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));

        Renderer::RenderScreenRectangle(
            food_tab_rectangle,
            FoodTabColor,
            screen);

        food_tab_text.Render(renderer, screen);

        // RENDER THE CURRENTLY DISPLAYED PAGE.
        switch (CurrentTab)
        {
            case GRAPHICS::GUI::InventoryGui::TabType::BIBLE:
                RenderBiblePage(renderer, screen);
                break;
            case GRAPHICS::GUI::InventoryGui::TabType::ANIMALS:
                RenderAnimalsPage(screen);
                break;
            case GRAPHICS::GUI::InventoryGui::TabType::FOOD:
                RenderFoodPage(screen);
                break;
            default:
                /// @todo   Error-handling?
                break;
        }
    }

    /// Renders the page of the inventory for the Bible tab.
    /// This page allows browsing Bible verses in the inventory.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    /// @param[in,out]  screen - The screen to render to.
    void InventoryGui::RenderBiblePage(Renderer& renderer, Screen& screen) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = screen.HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            screen.WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        Renderer::RenderScreenRectangle(
            background_rectangle,
            BibleTabColor,
            screen);

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
            renderer,
            screen);

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
            renderer,
            screen);
    }
    
    /// Renders the page of the inventory for the animals tab.
    /// This page allows browsing animals in the inventory.
    /// @param[in,out]  screen - The screen to render to.
    void InventoryGui::RenderAnimalsPage(Screen& screen) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = screen.HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            screen.WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        Renderer::RenderScreenRectangle(
            background_rectangle,
            AnimalsTabColor,
            screen);
    }

    /// Renders the page of the inventory for the food tab.
    /// This page allows browsing food in the inventory.
    /// @param[in,out]  screen - The screen to render to.
    void InventoryGui::RenderFoodPage(Screen& screen) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = screen.HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            screen.WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        Renderer::RenderScreenRectangle(
            background_rectangle,
            FoodTabColor,
            screen);
    }
}
}