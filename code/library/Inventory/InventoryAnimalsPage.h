#pragma once

#include <memory>
#include <SFML/System.hpp>
#include "Graphics/Color.h"
#include "Graphics/Renderer.h"
#include "Input/InputController.h"
#include "Inventory/Inventory.h"
#include "Math/Rectangle.h"
#include "Objects/Animal.h"
#include "States/SavedGameData.h"

namespace INVENTORY
{
    /// The page of the inventory GUI that displays collected animals.
    /// Collected animals are displayed in a grid-like fashion
    /// with icons and collected counts per species/gender.
    class InventoryAnimalsPage
    {
    public:
        // STATIC CONSTANTS.
        /// The background color of the animals page.
        static const GRAPHICS::Color BACKGROUND_COLOR;

        // UPDATING.
        void Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller);

        // RENDERING.
        void Render(const STATES::SavedGameData& current_game_data, GRAPHICS::Renderer& renderer) const;

    private:
        // ANIMAL SELECTION.
        void SelectPreviousAnimalSpecies();
        void SelectNextAnimalSpecies();

        // MEMBER VARIABLES.
        /// The index of the currently selected animal species in the global list.
        std::size_t SelectedAnimalSpeciesIndex = 0;
        /// The elapsed time a scroll key has been held down before switching to a different animal.
        /// Used for smoother scrolling through the animals on this page.
        sf::Time ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals = sf::Time::Zero;
    };
}
