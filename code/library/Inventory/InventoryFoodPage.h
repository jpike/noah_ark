#pragma once

#include <memory>
#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
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

        // CONSTRUCTION.
        explicit InventoryFoodPage(const std::shared_ptr<const Inventory>& inventory);

        // RENDERING.
        void Render(GRAPHICS::Renderer& renderer) const;

    private:
        // RENDERING.
        void RenderFoodBox(
            const OBJECTS::FoodType food_type,
            const unsigned int food_type_collected_count,
            const MATH::FloatRectangle& box_screen_rectangle,
            GRAPHICS::Renderer& renderer) const;

        // MEMBER VARIABLES.
        /// The inventory displayed on the page.
        std::shared_ptr<const Inventory> Inventory;
    };
}
