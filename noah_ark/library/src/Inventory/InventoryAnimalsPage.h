#pragma once

#include <memory>
#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Inventory/Inventory.h"
#include "Math/Rectangle.h"
#include "Objects/Animal.h"
#include "Resources/Assets.h"

namespace INVENTORY
{
    /// The page of the inventory GUI that displays collected animals.
    /// Collected animals are displayed in a grid-like fashion
    /// with icons and collected counts per species/gender.
    class InventoryAnimalsPage
    {
    public:
        // STATIC CONSTANTS.
        /// The background color of the Bible page.
        static const GRAPHICS::Color BACKGROUND_COLOR;

        // CONSTRUCTION.
        explicit InventoryAnimalsPage(
            const std::shared_ptr<const Inventory>& inventory,
            const std::shared_ptr<RESOURCES::Assets>& assets);

        // RENDERING.
        void Render(GRAPHICS::Renderer& renderer) const;

    private:
        // RENDERING.
        void RenderAnimalBox(
            const OBJECTS::AnimalSpecies species,
            const unsigned int species_male_animal_collected_count,
            const unsigned int species_female_animal_collected_count,
            const MATH::FloatRectangle& box_screen_rectangle,
            GRAPHICS::Renderer& renderer) const;

        // MEMBER VARIABLES.
        /// The inventory displayed on the page.
        std::shared_ptr<const Inventory> Inventory;
        /// The assets available for use on the page.
        std::shared_ptr<RESOURCES::Assets> Assets;
    };
}
