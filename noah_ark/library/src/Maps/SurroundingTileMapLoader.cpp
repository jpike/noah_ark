#include <functional>
#include "Maps/SurroundingTileMapLoader.h"

namespace MAPS
{
    SurroundingTileMapLoader::SurroundingTileMapLoader(const OverworldMapSpecification* const overworld_map_spec) :
    OverworldMapSpec(overworld_map_spec),
    MapLoader(),
    TopMap(),
    BottomMap(),
    LeftMap(),
    RightMap(),
    TopMapLoader(),
    BottomMapLoader(),
    LeftMapLoader(),
    RightMapLoader()
    {}

    SurroundingTileMapLoader::~SurroundingTileMapLoader()
    {
        // Nullify the pointer to avoid accidental memory corruption later.
        // Freeing the memory is a responsibility external to this class.
        OverworldMapSpec = nullptr;
    }

    void SurroundingTileMapLoader::StartLoadingTopTileMap(const MATH::Vector2ui& center_tile_map_overworld_grid_position)
    {
        // RESET THE TOP TILE MAP.
        TopMap.reset();

        // CHECK IF A TOP MAP EXISTS.
        // Determine the position of the top map in the overworld grid.
        MATH::Vector2ui top_tile_map_overworld_grid_position = center_tile_map_overworld_grid_position;
        top_tile_map_overworld_grid_position.Y--;
        // These checks are needed to prevent an exception.
        bool top_tile_map_exists = OverworldMapSpec->PositionInRange(
            top_tile_map_overworld_grid_position.Y,
            top_tile_map_overworld_grid_position.X);
        if (top_tile_map_exists)
        {
            // Get the filepath to the top tile map.
            std::string top_tile_map_filepath = OverworldMapSpec->GetTileMapFilepath(
                top_tile_map_overworld_grid_position.Y,
                top_tile_map_overworld_grid_position.X);

            // Launch a separate thread for loading the top map.  A separate thread isn't forced
            // to avoid exceptions on systems where additional threads may not be available.
            auto top_tile_map_loading_function = std::bind(&MAPS::TileMapLoader::LoadMap, &MapLoader, top_tile_map_filepath);
            TopMapLoader = std::async(top_tile_map_loading_function);
        }
    }

    void SurroundingTileMapLoader::StartLoadingBottomTileMap(const MATH::Vector2ui& center_tile_map_overworld_grid_position)
    {
        // RESET THE BOTTOM TILE MAP.
        BottomMap.reset();

        // CHECK IF A BOTTOM MAP EXISTS.
        // Determine the position of the bottom map in the overworld grid.
        MATH::Vector2ui bottom_tile_map_overworld_grid_position = center_tile_map_overworld_grid_position;
        bottom_tile_map_overworld_grid_position.Y++;
        // These checks are needed to prevent an exception.
        bool bottom_tile_map_exists = OverworldMapSpec->PositionInRange(
            bottom_tile_map_overworld_grid_position.Y,
            bottom_tile_map_overworld_grid_position.X);
        if (bottom_tile_map_exists)
        {
            // Get the filepath to the bottom tile map.
            std::string bottom_tile_map_filepath = OverworldMapSpec->GetTileMapFilepath(
                bottom_tile_map_overworld_grid_position.Y,
                bottom_tile_map_overworld_grid_position.X);

            // Launch a separate thread for loading the bottom map.  A separate thread isn't forced
            // to avoid exceptions on systems where additional threads may not be available.
            auto bottom_tile_map_loading_function = std::bind(&MAPS::TileMapLoader::LoadMap, &MapLoader, bottom_tile_map_filepath);
            BottomMapLoader = std::async(bottom_tile_map_loading_function);
        }
    }

    void SurroundingTileMapLoader::StartLoadingLeftTileMap(const MATH::Vector2ui& center_tile_map_overworld_grid_position)
    {
        // RESET THE LEFT TILE MAP.
        LeftMap.reset();

        // CHECK IF A LEFT MAP EXISTS.
        // Determine the position of the left map in the overworld grid.
        MATH::Vector2ui left_tile_map_overworld_grid_position = center_tile_map_overworld_grid_position;
        left_tile_map_overworld_grid_position.X--;
        // These checks are needed to prevent an exception.
        bool left_tile_map_exists = OverworldMapSpec->PositionInRange(
            left_tile_map_overworld_grid_position.Y,
            left_tile_map_overworld_grid_position.X);
        if (left_tile_map_exists)
        {
            // Get the filepath to the left tile map.
            std::string left_tile_map_filepath = OverworldMapSpec->GetTileMapFilepath(
                left_tile_map_overworld_grid_position.Y,
                left_tile_map_overworld_grid_position.X);

            // Launch a separate thread for loading the left map.  A separate thread isn't forced
            // to avoid exceptions on systems where additional threads may not be available.
            auto left_tile_map_loading_function = std::bind(&MAPS::TileMapLoader::LoadMap, &MapLoader, left_tile_map_filepath);
            LeftMapLoader = std::async(left_tile_map_loading_function);
        }
    }

