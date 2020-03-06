#pragma once

#include <memory>
#include "Graphics/AnimatedSprite.h"
#include "Objects/Animal.h"
#include "Resources/Assets.h"

namespace RESOURCES
{
    /// A specialized wrapper around resources containing logic
    /// for obtaining animal graphics.
    class AnimalGraphics
    {
    public:
        /// The width of an animal sprite, in pixels.
        static constexpr float SPRITE_WIDTH_IN_PIXELS = 16.0f;

        static std::shared_ptr<GRAPHICS::AnimatedSprite> GetSprite(
            const OBJECTS::AnimalType animal_type, 
            Assets& assets);

    private:
        static std::shared_ptr<GRAPHICS::AnimationSequence> GetAnimalAnimation(const OBJECTS::AnimalType& animal_type);
    };
}
