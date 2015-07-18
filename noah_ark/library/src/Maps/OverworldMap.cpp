#include <memory>
#include "Maps/OverworldMap.h"
#include "Maps/TiledMapJsonFile.h"
#include "Maps/TileMapBuilder.h"
#include "Maps/TileMapLoader.h"

namespace MAPS
{
    OverworldMap::OverworldMap() :
        m_currentTileMap(),
        m_topTileMap(),
        m_bottomTileMap(),
        m_leftTileMap(),
        m_rightTileMap()
    {
        // Nothing else to do.
    }

    OverworldMap::~OverworldMap()
    {
        // Nothing else to do.
    }

    void OverworldMap::PopulateFromSpecification(
        const OverworldMapSpecification& overworld_spec,
        const OverworldGridPosition& starting_tile_map_overworld_grid_position,
        const MATH::Vector2f& starting_tile_map_top_left_world_position,
        const std::shared_ptr<TileMapBuilder>& tile_map_builder)
    {
        // CREATE THE STARTING TILE MAP.
        std::shared_ptr<TiledMapJsonFile> starting_map_file = std::make_shared<TiledMapJsonFile>();
        bool starting_map_loaded = starting_map_file->Load(overworld_spec.GetStartingTileMapFilepath());
        if (!starting_map_loaded)
        {
            /// @todo   Return false?
            return;
        }

        std::shared_ptr<TileMap> starting_tile_map = tile_map_builder->BuildTileMap(
            starting_tile_map_overworld_grid_position,
            starting_tile_map_top_left_world_position,
            starting_map_file);
        SetCurrentTileMap(starting_tile_map);

        // CREATE THE TOP TILE MAP, IF ONE EXISTS.
        // Determine the position of the top map in the overworld grid.
        MATH::Vector2ui top_tile_map_overworld_grid_position = starting_tile_map_overworld_grid_position;
        top_tile_map_overworld_grid_position.Y--;
        // These checks are needed to prevent an exception.
        bool top_tile_map_exists = overworld_spec.PositionInRange(
            top_tile_map_overworld_grid_position.Y,
            top_tile_map_overworld_grid_position.X);
        if (top_tile_map_exists)
        {
            // Get the filepath to the top tile map.
            std::string top_tile_map_filepath = overworld_spec.GetTileMapFilepath(
                top_tile_map_overworld_grid_position.Y,
                top_tile_map_overworld_grid_position.X);

            // Create the top tile map in the overworld.
            std::shared_ptr<TiledMapJsonFile> top_map_file = std::make_shared<TiledMapJsonFile>();
            bool top_map_loaded = top_map_file->Load(top_tile_map_filepath);
            if (!top_map_loaded)
            {
                /// @todo   Return false?
                return;
            }
            std::shared_ptr<MAPS::TileMap> top_tile_map = tile_map_builder->BuildTopTileMap(
                *starting_tile_map,
                top_map_file);
            SetTopTileMap(top_tile_map);
        }

        // CREATE THE BOTTOM TILE MAP, IF ONE EXISTS.
        // Determine the position of the bottom map in the overworld grid.
        MATH::Vector2ui bottom_tile_map_overworld_grid_position = starting_tile_map_overworld_grid_position;
        bottom_tile_map_overworld_grid_position.Y++;
        // These checks are needed to prevent an exception.
        bool bottom_tile_map_exists = overworld_spec.PositionInRange(
            bottom_tile_map_overworld_grid_position.Y,
            bottom_tile_map_overworld_grid_position.X);
        if (bottom_tile_map_exists)
        {
            // Get the filepath to the bottom tile map.
            std::string bottom_tile_map_filepath = overworld_spec.GetTileMapFilepath(
                bottom_tile_map_overworld_grid_position.Y,
                bottom_tile_map_overworld_grid_position.X);

            // Create the bottom tile map in the overworld.
            std::shared_ptr<TiledMapJsonFile> bottom_map_file = std::make_shared<TiledMapJsonFile>();
            bool bottom_map_loaded = bottom_map_file->Load(bottom_tile_map_filepath);
            if (!bottom_map_loaded)
            {
                /// @todo   Return false?
                return;
            }

            std::shared_ptr<MAPS::TileMap> bottom_tile_map = tile_map_builder->BuildBottomTileMap(
                *starting_tile_map,
                bottom_map_file);
            SetBottomTileMap(bottom_tile_map);
        }

        // CREATE THE LEFT TILE MAP, IF ONE EXISTS.
        // Determine the position of the left map in the overworld grid.
        MATH::Vector2ui left_tile_map_overworld_grid_position = starting_tile_map_overworld_grid_position;
        left_tile_map_overworld_grid_position.X--;
        // These checks are needed to prevent an exception.
        bool left_tile_map_exists = overworld_spec.PositionInRange(
            left_tile_map_overworld_grid_position.Y,
            left_tile_map_overworld_grid_position.X);
        if (left_tile_map_exists)
        {
            // Get the filepath to the left tile map.
            std::string left_tile_map_filepath = overworld_spec.GetTileMapFilepath(
                left_tile_map_overworld_grid_position.Y,
                left_tile_map_overworld_grid_position.X);

            // Create the left tile map in the overworld.
            std::shared_ptr<TiledMapJsonFile> left_map_file = std::make_shared<TiledMapJsonFile>();
            bool left_map_loaded = left_map_file->Load(left_tile_map_filepath);
            if (!left_map_loaded)
            {
                /// @todo   Return false?
                return;
            }

            std::shared_ptr<MAPS::TileMap> left_tile_map = tile_map_builder->BuildLeftTileMap(
                *starting_tile_map,
                left_map_file);
            SetLeftTileMap(left_tile_map);
        }

        // CREATE THE RIGHT TILE MAP, IF ONE EXISTS.
        // Determine the position of the right map in the overworld grid.
        MATH::Vector2ui right_tile_map_overworld_grid_position = starting_tile_map_overworld_grid_position;
        right_tile_map_overworld_grid_position.X++;
        // These checks are needed to prevent an exception.
        bool right_tile_map_exists = overworld_spec.PositionInRange(
            right_tile_map_overworld_grid_position.Y,
            right_tile_map_overworld_grid_position.X);
        if (right_tile_map_exists)
        {
            // Get the filepath to the right tile map.
            std::string right_tile_map_filepath = overworld_spec.GetTileMapFilepath(
                right_tile_map_overworld_grid_position.Y,
                right_tile_map_overworld_grid_position.X);

            // Create the right tile map in the overworld.
            std::shared_ptr<TiledMapJsonFile> right_map_file = std::make_shared<TiledMapJsonFile>();
            bool right_map_loaded = right_map_file->Load(right_tile_map_filepath);
            if (!right_map_loaded)
            {
                /// @todo   Return false?
                return;
            }

            std::shared_ptr<MAPS::TileMap> right_tile_map = tile_map_builder->BuildRightTileMap(
                *starting_tile_map,
                right_map_file);
            SetRightTileMap(right_tile_map);
        }
    }

