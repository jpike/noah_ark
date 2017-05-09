#include "Core/NullChecking.h"
#include "Graphics/Color.h"
#include "Inventory/InventoryGui.h"

namespace INVENTORY
{
    // The color right now is arbitrary.
    const GRAPHICS::Color InventoryGui::FOOD_TAB_COLOR = GRAPHICS::Color::GREEN;

    /// Constructor.
    /// @param[in]  inventory - The inventory to display in the GUI.
    /// @param[in]  assets - The assets to use for the page.
    /// @throws std::exception - Thrown if a parameter is null.
    InventoryGui::InventoryGui(
        const std::shared_ptr<const INVENTORY::Inventory>& inventory,
        const std::shared_ptr<RESOURCES::Assets>& assets) :
    Inventory(inventory),
    CurrentTab(TabType::BIBLE),
    BiblePage(inventory),
    AnimalsPage(inventory, assets)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(Inventory, "Null inventory provided to HUD.");
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
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::UP_KEY))
                {
                    // SELECT THE PREVIOUS BIBLE VERSE.
                    BiblePage.BibleVerseListBox.SelectPreviousVerse();
                }
                else if (input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::DOWN_KEY))
                {
                    // SELECT THE NEXT BIBLE VERSE.
                    BiblePage.BibleVerseListBox.SelectNextVerse();
                }
                else if (input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::RIGHT_KEY))
                {
                    // SWITCH TO THE ANIMALS TAB ON THE RIGHT.
                    CurrentTab = TabType::ANIMALS;
                }
                break;
            }
            case TabType::ANIMALS:
            {
                // CHECK WHICH BUTTON WAS PRESSED.
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::LEFT_KEY))
                {
                    // SWITCH TO THE BIBLE TAB ON THE LEFT.
                    CurrentTab = TabType::BIBLE;
                }
                else if (input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::RIGHT_KEY))
                {
                    // SWITCH TO THE FOOD TAB ON THE RIGHT.
                    CurrentTab = TabType::FOOD;
                }
                break;
            }
            case TabType::FOOD:
            {
                // CHECK WHICH BUTTON WAS PRESSED.
                if (input_controller.ButtonWasPressed(INPUT_CONTROL::KeyboardInputController::LEFT_KEY))
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
        MATH::Vector2f bible_tab_left_top_screen_position_in_pixels(
            bible_tab_left_screen_position_in_pixels,
            bible_tab_top_screen_position_in_pixels);
        RenderTab(
            "Bible",
            bible_tab_left_top_screen_position_in_pixels,
            InventoryBiblePage::BACKGROUND_COLOR,
            renderer);

        // RENDER A TAB FOR THE ANIMAL PORTION OF THE GUI.
        const std::string ANIMALS_TAB_STRING = "Animals";
        unsigned int animals_tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS * ANIMALS_TAB_STRING.length();
        unsigned int animals_tab_text_half_width_in_pixels = animals_tab_text_width_in_pixels / 2;
        float animals_tab_left_screen_position_in_pixels = background_rectangle.GetCenterXPosition() - animals_tab_text_half_width_in_pixels;
        float animals_tab_top_screen_position_in_pixels = background_rectangle.GetTopYPosition();
        MATH::Vector2f animals_tab_left_top_screen_position_in_pixels(
            animals_tab_left_screen_position_in_pixels,
            animals_tab_top_screen_position_in_pixels);
        RenderTab(
            ANIMALS_TAB_STRING,
            animals_tab_left_top_screen_position_in_pixels,
            InventoryAnimalsPage::BACKGROUND_COLOR,
            renderer);

        // RENDER A TAB FOR THE FOOD PORTION OF THE GUI.
        const std::string FOOD_TAB_STRING = "Food";
        unsigned int food_tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS * FOOD_TAB_STRING.length();
        float food_tab_left_screen_position_in_pixels = background_rectangle.GetRightXPosition() - food_tab_text_width_in_pixels;
        float food_tab_top_screen_position_in_pixels = background_rectangle.GetTopYPosition();
        MATH::Vector2f food_tab_left_top_screen_position_in_pixels(
            food_tab_left_screen_position_in_pixels,
            food_tab_top_screen_position_in_pixels);
        RenderTab(
            FOOD_TAB_STRING,
            food_tab_left_top_screen_position_in_pixels,
            FOOD_TAB_COLOR,
            renderer);

        // RENDER THE CURRENTLY DISPLAYED PAGE.
        switch (CurrentTab)
        {
            case TabType::BIBLE:
                BiblePage.Render(renderer);
                break;
            case TabType::ANIMALS:
                AnimalsPage.Render(renderer);
                break;
            case TabType::FOOD:
                RenderFoodPage(renderer);
                break;
        }
    }

    /// Renders a tab that is part of the inventory GUI.
    /// @param[in]  tab_text - The text to display on the tab.
    /// @param[in]  left_top_screen_position_in_pixels - The left-top screen position of the tab.
    /// @param[in]  background_color - The background color of the tab.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryGui::RenderTab(
        const std::string& tab_text,
        const MATH::Vector2f& left_top_screen_position_in_pixels,
        const GRAPHICS::Color& background_color,
        GRAPHICS::Renderer& renderer) const
    {
        // RENDER A BACKGROUND RECTANGLE FOR THE TAB.
        unsigned int tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS * tab_text.length();
        MATH::FloatRectangle tab_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            left_top_screen_position_in_pixels.X,
            left_top_screen_position_in_pixels.Y,
            static_cast<float>(tab_text_width_in_pixels),
            static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS));

        renderer.RenderScreenRectangle(
            tab_rectangle,
            background_color);

        // RENDER THE TEXT FOR THE TAB.
        renderer.RenderText(tab_text, left_top_screen_position_in_pixels, GRAPHICS::Color::BLACK);
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