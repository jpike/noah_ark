#pragma once

#include <memory>
#include <SFML/System.hpp>
#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
#include "Inventory/Inventory.h"
#include "Math/Rectangle.h"
#include "Objects/Food.h"

namespace INVENTORY
{
    /// The page of the inventory GUI that displays collected food.
    /// Collected food are displayed in a grid-like fashion
    /// with icons and collected counts.
    class InventoryFoodPage
    {
    public:
        // STATIC CONSTANTS.
        /// The background color of the food page.
        static const GRAPHICS::Color BACKGROUND_COLOR;
        /// The index for an unselected row.
        static constexpr int UNSELECTED_ROW_INDEX = -1;
        /// The index for an unselected column.
        static constexpr int UNSELECTED_COLUMN_INDEX = -1;

        // UPDATING.
        void Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller);

        // RENDERING.
        void Render(const Inventory& inventory, GRAPHICS::Renderer& renderer);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The index of the row of food currently selected.
        int CurrentlySelectedRowIndex = UNSELECTED_ROW_INDEX;
        /// The index of the column of food currently selected.
        int CurrentlySelectedColumnIndex = UNSELECTED_COLUMN_INDEX;
        /// The type of the currently selected food.
        OBJECTS::Food::TypeId SelectedFoodType = OBJECTS::Food::NONE;

    private:
        // RENDERING.
        void RenderFoodBox(
            const OBJECTS::Food::TypeId food_type,
            const unsigned int food_type_collected_count,
            const bool is_selected,
            const MATH::FloatRectangle& box_screen_rectangle,
            GRAPHICS::Renderer& renderer) const;
    };
}
