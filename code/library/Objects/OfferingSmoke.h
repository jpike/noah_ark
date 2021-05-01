#pragma once

#include <SFML/System.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Math/Vector2.h"

namespace OBJECTS
{
    /// The smoke from the burnt offerings that Noah makes toward the end of Genesis 8.
    class OfferingSmoke
    {
    public:
        // CONSTRUCTION.
        explicit OfferingSmoke(const MATH::Vector2f& altar_center_world_position);

        // OTHER PUBLIC METHODS.
        void Update(const sf::Time& elapsed_time);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the smoke.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The total time the smoke has been in existence.  Increased during each update call.
        float TotalElapsedTimeInSeconds = 0.0f;
    };
}
