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
        /// The row of the map with the ark entrance.
        static constexpr unsigned int ARK_ENTRANCE_TILE_MAP_ROW = 0;
        /// The column of the map with the ark entrance.
        static constexpr unsigned int ARK_ENTRANCE_TILE_MAP_COLUMN = 2;

        // CONSTRUCTION.
        explicit Overworld(MEMORY::NonNullRawPointer<World> world);

        // OTHER PUBLIC METHODS.
        void ResetToInitialState();
        void SetArkPiecesBuiltStatus(const bool built);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The tile map grid for the overworld.
        MultiTileMapGrid MapGrid;
        /// Animals being transferred from following Noah into the ark.
        std::vector<MEMORY::NonNullSharedPointer<OBJECTS::Animal>> AnimalsGoingIntoArk;
    };
}