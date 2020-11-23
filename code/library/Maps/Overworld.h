#pragma once

#include <vector>
#include "Maps/MultiTileMapGrid.h"
#include "Memory/Pointers.h"
#include "Objects/Animal.h"

namespace MAPS
{
    // Forward declarations.
    class World;

    /// The overworld of the game.
    class Overworld
    {
    public:
        // STATIC CONSTANTS.
        /// The width of the overworld in tile maps.
        static constexpr unsigned int WIDTH_IN_TILE_MAPS = 7;
        /// The height of the overworld in tile maps.
        static constexpr unsigned int HEIGHT_IN_TILE_MAPS = 7;

        // CONSTRUCTION.
        explicit Overworld(MEMORY::NonNullRawPointer<World> world);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The tile map grid for the overworld.
        MultiTileMapGrid MapGrid;
        /// Animals being transferred from following Noah into the ark.
        std::vector<MEMORY::NonNullSharedPointer<OBJECTS::Animal>> AnimalsGoingIntoArk;
    };
}