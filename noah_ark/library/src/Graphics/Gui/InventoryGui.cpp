#include "Core/NullChecking.h"
#include "Graphics/Color.h"
#include "Graphics/Gui/InventoryGui.h"
#include "Graphics/Gui/Text.h"
#include "Graphics/Rendering.h"
#include "Graphics/Screen.h"

namespace GRAPHICS
{
namespace GUI
{
    InventoryGui::InventoryGui(
        const std::shared_ptr<const OBJECTS::Inventory>& inventory,
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font) :
    Font(font),
    Inventory(inventory),
    CurrentTab(TabType::BIBLE)
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

    void InventoryGui::Render(sf::RenderTarget& render_target)
    {
        // RENDER A RECTANGLE FOR THE BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // top row of the HUD that is always displayed.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = static_cast<float>(GRAPHICS::Screen::HEIGHT_IN_PIXELS) - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            static_cast<float>(GRAPHICS::Screen::WIDTH_IN_PIXELS),
            BACKGROUND_HEIGHT_IN_PIXELS);

        // The background color is currently arbitrary.
        GRAPHICS::Color background_color;
        background_color.Red = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        background_color.Green = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        background_color.Blue = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        RenderScreenRectangle(
            background_rectangle,
            background_color,
            render_target);

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

        MATH::FloatRectangle bible_tab_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            bible_tab_left_screen_position_in_pixels,
            bible_tab_top_screen_position_in_pixels,
            static_cast<float>(bible_tab_text_width_in_pixels),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));

        RenderScreenRectangle(
            bible_tab_rectangle,
            BibleTabColor,
            render_target);

        bible_tab_text.Render(render_target);

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

        MATH::FloatRectangle animals_tab_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            animals_tab_left_screen_position_in_pixels,
            animals_tab_top_screen_position_in_pixels,
            static_cast<float>(animals_tab_text_width_in_pixels),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));

        RenderScreenRectangle(
            animals_tab_rectangle,
            AnimalsTabColor,
            render_target);

        animals_tab_text.Render(render_target);
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

        MATH::FloatRectangle food_tab_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            food_tab_left_screen_position_in_pixels,
            food_tab_top_screen_position_in_pixels,
            static_cast<float>(food_tab_text_width_in_pixels),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));

        RenderScreenRectangle(
            food_tab_rectangle,
            FoodTabColor,
            render_target);

        food_tab_text.Render(render_target);

        // RENDER THE CURRENTLY DISPLAYED PAGE.
        switch (CurrentTab)
        {
            case GRAPHICS::GUI::InventoryGui::TabType::BIBLE:
                RenderBiblePage(render_target);
                break;
            case GRAPHICS::GUI::InventoryGui::TabType::ANIMALS:
                RenderAnimalsPage(render_target);
                break;
            case GRAPHICS::GUI::InventoryGui::TabType::FOOD:
                RenderFoodPage(render_target);
                break;
            default:
                /// @todo   Error-handling?
                break;
        }
    }

    void InventoryGui::RenderBiblePage(sf::RenderTarget& render_target)
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = static_cast<float>(GRAPHICS::Screen::HEIGHT_IN_PIXELS) - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            static_cast<float>(GRAPHICS::Screen::WIDTH_IN_PIXELS),
            BACKGROUND_HEIGHT_IN_PIXELS);

        RenderScreenRectangle(
            background_rectangle,
            BibleTabColor,
            render_target);
    }
    
    void InventoryGui::RenderAnimalsPage(sf::RenderTarget& render_target)
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = static_cast<float>(GRAPHICS::Screen::HEIGHT_IN_PIXELS) - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            static_cast<float>(GRAPHICS::Screen::WIDTH_IN_PIXELS),
            BACKGROUND_HEIGHT_IN_PIXELS);

        RenderScreenRectangle(
            background_rectangle,
            AnimalsTabColor,
            render_target);
    }

    void InventoryGui::RenderFoodPage(sf::RenderTarget& render_target)
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        /// @todo   Figure out a better way to decouple this so that
        /// this assumption isn't baked into the code right here.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * Glyph::HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = static_cast<float>(GRAPHICS::Screen::HEIGHT_IN_PIXELS) - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            static_cast<float>(GRAPHICS::Screen::WIDTH_IN_PIXELS),
            BACKGROUND_HEIGHT_IN_PIXELS);

        RenderScreenRectangle(
            background_rectangle,
            FoodTabColor,
            render_target);
    }
}
}