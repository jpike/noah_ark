#pragma once

#include <memory>
#include "Graphics/Sprite.h"
#include "Objects/Food.h"

namespace RESOURCES
{
    /// A specialized wrapper around resources containing logic
    /// for obtaining food graphics.
    class FoodGraphics
    {
    public:
        static std::shared_ptr<GRAPHICS::Sprite> GetSprite(const OBJECTS::Food::TypeId food_type);
    };
}
