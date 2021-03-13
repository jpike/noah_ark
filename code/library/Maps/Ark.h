#pragma once

#include <optional>
#include "Containers/NestedArray.h"
#include "Maps/LayeredMultiTileMapGrids.h"
#include "Maps/TileMap.h"
#include "Memory/Pointers.h"
#include "Objects/Animal.h"
#include "States/SavedGameData.h"

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
        /// The number of layers in the ark.
        static constexpr unsigned int LAYER_COUNT = 3;

        // CONSTRUCTION.
        explicit Ark(MEMORY::NonNullRawPointer<World> world);

        // INITIALIZATION.
        void ResetToInitialState(MEMORY::NonNullRawPointer<World> world);
        void InitializeAnimalPens(
            const CONTAINERS::NestedEnumArray<INVENTORY::AnimalCollectionStatistics, OBJECTS::AnimalSpecies, OBJECTS::AnimalGender>& collected_animals_by_species_then_gender);

        // OTHER PUBLIC METHODS.
        std::shared_ptr<TileMap> GetEntranceMap() const;
        void AddAnimalToPen(
            const MEMORY::NonNullSharedPointer<OBJECTS::Animal>& animal,
            STATES::SavedGameData& game_data);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The interior of the ark.
        LayeredMultiTileMapGrids Interior;
    };
}
