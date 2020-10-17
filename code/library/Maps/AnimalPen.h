#pragma once

#include <limits>
#include "Objects/Animal.h"

namespace MAPS
{
    /// A pen within the ark for holding animals of a specific type.
    class AnimalPen
    {
    public:
        /// The species of animal in the pen.
        OBJECTS::AnimalSpecies Species = static_cast<OBJECTS::AnimalSpecies>(std::numeric_limits<int>::max());
        /// The animals currently in the pen.
        std::vector<std::shared_ptr<OBJECTS::Animal>> Animals = {};
    };
}
