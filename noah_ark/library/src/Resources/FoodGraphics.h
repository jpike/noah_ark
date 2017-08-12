#pragma once

#include <memory>
#include "Graphics/Sprite.h"
#include "Objects/Food.h"
#include "Resources/Assets.h"

namespace RESOURCES
{
    /// A specialized wrapper around resources containing logic
    /// for obtaining food graphics.
    class FoodGraphics
    {
    public:
        static std::shared_ptr<GRAPHICS::Sprite> GetSprite(
            const OBJECTS::FoodType food_type,
            Assets& assets);
    };
}
