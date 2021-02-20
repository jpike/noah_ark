#pragma once

#include <SFML/System.hpp>
#include "Graphics/Sprite.h"

namespace OBJECTS
{
    /// The flaming sword set to guard the Garden of Eden/tree of life at the end of Genesis 3.
    /// Randomly appears and then vanishes in the game as a bit of a mysterious Easter egg.
    class FlamingSword
    {
    public:
        // CONSTRUCTION.
        explicit FlamingSword(const float world_x_position, const float world_y_position);

        // OTHER PUBLIC METHODS.
        void Update(const sf::Time& elasped_time);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the sword.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();
        /// The total elapsed time the sword has been visible.
        sf::Time TotalElapsedTime = sf::Time::Zero;
    };
}
