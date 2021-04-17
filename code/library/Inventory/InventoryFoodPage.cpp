#include "Inventory/InventoryFoodPage.h"
#include "Math/Number.h"
#include "Resources/FoodGraphics.h"

namespace INVENTORY
{
    // The exact color is currently arbitrary.
    const GRAPHICS::Color InventoryFoodPage::BACKGROUND_COLOR = GRAPHICS::Color::GREEN;

    /// Updates the food page.
    /// @param[in]  elapsed_time - The elapsed time since the last frame.
    /// @param[in]  input_controller - The controller on which to check user input.
    void InventoryFoodPage::Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller)
    {
        /// @todo
        elapsed_time;

        // CHECK WHICH BUTTONS WERE PRESSED.
        if (input_controller.ButtonWasPressed(sf::Keyboard::Left))
        {
            --CurrentlySelectedColumnIndex;
        }
        else if (input_controller.ButtonWasPressed(sf::Keyboard::Right))
        {
            ++CurrentlySelectedColumnIndex;
        }
        else if (input_controller.ButtonWasPressed(sf::Keyboard::Up))
        {
            --CurrentlySelectedRowIndex;
        }
        else if (input_controller.ButtonWasPressed(sf::Keyboard::Down))
        {
            ++CurrentlySelectedRowIndex;

            // CHECK IF THIS IS THE FIRST TIME A USER IS MOVING TO SELECT A FOOD.
            constexpr int MIN_SELECTED_ROW_INDEX = 0;
            bool user_first_initiating_food_selection = (
                (MIN_SELECTED_ROW_INDEX == CurrentlySelectedRowIndex) &&
                (UNSELECTED_COLUMN_INDEX == CurrentlySelectedColumnIndex));
            if (user_first_initiating_food_selection)
            {
                // MOVE TO THE FIRST VALID FOOD FOR SELECTION.
                // This provides an easy way for users to move from normal switching between
                // inventory pages to selecting food.
                constexpr int MIN_SELECTED_COLUMN_INDEX = 0;
                CurrentlySelectedColumnIndex = MIN_SELECTED_COLUMN_INDEX;
            }
        }
        else if (input_controller.ButtonWasPressed(INPUT_CONTROL::InputController::PRIMARY_ACTION_KEY))
        {
            /// @todo   Place food if appropriate!
        }

        // KEEP THE SELECTED INDICES WITHIN RANGE.
        constexpr int MAX_ROW_INDEX = 4;
        CurrentlySelectedRowIndex = MATH::Number::Clamp(CurrentlySelectedRowIndex, UNSELECTED_ROW_INDEX, MAX_ROW_INDEX);

        constexpr int MAX_COLUMN_INDEX = 7;
        CurrentlySelectedColumnIndex = MATH::Number::Clamp(CurrentlySelectedColumnIndex, UNSELECTED_COLUMN_INDEX, MAX_COLUMN_INDEX);

        // DESELECT THE FOOD IF NO SELECTION CURRENTLY EXISTS.
        // Updating the selected food type is done during rendering for simplicity.
        // However, that code does not handle unselecting any food type, so that must be done here instead.
        bool food_selected = (CurrentlySelectedRowIndex != UNSELECTED_ROW_INDEX) && (CurrentlySelectedColumnIndex != UNSELECTED_COLUMN_INDEX);
        if (!food_selected)
        {
            SelectedFoodType = OBJECTS::Food::NONE;
        }
    }

    /// Renders the inventory GUI food page to the provided screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryFoodPage::Render(const Inventory& inventory, GRAPHICS::Renderer& renderer)
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        constexpr float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        constexpr float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        float background_height_in_pixels = renderer.Screen->HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            renderer.Screen->WidthInPixels<float>(),
            background_height_in_pixels);

        renderer.RenderScreenRectangle(
            background_rectangle,
            BACKGROUND_COLOR);

        // CALCULATE HOW MANY FOOD BOXES CAN APPEAR PER ROW AND COLUMN.
        // Some padding is added around the boxes in order to space them out.
        constexpr float BOX_DIMENSION_IN_PIXELS = 52.0f;
        constexpr float PADDING_BETWEEN_BOXES_IN_PIXELS = 10.0f;
        constexpr float BOX_WIDTH_PADDING_DIMENSION_IN_PIXELS = BOX_DIMENSION_IN_PIXELS + PADDING_BETWEEN_BOXES_IN_PIXELS;
        float page_width_in_pixels = background_rectangle.Width();
        unsigned int boxes_per_row = static_cast<unsigned int>(page_width_in_pixels / BOX_WIDTH_PADDING_DIMENSION_IN_PIXELS);

        // RENDER BOXES FOR EACH COLLECTED FOOD TYPE.
        const float BOX_HALF_DIMENSION_IN_PIXELS = BOX_DIMENSION_IN_PIXELS / 2.0f;
        // Some initial offset is added to help the food boxes appear centered.
        constexpr float INITIAL_LEFT_OFFSET_IN_PIXELS = 3.0f;
        constexpr float INITIAL_TOP_OFFSET_IN_PIXELS = 16.0f;
        float starting_box_center_x_position = background_rectangle.LeftTop.X + PADDING_BETWEEN_BOXES_IN_PIXELS + BOX_HALF_DIMENSION_IN_PIXELS + INITIAL_LEFT_OFFSET_IN_PIXELS;
        float starting_box_center_y_position = background_rectangle.LeftTop.Y + PADDING_BETWEEN_BOXES_IN_PIXELS + BOX_HALF_DIMENSION_IN_PIXELS + INITIAL_TOP_OFFSET_IN_PIXELS;
        const unsigned int FIRST_VALID_FOOD_ID = OBJECTS::Food::TypeId::NONE + 1;
        for (unsigned int food_id = FIRST_VALID_FOOD_ID; food_id < OBJECTS::Food::TypeId::COUNT; ++food_id)
        {
            // GET THE COLLECTED COUNT FOR THE FOOD TYPE.
            OBJECTS::Food::TypeId food_type = static_cast<OBJECTS::Food::TypeId>(food_id);
            unsigned int food_collected_count = inventory.FoodCounts[food_type];

            // CALCULATE THE BOX FOR THIS FOOD TYPE.
            // Since the first food enum isn't valid, the ID must be adjusted to
            // make things zero-based for calculating the box indices.
            unsigned int zero_based_food_id = food_id - 1;
            unsigned int box_row_index = zero_based_food_id / boxes_per_row;
            unsigned int box_column_index = zero_based_food_id % boxes_per_row;

            float current_box_x_offset = static_cast<float>(box_column_index * BOX_WIDTH_PADDING_DIMENSION_IN_PIXELS);
            float current_box_center_x_position = starting_box_center_x_position + current_box_x_offset;

            float current_box_y_offset = static_cast<float>(box_row_index * BOX_WIDTH_PADDING_DIMENSION_IN_PIXELS);
            float current_box_center_y_position = starting_box_center_y_position + current_box_y_offset;

            MATH::FloatRectangle box_screen_rectangle = MATH::FloatRectangle::FromCenterAndDimensions(
                current_box_center_x_position,
                current_box_center_y_position,
                BOX_DIMENSION_IN_PIXELS,
                BOX_DIMENSION_IN_PIXELS);

            // UPDATE THE SELECTED FOOD.
            // This is done here rather than in "update" for simplicity.
            bool current_food_box_is_selected = (
                (CurrentlySelectedRowIndex == static_cast<int>(box_row_index)) &&
                (CurrentlySelectedColumnIndex == static_cast<int>(box_column_index)));
            if (current_food_box_is_selected)
            {
                bool food_exists = (food_collected_count > 0);
                if (food_exists)
                {
                    SelectedFoodType = food_type;
                }
                else
                {
                    // The current box is selected, but no food exists within it.
                    SelectedFoodType = OBJECTS::Food::NONE;
                }
            }

            // RENDER A BOX FOR THE FOOD TYPE.
            RenderFoodBox(
                food_type,
                food_collected_count,
                current_food_box_is_selected,
                box_screen_rectangle,
                renderer);
        }
    }

    /// Renders a box for a type of food on the page.
    /// @param[in]  food_type - The type of food to render.
    /// @param[in]  food_type_collected_count - The count of food items collected of this type.
    /// @param[in]  is_selected - True if the food is selected; false otherwise.
    /// @param[in]  box_screen_rectangle - The placement/dimensions of the box to render, in screen coordinates.
    /// @param[in,out]  renderer - The renderer to use.
    void InventoryFoodPage::RenderFoodBox(
        const OBJECTS::Food::TypeId food_type,
        const unsigned int food_type_collected_count,
        const bool is_selected,
        const MATH::FloatRectangle& box_screen_rectangle,
        GRAPHICS::Renderer& renderer) const
    {
        // RENDER A SLIGHTLY DARKER RECTANGLE FOR THE BACKGROUND.
        GRAPHICS::Color food_box_background_color = BACKGROUND_COLOR;

        // The exact scale factor will depend on if the food is selected or not.
        if (is_selected)
        {
            const uint8_t SELECTED_BACKGROUND_COLOR_SCALE_FACTOR = 3;
            food_box_background_color.ScaleRgbDown(SELECTED_BACKGROUND_COLOR_SCALE_FACTOR);
        }
        else
        {
            const uint8_t UNSELECTED_BACKGROUND_COLOR_SCALE_FACTOR = 2;
            food_box_background_color.ScaleRgbDown(UNSELECTED_BACKGROUND_COLOR_SCALE_FACTOR);
        }

        renderer.RenderScreenRectangle(box_screen_rectangle, food_box_background_color);

        // RENDER INFORMATION ABOUT ANY COLLECTED FOOD OF THE TYPE.
        // No icon is rendered if the player hasn't collected any of the type of food
        // in order to keep food types hidden from players until they are discovered.
        bool food_type_collected = (food_type_collected_count > 0);
        if (food_type_collected)
        {
            // RENDER AN ICON FOR THE FOOD.
            // It should appear in the left-top corner of the box.
            std::shared_ptr<GRAPHICS::Sprite> food_sprite = RESOURCES::FoodGraphics::GetSprite(food_type);
            if (food_sprite)
            {
                MATH::Vector2f food_icon_left_top_screen_position = box_screen_rectangle.LeftTop;
                renderer.RenderGuiIcon(*food_sprite, food_icon_left_top_screen_position);

                // RENDER THE COLLECTED COUNT FOR THE FOOD TYPE.
                // It should appear just to the right of the food icon.
                std::string collected_count_text = "x" + std::to_string(food_type_collected_count);
                MATH::Vector2f collected_count_left_top_screen_position = food_icon_left_top_screen_position;
                collected_count_left_top_screen_position.X += food_sprite->GetWidthInPixels();
                renderer.RenderText(collected_count_text, RESOURCES::AssetId::FONT_TEXTURE, collected_count_left_top_screen_position);
            }
        }
    }
}
