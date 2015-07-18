#pragma once

#include <map>
#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Maps/OverworldMapSpecification.h"
#include "Maps/TileMap.h"
#include "Maps/TileMapBuilder.h"
#include "Math/Vector2.h"

/// Holds code related to maps in the game.
namespace MAPS
{
    ///////////////////////////////////////////////////////////
    /// @brief  Represents the entire overworld map, which is composed of many
    ///         individual tile maps (and objects on them) that are connected together.
    ///         This class is responsible for managing the connective relationship
    ///         between adjacent tile maps.
    ///////////////////////////////////////////////////////////
    class OverworldMap
    {
    public:
        /// @brief  Constructor.
        explicit OverworldMap();
        /// @brief  Destructor.
        ~OverworldMap();

        /// @brief          Populates the overworld map from the provided specification.
        ///                 Any existing data in this overworld map are overwritten.
        /// @param[in]      overworld_spec - The specification describing how tile maps
        ///                 are positioned in the overworld.
        /// @param[in]      starting_tile_map_overworld_grid_position - The relative position of
        ///                 the starting tile map within the overworld's "grid".
        /// @param[in]      starting_tile_map_top_left_world_position - The top-left world position
        ///                 of the starting tile map.
        /// @param[in,out]  tile_map_builder - The builder for creating tile maps described
        ///                 in the specification.
        void PopulateFromSpecification(
            const OverworldMapSpecification& overworld_spec,
            const OverworldGridPosition& starting_tile_map_overworld_grid_position,
            const MATH::Vector2f& starting_tile_map_top_left_world_position,
            const std::shared_ptr<TileMapBuilder>& tile_map_builder);
        
        /// @brief      Gets the tile at the specified world position.
        /// @param[in]  worldXPosition - The world X position at which to get the tile.
        /// @param[in]  worldYPosition - The world Y position at which to get the tile.
        /// @return     The tile at the specified world position, if one exists.
        std::shared_ptr<Tile> GetTileAtWorldPosition(const float worldXPosition, const float worldYPosition) const;

        /// Gets the trees within the sub-area of the world in which the provided position is located.
        /// @param[in]  world_position - The position identifying the sub-area for which to retrieve trees.
        /// @return A pointer to the trees within the area containing the provided world position;
        ///     null if no world sub-area exists for the world position.  The pointer is only valid
        ///     as long as the sub-area still exists within the world.
        std::vector< std::shared_ptr<OBJECTS::Tree> >* GetTreesInArea(const MATH::Vector2f& world_position) const;

        /// @brief  Gets the currently displayed tile map.
        /// @return The currently displayed tile map.
        std::shared_ptr<TileMap> GetCurrentTileMap() const;

        /// @brief  Gets the tile map above the currently displayed map.
        /// @return The tile map above the current one.
        std::shared_ptr<TileMap> GetTopTileMap() const;

        /// @brief  Gets the tile map below the currently displayed map.
        /// @return The tile map below the current one.
        std::shared_ptr<TileMap> GetBottomTileMap() const;

        /// @brief  Gets the tile map to the left the currently displayed map.
        /// @return The tile map left of the current one.
        std::shared_ptr<TileMap> GetLeftTileMap() const;

        /// @brief  Gets the tile map to the right the currently displayed map.
        /// @return The tile map right of the current one.
        std::shared_ptr<TileMap> GetRightTileMap() const;

        /// @brief      Sets the current tile map displayed by the overworld.
        /// @param[in]  tileMap - The new current tile map.
        void SetCurrentTileMap(const std::shared_ptr<TileMap>& tileMap);

        /// @brief      Sets the tile map above the map currently displayed by the overworld.
        /// @param[in]  tileMap - The new tile map above the currently displayed one.
        void SetTopTileMap(const std::shared_ptr<TileMap>& tileMap);

        /// @brief      Sets the tile map below the map currently displayed by the overworld.
        /// @param[in]  tileMap - The new tile map below the currently displayed one.
        void SetBottomTileMap(const std::shared_ptr<TileMap>& tileMap);

        /// @brief      Sets the tile map to the left of the map currently displayed by the overworld.
        /// @param[in]  tileMap - The new tile map to the left of the currently displayed one.
        void SetLeftTileMap(const std::shared_ptr<TileMap>& tileMap);

        /// @brief      Sets the tile map to the right of the map currently displayed by the overworld.
        /// @param[in]  tileMap - The new tile map to the right of the currently displayed one.
        void SetRightTileMap(const std::shared_ptr<TileMap>& tileMap);

    private:
        OverworldMap(const OverworldMap& mapToCopy);  ///< Private to disallow copying.
        OverworldMap& operator= (const OverworldMap& rhsMap); ///< Private to disallow assignment.

        std::shared_ptr<TileMap> m_currentTileMap;  ///< The current tile map containing the player.
        std::shared_ptr<TileMap> m_topTileMap;  ///< The tile map above the current tile map.
        std::shared_ptr<TileMap> m_bottomTileMap;   ///< The tile map below the current tile map.
        std::shared_ptr<TileMap> m_leftTileMap; ///< The tile map to the left of the current tile map.
        std::shared_ptr<TileMap> m_rightTileMap;    ///< The tile map to the right of the current tile map.
    };
}