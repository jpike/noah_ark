#pragma once

#include <vector>
#include "Maps/Ark.h"
#include "Maps/MultiTileMapGrid.h"
#include "Memory/Pointers.h"
#include "Objects/Noah.h"
#include "States/BuiltArkPieceTileMapData.h"

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

        // CONSTRUCTION/INITIALIZATION.
        static MEMORY::NonNullSharedPointer<World> CreateInitial();
        void InitializeBuiltArkInOverworld(const std::vector<STATES::BuiltArkPieceTileMapData>& built_ark_pieces);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The overworld of the game.
        MultiTileMapGrid Overworld = MultiTileMapGrid(OVERWORLD_WIDTH_IN_TILE_MAPS, OVERWORLD_HEIGHT_IN_TILE_MAPS);
        /// The ark.
        Ark Ark = {};
        /// Noah (the player) character within the game.
        OBJECTS::Noah NoahPlayer = OBJECTS::Noah();

    private:
        // HELPER METHODS.
        void PopulateOverworld();
    };
}
