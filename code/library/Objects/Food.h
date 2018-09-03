#pragma once

#include <SFML/System.hpp>
#include "Graphics/Sprite.h"
#include "Math/Vector2.h"

namespace OBJECTS
{
    /// Enumerates all the different types of food.
    enum class FoodType
    {
        /// Indicates no food exists.
        NONE,
        ALMOND,
        APPLE,
        APRICOT,
        AVOCADO,
        BANANA,
        BLACKBERRY,
        BLUEBERRY,
        CANTELOUPE,
        CASHEW,
        CHERRY,
        CHESTNUT,
        CLEMENTINE,
        CLOUDBERRY,
        COCONUT,
        CRANBERRY,
        DURIAN,
        ELDERBERRY,
        FIG,
        GOURD,
        GRAPE,
        GRAPEFRUIT,
        GUAVA,
        HAZELNUT,
        LEMON,
        LIME,
        LYCHEE,
        MANGO,
        MELON,
        MULBERRY,
        OLIVE,
        ORANGE,
        PAWPAW,
        PEACH,
        PEAR,
        PECAN,
        PERSIMMON,
        PLUM,
        POMEGRANATE,
        RASPBERRY,
        WALNUT,
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

    /// An item of food that is falling off a plant toward the ground.
    /// This class helps manage updating food as it is falling.
    class FallingFood
    {
    public:
        // METHODS.
        void Fall(const sf::Time& time);
        bool FinishedFalling() const;

        // MEMBER VARIABLES.
        /// The food that is falling.
        Food FoodItem = Food();
        /// The world position that the food is falling toward.
        MATH::Vector2f DestinationWorldPosition = MATH::Vector2f();
    };
}
