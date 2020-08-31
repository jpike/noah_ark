#pragma once

#include <memory>
#include "Maps/Ark.h"
#include "Maps/MultiTileMapGrid.h"
#include "Objects/Noah.h"
#include "Resources/Assets.h"

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

        // CONSTRUCTION.
        static std::shared_ptr<World> Populate(RESOURCES::Assets& assets);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The overworld of the game.
        MultiTileMapGrid Overworld = MultiTileMapGrid(OVERWORLD_WIDTH_IN_TILE_MAPS, OVERWORLD_HEIGHT_IN_TILE_MAPS);
        /// The ark.
        Ark Ark = {};
        /// Noah (the player) character within the game.
        OBJECTS::Noah NoahPlayer = {};

    private:
        // HELPER METHODS.
        void PopulateOverworld(RESOURCES::Assets& assets);
    };
}