    std::shared_ptr<Tile> OverworldMap::GetTileAtWorldPosition(const float worldXPosition, const float worldYPosition) const
    {
        // ATTEMPT TO RETRIEVE THE TILE FROM THE CURRENT CENTER TILE MAP.
        bool currentTileMapExists = (nullptr != m_currentTileMap);
        if (currentTileMapExists)
        {
            std::shared_ptr<Tile> requestedTile = m_currentTileMap->GetTileAtWorldPosition(worldXPosition, worldYPosition);
            bool requestedTileFound = (nullptr != requestedTile);
            if (requestedTileFound)
            {
                return requestedTile;
            }
        }

        // ATTEMPT TO RETRIEVE THE TILE FROM THE TOP TILE MAP.
        bool topTileMapExists = (nullptr != m_topTileMap);
        if (topTileMapExists)
        {
            std::shared_ptr<Tile> requestedTile = m_topTileMap->GetTileAtWorldPosition(worldXPosition, worldYPosition);
            bool requestedTileFound = (nullptr != requestedTile);
            if (requestedTileFound)
            {
                return requestedTile;
            }
        }

        // ATTEMPT TO RETRIEVE THE TILE FROM THE BOTTOM TILE MAP.
        bool bottomTileMapExists = (nullptr != m_bottomTileMap);
        if (bottomTileMapExists)
        {
            std::shared_ptr<Tile> requestedTile = m_bottomTileMap->GetTileAtWorldPosition(worldXPosition, worldYPosition);
            bool requestedTileFound = (nullptr != requestedTile);
            if (requestedTileFound)
            {
                return requestedTile;
            }
        }

        // ATTEMPT TO RETRIEVE THE TILE FROM THE LEFT TILE MAP.
        bool leftTileMapExists = (nullptr != m_leftTileMap);
        if (leftTileMapExists)
        {
            std::shared_ptr<Tile> requestedTile = m_leftTileMap->GetTileAtWorldPosition(worldXPosition, worldYPosition);
            bool requestedTileFound = (nullptr != requestedTile);
            if (requestedTileFound)
            {
                return requestedTile;
            }
        }

        // ATTEMPT TO RETRIEVE THE TILE FROM THE RIGHT TILE MAP.
        bool rightTileMapExists = (nullptr != m_rightTileMap);
        if (rightTileMapExists)
        {
            std::shared_ptr<Tile> requestedTile = m_rightTileMap->GetTileAtWorldPosition(worldXPosition, worldYPosition);
            bool requestedTileFound = (nullptr != requestedTile);
            if (requestedTileFound)
            {
                return requestedTile;
            }
        }

        // NO VALID TILE COULD BE FOUND.
        return nullptr;
    }