    void SurroundingTileMapLoader::StartLoadingRightTileMap(const MATH::Vector2ui& center_tile_map_overworld_grid_position)
    {
        // RESET THE RIGHT TILE MAP.
        RightMap.reset();

        // CHECK IF A RIGHT MAP EXISTS.
        // Determine the position of the right map in the overworld grid.
        MATH::Vector2ui right_tile_map_overworld_grid_position = center_tile_map_overworld_grid_position;
        right_tile_map_overworld_grid_position.X++;
        // These checks are needed to prevent an exception.
        bool right_tile_map_exists = OverworldMapSpec->PositionInRange(
            right_tile_map_overworld_grid_position.Y,
            right_tile_map_overworld_grid_position.X);
        if (right_tile_map_exists)
        {
            // Get the filepath to the right tile map.
            std::string right_tile_map_filepath = OverworldMapSpec->GetTileMapFilepath(
                right_tile_map_overworld_grid_position.Y,
                right_tile_map_overworld_grid_position.X);

            // Launch a separate thread for loading the right map.  A separate thread isn't forced
            // to avoid exceptions on systems where additional threads may not be available.
            auto right_tile_map_loading_function = std::bind(&MAPS::TileMapLoader::LoadMap, &MapLoader, right_tile_map_filepath);
            RightMapLoader = std::async(right_tile_map_loading_function);
        }
    }

    std::shared_ptr<TiledMapJsonFile> SurroundingTileMapLoader::GetTopTileMap()
    {
        // CATCH ANY EXCEPTIONS THAT MAY OCCUR.
        try
        {
            // Verify that the future for the top TMX map is valid.
            if (TopMapLoader.valid())
            {
                // Return the loaded map.
                TopMap = TopMapLoader.get();
                return TopMap;
            }
            else
            {
                // Return the previously loaded map (if one exists).
                return TopMap;
            }
        }
        catch (const std::exception&)
        {
            // No map could be loaded.
            return nullptr;
        }
    }

    std::shared_ptr<TiledMapJsonFile> SurroundingTileMapLoader::GetBottomTileMap()
    {
        // CATCH ANY EXCEPTIONS THAT MAY OCCUR.
        try
        {
            // Verify that the future for the bottom TMX map is valid.
            if (BottomMapLoader.valid())
            {
                // Return the loaded map.
                BottomMap = BottomMapLoader.get();
                return BottomMap;
            }
            else
            {
                // Return the previously loaded map (if one exists).
                return BottomMap;
            }
        }
        catch (const std::exception&)
        {
            // No map could be loaded.
            return nullptr;
        }
    }

    std::shared_ptr<TiledMapJsonFile> SurroundingTileMapLoader::GetLeftTileMap()
    {
        // CATCH ANY EXCEPTIONS THAT MAY OCCUR.
        try
        {
            // Verify that the future for the left TMX map is valid.
            if (LeftMapLoader.valid())
            {
                // Return the loaded map.
                LeftMap = LeftMapLoader.get();
                return LeftMap;
            }
            else
            {
                // Return the previously loaded map (if one exists).
                return LeftMap;
            }
        }
        catch (const std::exception&)
        {
            // No map could be loaded.
            return nullptr;
        }
    }

    std::shared_ptr<TiledMapJsonFile> SurroundingTileMapLoader::GetRightTileMap()
    {
        // CATCH ANY EXCEPTIONS THAT MAY OCCUR.
        try
        {
            // Verify that the future for the right TMX map is valid.
            if (RightMapLoader.valid())
            {
                // Return the loaded map.
                RightMap = RightMapLoader.get();
                return RightMap;
            }
            else
            {
                // Return the previously loaded map (if one exists).
                return RightMap;
            }
        }
        catch (const std::exception&)
        {
            // No map could be loaded.
            return nullptr;
        }
    }
}