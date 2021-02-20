#pragma once

#include <optional>
#include <vector>
#include <SFML/System.hpp>
#include "Audio/Speakers.h"
#include "Hardware/GamingHardware.h"
#include "Maps/AnimalPen.h"
#include "Maps/ExitPoint.h"
#include "Maps/GroundLayer.h"
#include "Maps/TileMapType.h"
#include "Math/Vector2.h"
#include "Memory/Pointers.h"
#include "Objects/Animal.h"
#include "Objects/ArkPiece.h"
#include "Objects/DustCloud.h"
#include "Objects/FlamingSword.h"
#include "Objects/Food.h"
#include "Objects/Tree.h"
#include "Objects/WoodLogs.h"
#include "States/SavedGameData.h"

namespace MAPS
{    
    // Forward declarations.
    class World;
    class MultiTileMapGrid;

    /// A single 2D map composed of individual tiles and any objects
    /// that may located on the map.
    class TileMap
    {
    public:
        // STATIC DIMENSION CONSTANTS.
        /// The width of a tile map, in tiles.
        static const unsigned int WIDTH_IN_TILES = 32;
        /// The height of a tile map, in tiles.
        static const unsigned int HEIGHT_IN_TILES = 24;

        // CONSTRUCTION.
        explicit TileMap(
            const TileMapType type,
            MEMORY::NonNullRawPointer<MultiTileMapGrid> map_grid,
            const unsigned int grid_row_index,
            const unsigned int grid_column_index,
            const MATH::Vector2f& center_world_position,
            const MATH::Vector2ui& dimensions_in_tiles);

        // POSITIONING/BOUNDARIES.
        MATH::Vector2f GetCenterWorldPosition() const;
        MATH::Vector2ui GetDimensionsInTiles() const;
        MATH::FloatRectangle GetWorldBoundingBox() const;

        // RETRIEVAL.
        std::shared_ptr<MAPS::Tile> GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const;
        OBJECTS::ArkPiece* GetArkPieceAtWorldPosition(const MATH::Vector2f& world_position);
        MAPS::ExitPoint* GetExitPointAtWorldPosition(const MATH::Vector2f& world_position);

        // UPDATING.
        void Update(
            const bool objects_can_move_across_space,
            STATES::SavedGameData& current_game_data,
            HARDWARE::GamingHardware& gaming_hardware);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The type of tile map.
        TileMapType Type;
        /// The map grid this tile map belongs to.
        MEMORY::NonNullRawPointer<MultiTileMapGrid> MapGrid;
        /// The 0-based index (from the top) of the tile map as located in a larger grid.
        unsigned int GridRowIndex;
        /// The 0-based index (from the left) of the tile map as located in a larger grid.
        unsigned int GridColumnIndex;
        /// The ground in this tile map.
        GroundLayer Ground;
        /// Trees in this tile map.
        std::vector<OBJECTS::Tree> Trees;
        /// Food that is falling off a plant toward the ground.
        /// After food falls off of a plant, it should be transferred here.
        std::vector<OBJECTS::FallingFood> FallingFood;
        /// Food in the tile map that has fallen to the groun.
        /// After food has finished falling, it should be transferred here.
        std::vector<OBJECTS::Food> FoodOnGround;
        /// Dust clouds within the tile map.
        std::vector<OBJECTS::DustCloud> DustClouds;
        /// Wooden logs that have appeared in the map from chopped down trees.
        std::vector<OBJECTS::WoodLogs> WoodLogs;
        /// Any flaming sword in the map.  Randomly appears/disappears.
        std::optional<OBJECTS::FlamingSword> FlamingSword;
        /// Pieces of the ark that may exist on this tile map.
        /// Stored in a vector currently for simplicitly at the moment,
        /// but a different data structure may be needed later for performance.
        std::vector<OBJECTS::ArkPiece> ArkPieces;
        /// Animals freely roaming in the tile map that haven't yet been collected by the player.
        std::vector<MEMORY::NonNullSharedPointer<OBJECTS::Animal>> RoamingAnimals;
        /// Animal pens (only in the ark).
        std::vector<MAPS::AnimalPen> AnimalPens;
        /// Exit points within the tile map.
        std::vector<MAPS::ExitPoint> ExitPoints;

    private:
        // UPDATING HELPER METHODS.
        void MoveAnimals(STATES::SavedGameData& current_game_data, HARDWARE::GamingHardware& gaming_hardware);
    };
}