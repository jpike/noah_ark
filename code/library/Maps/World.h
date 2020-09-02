#pragma once

#include <memory>
#include <vector>
#include "Maps/Ark.h"
#include "Maps/MultiTileMapGrid.h"
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

        // INITIALIZATION.
        static std::shared_ptr<World> Populate();
        void InitializeBuiltArkInOverworld(const std::vector<STATES::BuiltArkPieceTileMapData>& built_ark_pieces);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The overworld of the game.
        MultiTileMapGrid Overworld = MultiTileMapGrid(OVERWORLD_WIDTH_IN_TILE_MAPS, OVERWORLD_HEIGHT_IN_TILE_MAPS);
        /// The ark.
        Ark Ark = {};
        /// Noah (the player) character within the game.
        std::shared_ptr<OBJECTS::Noah> NoahPlayer = nullptr;

    private:
        // HELPER METHODS.
        void PopulateOverworld();
    };
}
