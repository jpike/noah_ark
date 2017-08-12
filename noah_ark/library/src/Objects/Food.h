#pragma once

#include "Graphics/Sprite.h"

namespace OBJECTS
{
    /// Enumerates all the different types of food.
    enum class FoodType
    {
        /// Indicates no food exists.
        NONE,
        /// An apple.
        APPLE,
        /// A non-food value that supports determining the number of food types
        /// for algorithms that need this information.
        COUNT
    };

    /// An item of food.
    class Food
    {
    public:
        // STATIC CONSTANTS.
        /// The amount of food in a given food item.
        /// Should match the visual representation of all food sprites.
        static constexpr unsigned int COUNT_PER_INSTANCE = 3;

        // MEMBER VARIABLES.
        /// The type of the food.
        FoodType Type = FoodType::NONE;
        /// The graphical sprite for the food.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();
    };
}
