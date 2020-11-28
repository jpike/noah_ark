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
        /// The layer index of the tile map containing the pen.
        unsigned int TileMapLayerIndex = 0;
        /// The column index of the tile map containing the pen.
        unsigned int TileMapColumnIndex = 0;
        /// The row index of the tile map containing the pen.
        unsigned int TileMapRowIndex = 0;
        /// The index of the animal pen within the tile map.
        unsigned int AnimalPenIndex = 0;
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
