#pragma once

#include "Maps/MultiTileMapGrid.h"
#include "Maps/OverworldMapData.h"

namespace MAPS
{
    /// The entire game world.
    class World
    {
    public:
        // STATIC CONSTANTS.
        /// The width of the overworld in tile maps.
        static constexpr unsigned int OVERWORLD_WIDTH_IN_TILE_MAPS = 7;
        /// The height of the overworld in tile maps.
        static constexpr unsigned int OVERWORLD_HEIGHT_IN_TILE_MAPS = 7;
        /// The width of the ark interior in tile maps.
        static constexpr unsigned int ARK_INTERIOR_WIDTH_IN_TILE_MAPS = 1;
        /// The height of the ark interior in tile maps.
        static constexpr unsigned int ARK_INTERIOR_HEIGHT_IN_TILE_MAPS = 1;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The overworld of the game.
        MultiTileMapGrid Overworld = MultiTileMapGrid(OVERWORLD_WIDTH_IN_TILE_MAPS, OVERWORLD_HEIGHT_IN_TILE_MAPS);
        /// The interior of the ark.
        MultiTileMapGrid ArkInterior = MultiTileMapGrid(ARK_INTERIOR_WIDTH_IN_TILE_MAPS, ARK_INTERIOR_HEIGHT_IN_TILE_MAPS);
    };
}
