#include <cassert>
#include "Core/NullChecking.h"
#include "Inventory/InventoryFoodPage.h"
#include "Resources/FoodGraphics.h"

namespace INVENTORY
{
    // The exact color is currently arbitrary.
    const GRAPHICS::Color InventoryFoodPage::BACKGROUND_COLOR = GRAPHICS::Color::GREEN;

    /// Constructor.
    /// @param[in]  inventory - The inventory to display in the GUI.
    /// @param[in]  assets - The assets to use for the page.
    /// @throws std::exception - Thrown if a parameter is null.
    InventoryFoodPage::InventoryFoodPage(
        const std::shared_ptr<const INVENTORY::Inventory>& inventory,
        const std::shared_ptr<RESOURCES::Assets>& assets) :
    Inventory(inventory),
    Assets(assets)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(Inventory, "Null inventory provided to inventory food page.");
        CORE::ThrowInvalidArgumentExceptionIfNull(Assets, "Null assets provided to inventory food page.");
    }

    /// Renders the inventory GUI food page to the provided screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryFoodPage::Render(GRAPHICS::Renderer& renderer) const
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
            BACKGROUND_COLOR);

        // CALCULATE HOW MANY FOOD BOXES CAN APPEAR PER ROW AND COLUMN.
        // Some padding is added around the boxes in order to space them out.
        const float BOX_DIMENSION_IN_PIXELS = 48.0f;
        const float PADDING_BETWEEN_BOXES_IN_PIXELS = 16.0f;
        const float BOX_WITH_PADDING_DIMENSION_IN_PIXELS = BOX_DIMENSION_IN_PIXELS + PADDING_BETWEEN_BOXES_IN_PIXELS;
        float page_width_in_pixels = background_rectangle.GetWidth();
        unsigned int boxes_per_row = static_cast<unsigned int>(page_width_in_pixels / BOX_WITH_PADDING_DIMENSION_IN_PIXELS);

        float page_height_in_pixels = background_rectangle.GetHeight();
        unsigned int boxes_per_column = static_cast<unsigned int>(page_height_in_pixels / BOX_WITH_PADDING_DIMENSION_IN_PIXELS);

        // RENDER BOXES FOR EACH COLLECTED FOOD TYPE.
        const float BOX_HALF_DIMENSION_IN_PIXELS = BOX_DIMENSION_IN_PIXELS / 2.0f;
        float starting_box_center_x_position = background_rectangle.GetLeftXPosition() + PADDING_BETWEEN_BOXES_IN_PIXELS + BOX_HALF_DIMENSION_IN_PIXELS;
        float starting_box_center_y_position = background_rectangle.GetTopYPosition() + PADDING_BETWEEN_BOXES_IN_PIXELS + BOX_HALF_DIMENSION_IN_PIXELS;
        const unsigned int FIRST_VALID_FOOD_ID = static_cast<unsigned int>(OBJECTS::FoodType::NONE) + 1;
        for (unsigned int food_id = FIRST_VALID_FOOD_ID; food_id < static_cast<unsigned int>(OBJECTS::FoodType::COUNT); ++food_id)
        {
            // GET THE COLLECTED COUNT FOR THE FOOD TYPE.
            OBJECTS::FoodType food_type = static_cast<OBJECTS::FoodType>(food_id);
            unsigned int food_collected_count = Inventory->GetCollectedFoodCount(food_type);

            // CALCULATE THE BOX FOR THIS FOOD TYPE.
            /// @todo Support scrolling through multiples "pages" within this inventory page
            /// as not all food will be able to fit on a single screen.
            // Since the first food enum isn't valid, the ID must be adjusted to
            // make things zero-based for calculating the box indices.
            unsigned int zero_based_food_id = food_id - 1;
            unsigned int box_row_index = zero_based_food_id / boxes_per_row;
            unsigned int box_column_index = zero_based_food_id % boxes_per_row;

            float current_box_x_offset = static_cast<float>(box_column_index * BOX_WITH_PADDING_DIMENSION_IN_PIXELS);
            float current_box_center_x_position = starting_box_center_x_position + current_box_x_offset;

            float current_box_y_offset = static_cast<float>(box_row_index * BOX_WITH_PADDING_DIMENSION_IN_PIXELS);
            float current_box_center_y_position = starting_box_center_y_position + current_box_y_offset;

            MATH::FloatRectangle box_screen_rectangle = MATH::FloatRectangle::FromCenterAndDimensions(
                current_box_center_x_position,
                current_box_center_y_position,
                BOX_DIMENSION_IN_PIXELS,
                BOX_DIMENSION_IN_PIXELS);

            // RENDER A BOX FOR THE FOOD TYPE.
            RenderFoodBox(
                food_type,
                food_collected_count,
                box_screen_rectangle,
                renderer);
        }
    }

    /// Renders a box for a type of food on the page.
    /// @param[in]  food_type - The type of food to render.
    /// @param[in]  food_type_collected_count - The count of food items collected of this type.
    /// @param[in]  box_screen_rectangle - The placement/dimensions of the box to render, in screen coordinates.
    /// @param[in,out]  renderer - The renderer to use.
    void InventoryFoodPage::RenderFoodBox(
        const OBJECTS::FoodType food_type,
        const unsigned int food_type_collected_count,
        const MATH::FloatRectangle& box_screen_rectangle,
        GRAPHICS::Renderer& renderer) const
    {
        // RENDER A SLIGHTLY DARKER RECTANGLE FOR THE BACKGROUND.
        const uint8_t ARBITRARY_BACKGROUND_COLOR_SCALE_FACTOR = 2;
        GRAPHICS::Color food_box_background_color = BACKGROUND_COLOR;
        food_box_background_color.ScaleRgbDown(ARBITRARY_BACKGROUND_COLOR_SCALE_FACTOR);
        renderer.RenderScreenRectangle(box_screen_rectangle, food_box_background_color);

        // RENDER INFORMATION ABOUT ANY COLLECTED FOOD OF THE TYPE.
        // No icon is rendered if the player hasn't collected any of the type of food
        // in order to keep food types hidden from players until they are discovered.
        bool food_type_collected = (food_type_collected_count > 0);
        if (food_type_collected)
        {
            // RENDER AN ICON FOR THE FOOD.
            // It should appear in the left-top corner of the box.
            std::shared_ptr<GRAPHICS::Sprite> food_sprite = RESOURCES::FoodGraphics::GetSprite(food_type, *Assets);
            assert(food_sprite);
            MATH::Vector2f food_icon_left_top_screen_position = box_screen_rectangle.GetLeftXTopYPosition();
            renderer.RenderGuiIcon(*food_sprite, food_icon_left_top_screen_position);

            // RENDER THE COLLECTED COUNT FOR THE FOOD TYPE.
            // It should appear just to the right of the food icon.
            std::string collected_count_text = "x" + std::to_string(food_type_collected_count);
            MATH::Vector2f collected_count_left_top_screen_position = food_icon_left_top_screen_position;
            collected_count_left_top_screen_position.X += food_sprite->GetWidthInPixels();
            renderer.RenderText(collected_count_text, collected_count_left_top_screen_position);
        }
    }
}
