#pragma once

#include <optional>
#include "Maps/LayeredMultiTileMapGrids.h"
#include "Maps/TileMap.h"
#include "Memory/Pointers.h"

namespace MAPS
{
    // Forward declarations.
    class World;

    /// The ark as represented as a map in the world.
    class Ark
    {
    public:
        // STATIC CONSTANTS.
        /// The width of the ark interior in tile maps.
        static constexpr unsigned int INTERIOR_WIDTH_IN_TILE_MAPS = 3;
        /// The height of the ark interior in tile maps.
        static constexpr unsigned int INTERIOR_HEIGHT_IN_TILE_MAPS = 1;
        /// The index of the lowest map layer.
        static constexpr unsigned int LOWEST_LAYER_INDEX = 0;

        // CONSTRUCTION.
        explicit Ark(MEMORY::NonNullRawPointer<World> world);

        // PUBLIC METHODS.
        std::shared_ptr<TileMap> GetEntranceMap() const;
        void AddAnimalToPen(const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The interior of the ark.
        LayeredMultiTileMapGrids Interior;
    };
}
