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
        static std::shared_ptr<GRAPHICS::AnimatedSprite> GetSprite(
            const OBJECTS::AnimalType animal_type, 
            Assets& assets);

    private:
        static std::shared_ptr<GRAPHICS::AnimationSequence> GetAnimalAnimation(const OBJECTS::AnimalType& animal_type);
    };
}
