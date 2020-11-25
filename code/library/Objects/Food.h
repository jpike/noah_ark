#pragma once

#include <SFML/System.hpp>
#include "Graphics/Sprite.h"
#include "Math/Vector2.h"

namespace OBJECTS
{
    /// An item of food.
    struct Food
    {
        // STATIC CONSTANTS.
        /// The amount of food in a given food item.
        /// Should match the visual representation of all food sprites.
        static constexpr unsigned int COUNT_PER_INSTANCE = 3;

        // ENUMERATIONS.
        /// Enumerates all the different types of food.
        /// A regular enum nested in a struct to provide scoping along with implicit conversion to integers.
        /// Has an "ID" suffix on the name to allow "Type" to be used as a regular member variable name.
        enum TypeId
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

        // MEMBER VARIABLES.
        /// The type of the food.
        TypeId Type = TypeId::NONE;
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
