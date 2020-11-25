#pragma once

#include <limits>
#include "Math/Rectangle.h"
#include "Memory/Pointers.h"
#include "Objects/Animal.h"

namespace MAPS
{
    /// A pen within the ark for holding animals of a specific type.
    class AnimalPen
    {
    public:
        /// The bounding box of the interior of the animal pen.
        /// This is stored over the exterior bounds because only the interior bounds
        /// are needed for bounding animal movement.
        MATH::FloatRectangle InteriorBoundingBox = MATH::FloatRectangle();
        /// The species of animal in the pen.
        OBJECTS::AnimalSpecies::Value Species = static_cast<OBJECTS::AnimalSpecies::Value>(std::numeric_limits<int>::max());
        /// The animals currently in the pen.
        std::vector<MEMORY::NonNullSharedPointer<OBJECTS::Animal>> Animals = {};
    };
}