    std::vector< std::shared_ptr<OBJECTS::Tree> >* OverworldMap::GetTreesInArea(const MATH::Vector2f& world_position) const
    {
        /// @todo   Use a spatial hash.

        // GET TREES FROM THE CURRENT TILE MAP IF THE WORLD POSITION IS WITHIN THAT MAP.
        bool current_tile_map_exists = (nullptr != m_currentTileMap);
        if (current_tile_map_exists)
        {
            bool position_within_current_tile_map = m_currentTileMap->ContainsPosition(world_position);
            if (position_within_current_tile_map)
            {
                return m_currentTileMap->GetTrees();
            }
        }

        // GET TREES FROM THE TOP TILE MAP IF THE WORLD POSITION IS WITHIN THAT MAP.
        bool top_tile_map_exists = (nullptr != m_topTileMap);
        if (top_tile_map_exists)
        {
            bool position_within_top_tile_map = m_topTileMap->ContainsPosition(world_position);
            if (position_within_top_tile_map)
            {
                return m_topTileMap->GetTrees();
            }
        }

        // GET TREES FROM THE BOTTOM TILE MAP IF THE WORLD POSITION IS WITHIN THAT MAP.
        bool bottom_tile_map_exists = (nullptr != m_bottomTileMap);
        if (bottom_tile_map_exists)
        {
            bool position_within_bottom_tile_map = m_bottomTileMap->ContainsPosition(world_position);
            if (position_within_bottom_tile_map)
            {
                return m_bottomTileMap->GetTrees();
            }
        }

        // GET TREES FROM THE LEFT TILE MAP IF THE WORLD POSITION IS WITHIN THAT MAP.
        bool left_tile_map_exists = (nullptr != m_leftTileMap);
        if (left_tile_map_exists)
        {
            bool position_within_left_tile_map = m_leftTileMap->ContainsPosition(world_position);
            if (position_within_left_tile_map)
            {
                return m_leftTileMap->GetTrees();
            }
        }

        // GET TREES FROM THE RIGHT TILE MAP IF THE WORLD POSITION IS WITHIN THAT MAP.
        bool right_tile_map_exists = (nullptr != m_rightTileMap);
        if (right_tile_map_exists)
        {
            bool position_within_right_tile_map = m_rightTileMap->ContainsPosition(world_position);
            if (position_within_right_tile_map)
            {
                return m_rightTileMap->GetTrees();
            }
        }

        // No world area could be found for the specified position,
        // so indicate that no trees are available.
        return nullptr;
    }

    std::shared_ptr<TileMap> OverworldMap::GetCurrentTileMap() const
    {
        return m_currentTileMap;
    }

    std::shared_ptr<TileMap> OverworldMap::GetTopTileMap() const
    {
        return m_topTileMap;
    }

    std::shared_ptr<TileMap> OverworldMap::GetBottomTileMap() const
    {
        return m_bottomTileMap;
    }

    std::shared_ptr<TileMap> OverworldMap::GetLeftTileMap() const
    {
        return m_leftTileMap;
    }

    std::shared_ptr<TileMap> OverworldMap::GetRightTileMap() const
    {
        return m_rightTileMap;
    }

    void OverworldMap::SetCurrentTileMap(const std::shared_ptr<TileMap>& tileMap)
    {
        m_currentTileMap = tileMap;

        // The current tile map should always be visible.
        m_currentTileMap->SetVisible(true);
    }

    void OverworldMap::SetTopTileMap(const std::shared_ptr<TileMap>& tileMap)
    {
        m_topTileMap = tileMap;
    }

    void OverworldMap::SetBottomTileMap(const std::shared_ptr<TileMap>& tileMap)
    {
        m_bottomTileMap = tileMap;
    }

    void OverworldMap::SetLeftTileMap(const std::shared_ptr<TileMap>& tileMap)
    {
        m_leftTileMap = tileMap;
    }

    void OverworldMap::SetRightTileMap(const std::shared_ptr<TileMap>& tileMap)
    {
        m_rightTileMap = tileMap;
    }
}