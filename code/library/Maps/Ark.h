#pragma once

#include <optional>
#include "Maps/LayeredMultiTileMapGrids.h"

namespace MAPS
{
    /// The ark as represented as a map in the world.
    class Ark
    {
    public:
        // STATIC CONSTANTS.
        /// The width of the ark interior in tile maps.
        static constexpr unsigned int INTERIOR_WIDTH_IN_TILE_MAPS = 3;
        /// The height of the ark interior in tile maps.
        static constexpr unsigned int INTERIOR_HEIGHT_IN_TILE_MAPS = 1;

        // POPULATION.
        void Populate();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The interior of the ark.
        LayeredMultiTileMapGrids Interior = LayeredMultiTileMapGrids();
    };
}
