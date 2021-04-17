#include "Graphics/Color.h"
#include "Inventory/InventoryGui.h"

namespace INVENTORY
{
    /// Updates the inventory GUI.
    /// @param[in]  elapsed_time - The elapsed time since the last frame.
    /// @param[in]  input_controller - The controller on which to check user input.
    void InventoryGui::Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller)
    {
        // CHECK WHICH TAB IS OPENED.
        switch (CurrentTab)
        {
            case TabType::BIBLE:
            {
                // CHECK WHICH BUTTON WAS PRESSED.
                // Scrolling between verses is done via 3 scenarios:
                // 1. A complete arrow key press + release.  This ensures that even if the press/release occurs
                //      really quickly, the user will still see the list scroll to the next row.
                // 2. Holding down a button after a sufficient amount of time.  Having the user press and release a key
                //      for scrolling through verses can take too long.  Therefore, to allow easier scrolling, scrolling
                //      is allowed if the user holds an appropriate key down long enough.  We can't just always check if
                //      the keys are down since that would result in scrolling too fast - potentially preventing the player
                //      from being able to land on selecting a particular verse.  The exact time used here is slightly
                //      longer than that for the animals page since more reading is expected on this page.
                // 3. Using the page up/down arrow keys for quicker scrolling.
                const sf::Time ELAPSED_TIME_FOR_KEY_DOWN_BEFORE_MOVING_TO_ADJACENT_VERSE = sf::seconds(0.15f);
                constexpr unsigned int VERSE_JUMP_COUNT_PER_PAGE_UP_OR_DOWN = 10;
                if (input_controller.ButtonWasPressed(sf::Keyboard::Up))
                {
                    // MOVE TO THE PREVIOUS VERSE SINCE A COMPLETE BUTTON PRESS WAS DETECTED.
                    BiblePage.BibleVerseListBox.SelectPreviousVerse();

                    // Since the previous verse has been switched to, the timer needs to be reset.
                    BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses = sf::Time::Zero;
                }
                else if (input_controller.ButtonDown(sf::Keyboard::Up))
                {
                    // UPDATE THE ELAPSED TIME FOR A SCROLLING KEY BEING PRESSED.
                    BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses += elapsed_time;

                    // SELECT THE PREVIOUS BIBLE VERSE IF ENOUGH TIME HAS PASSED.
                    bool scroll_key_held_down_long_enough = (BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses >= ELAPSED_TIME_FOR_KEY_DOWN_BEFORE_MOVING_TO_ADJACENT_VERSE);
                    if (scroll_key_held_down_long_enough)
                    {
                        BiblePage.BibleVerseListBox.SelectPreviousVerse();

                        // Since the previous verse has been switched to, the timer needs to be reset.
                        BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses = sf::Time::Zero;
                    }
                }
                else if (input_controller.ButtonWasPressed(sf::Keyboard::PageUp))
                {
                    // SELECT THE PREVIOUS VERSE UP TO THE MAX NUMBER OF TIMES.
                    for (unsigned int verse_count = 0; verse_count < VERSE_JUMP_COUNT_PER_PAGE_UP_OR_DOWN; ++verse_count)
                    {
                        BiblePage.BibleVerseListBox.SelectPreviousVerse();
                    }

                    // Since a previous verse has been switched to, the timer needs to be reset.
                    BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses = sf::Time::Zero;
                }
                else if (input_controller.ButtonWasPressed(sf::Keyboard::Down))
                {
                    // MOVE TO THE NEXT VERSE SINCE A COMPLETE BUTTON PRESS WAS DETECTED.
                    BiblePage.BibleVerseListBox.SelectNextVerse();

                    // Since the previous verse has been switched to, the timer needs to be reset.
                    BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses = sf::Time::Zero;
                }
                else if (input_controller.ButtonDown(sf::Keyboard::Down))
                {
                    // UPDATE THE ELAPSED TIME FOR A SCROLLING KEY BEING PRESSED.
                    BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses += elapsed_time;

                    // SELECT THE NEXT BIBLE VERSE IF ENOUGH TIME HAS PASSED.
                    bool scroll_key_held_down_long_enough = (BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses >= ELAPSED_TIME_FOR_KEY_DOWN_BEFORE_MOVING_TO_ADJACENT_VERSE);
                    if (scroll_key_held_down_long_enough)
                    {
                        BiblePage.BibleVerseListBox.SelectNextVerse();

                        // Since the next verse has been switched to, the timer needs to be reset.
                        BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses = sf::Time::Zero;
                    }
                }
                else if (input_controller.ButtonWasPressed(sf::Keyboard::PageDown))
                {
                    // SELECT THE NEXT VERSE UP TO THE MAX NUMBER OF TIMES.
                    for (unsigned int verse_count = 0; verse_count < VERSE_JUMP_COUNT_PER_PAGE_UP_OR_DOWN; ++verse_count)
                    {
                        BiblePage.BibleVerseListBox.SelectNextVerse();
                    }

                    // Since a next verse has been switched to, the timer needs to be reset.
                    BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses = sf::Time::Zero;
                }
                else if (input_controller.ButtonWasPressed(sf::Keyboard::Right))
                {
                    // SWITCH TO THE ANIMALS TAB ON THE RIGHT.
                    CurrentTab = TabType::ANIMALS;
                }
                else
                {
                    // RESET THE TIMER TRACKING HOW LONG A SCROLL KEY HAS BEEN DOWN SINCE NO KEY WAS DOWN THIS FRAME.
                    BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses = sf::Time::Zero;
                }
                break;
            }
            case TabType::ANIMALS:
            {
                // UPDATE THE ANIMALS PAGE.
                AnimalsPage.Update(elapsed_time, input_controller);

                // RESPOND TO INPUT THAT MIGHT SWITCH THE TAB OF THE INVENTORY GUI.
                if (input_controller.ButtonWasPressed(sf::Keyboard::Left))
                {
                    // SWITCH TO THE BIBLE TAB ON THE LEFT.
                    CurrentTab = TabType::BIBLE;

                    // RESET THE TIMER TRACKING HOW LONG A SCROLL KEY HAS BEEN DOWN ON THE BIBLE PAGE SINCE WE'RE NEWLY SWITCHING TO IT.
                    BiblePage.ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingVerses = sf::Time::Zero;
                }
                else if (input_controller.ButtonWasPressed(sf::Keyboard::Right))
                {
                    // SWITCH TO THE FOOD TAB ON THE RIGHT.
                    CurrentTab = TabType::FOOD;
                }
                break;
            }
            case TabType::FOOD:
            {
                // UPDATE THE FOOD PAGE.
                FoodPage.Update(elapsed_time, input_controller);

                // SWITCH TABS IF NO FOOD SELECTION IS IN-PROGRESS AND AN APPROPRIATE BUTTON IS PRESSED.
                // The selected indices need to be checked rather than the food type itself since some boxes
                // may not have any food in them.
                bool food_selection_in_progress = (
                    (InventoryFoodPage::UNSELECTED_ROW_INDEX != FoodPage.CurrentlySelectedRowIndex) &&
                    (InventoryFoodPage::UNSELECTED_COLUMN_INDEX != FoodPage.CurrentlySelectedColumnIndex));
                if (!food_selection_in_progress)
                {
                    // CHECK WHICH BUTTON WAS PRESSED.
                    if (input_controller.ButtonWasPressed(sf::Keyboard::Left))
                    {
                        // SWITCH TO THE ANIMALS TAB ON THE LEFT.
                        CurrentTab = TabType::ANIMALS;
                    }
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
    void InventoryGui::Render(const STATES::SavedGameData& saved_game_data, GRAPHICS::Renderer& renderer)
    {
        // RENDER A RECTANGLE FOR THE BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // top row of the HUD that is always displayed.  Otherwise,
        // it should cover the remainder of the screen.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = renderer.Screen->HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            renderer.Screen->WidthInPixels<float>(),
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
        MATH::Vector2f bible_tab_left_top_screen_position_in_pixels = background_rectangle.LeftTop;
        RenderTab(
            "Bible",
            bible_tab_left_top_screen_position_in_pixels,
            InventoryBiblePage::BACKGROUND_COLOR,
            renderer);

        // RENDER A TAB FOR THE ANIMAL PORTION OF THE GUI.
        const std::string ANIMALS_TAB_STRING = "Animals";
        size_t animals_tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS * ANIMALS_TAB_STRING.length();
        size_t animals_tab_text_half_width_in_pixels = animals_tab_text_width_in_pixels / 2;
        float animals_tab_left_screen_position_in_pixels = background_rectangle.CenterX() - animals_tab_text_half_width_in_pixels;
        float animals_tab_top_screen_position_in_pixels = background_rectangle.LeftTop.Y;
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
        size_t food_tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS * FOOD_TAB_STRING.length();
        float food_tab_left_screen_position_in_pixels = background_rectangle.RightBottom.X - food_tab_text_width_in_pixels;
        float food_tab_top_screen_position_in_pixels = background_rectangle.LeftTop.Y;
        MATH::Vector2f food_tab_left_top_screen_position_in_pixels(
            food_tab_left_screen_position_in_pixels,
            food_tab_top_screen_position_in_pixels);
        RenderTab(
            FOOD_TAB_STRING,
            food_tab_left_top_screen_position_in_pixels,
            InventoryFoodPage::BACKGROUND_COLOR,
            renderer);

        // RENDER THE CURRENTLY DISPLAYED PAGE.
        switch (CurrentTab)
        {
            case TabType::BIBLE:
                BiblePage.Render(saved_game_data.Player->Inventory, renderer);
                break;
            case TabType::ANIMALS:
                AnimalsPage.Render(saved_game_data, renderer);
                break;
            case TabType::FOOD:
                FoodPage.Render(saved_game_data.Player->Inventory, renderer);
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
        size_t tab_text_width_in_pixels = GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS * tab_text.length();
        MATH::FloatRectangle tab_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            left_top_screen_position_in_pixels.X,
            left_top_screen_position_in_pixels.Y,
            static_cast<float>(tab_text_width_in_pixels),
            static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS));

        renderer.RenderScreenRectangle(
            tab_rectangle,
            background_color);

        // RENDER THE TEXT FOR THE TAB.
        renderer.RenderText(tab_text, RESOURCES::AssetId::FONT_TEXTURE, left_top_screen_position_in_pixels, GRAPHICS::Color::BLACK);
    }
}