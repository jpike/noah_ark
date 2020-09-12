#include "Maps/TileMap.h"

namespace MAPS
{
    /// Creates an empty tile map.  Parameters have default values to allow
    /// for default construction.
    /// @param[in]  grid_row_index - The 0-based index (from the top) of
    ///     the tile map as located in its grid.
    /// @param[in]  grid_column_index - The 0-based index (from the left) of
    ///     the tile map as located in its grid.
    /// @param[in]  center_world_position - The world position of the center
    ///     of the tile map.
    /// @param[in]  dimensions_in_tiles - The dimensions of the map in tiles.
    /// @param[in]  tile_dimension_in_pixels - The dimensions (both width and height)
    ///     of an individual tile in the map (in units of pixels).
    TileMap::TileMap(
        const unsigned int grid_row_index,
        const unsigned int grid_column_index,
        const MATH::Vector2f& center_world_position,
        const MATH::Vector2ui& dimensions_in_tiles,
        const unsigned int tile_dimension_in_pixels) :
    GridRowIndex(grid_row_index),
    GridColumnIndex(grid_column_index),
    Ground(center_world_position, dimensions_in_tiles, tile_dimension_in_pixels),
    Trees(),
    FallingFood(),
    FoodOnGround(),
    DustClouds(),
    WoodLogs(),
    ArkPieces()
    {}

    /// Gets the center world position of the tile map.
    /// @return The center world position of the tile map.
    MATH::Vector2f TileMap::GetCenterWorldPosition() const
    {
        return Ground.CenterWorldPosition;
    }

    /// Gets the dimensions (width, height) of the tile map, in units of tiles.
    /// @return The dimensions of the tile map, in tiles.
    MATH::Vector2ui TileMap::GetDimensionsInTiles() const
    {
        MATH::Vector2ui dimensions_in_tiles(
            Ground.Tiles.GetWidth(),
            Ground.Tiles.GetHeight());

        return dimensions_in_tiles;
    }

    /// Gets the bounding box of the tile map in world coordinates.
    /// @return The world bounding box of the tile map.
    MATH::FloatRectangle TileMap::GetWorldBoundingBox() const
    {
        // GET THE CENTER OF THE TILE MAP.
        MATH::Vector2f center_world_position = GetCenterWorldPosition();

        // GET THE FULL DIMENSIONS OF THE TILE MAP.
        MATH::Vector2ui dimensions_in_tiles = GetDimensionsInTiles();
        float width_in_pixels = static_cast<float>(dimensions_in_tiles.X * Ground.TileDimensionInPixels);
        float height_in_pixels = static_cast<float>(dimensions_in_tiles.Y * Ground.TileDimensionInPixels);

        // FORM THE WORLD BOUNDING BOX.
        MATH::FloatRectangle world_bounding_box = MATH::FloatRectangle::FromCenterAndDimensions(
            center_world_position.X,
            center_world_position.Y,
            width_in_pixels,
            height_in_pixels);
        return world_bounding_box;
    }

    /// Gets the tile that includes the specified world coordinates.
    /// @param[in]  world_x_position - The world x position of the tile to retrieve.
    /// @param[in]  world_y_position - The world y position of the tile to retrieve.
    /// @return The tile at the specified location, if one exists; null otherwise.
    std::shared_ptr<MAPS::Tile> TileMap::GetTileAtWorldPosition(const float world_x_position, const float world_y_position) const
    {
        std::shared_ptr<MAPS::Tile> tile = Ground.GetTileAtWorldPosition(world_x_position, world_y_position);
        return tile;
    }

    /// Gets an ark piece at the specified world position, if one exists.
    /// @param[in]  world_position - The world position for which to retrieve an ark piece.
    /// @return The ark piece at the specified world position, if one exists; null otherwise.
    OBJECTS::ArkPiece* TileMap::GetArkPieceAtWorldPosition(const MATH::Vector2f& world_position)
    {
        // SEARCH FOR AN ARK PIECE THAT CONTAINS THE SPECIFIED WORLD POSITION.
        for (auto& ark_piece : ArkPieces)
        {
            // CHECK IF THE CURRENT ARK PIECE INCLUDES THE WORLD POSITION.
            MATH::FloatRectangle ark_piece_bounding_box = ark_piece.Sprite.GetWorldBoundingBox();
            bool ark_piece_contains_world_position = ark_piece_bounding_box.Contains(world_position.X, world_position.Y);
            if (ark_piece_contains_world_position)
            {
                // RETURN THE ARK PIECE.
                return &ark_piece;
            }
        }

        // INDICATE THAT NO ARK PIECE COULD BE FOUND.
        return nullptr;
    }

    /// Gets an exit point at the specified world position, if one exists.
    /// @param[in]  world_position - The world position for which to retrieve an exit point.
    /// @return The exit point at the specified world position, if one exists; null otherwise.
    MAPS::ExitPoint* TileMap::GetExitPointAtWorldPosition(const MATH::Vector2f& world_position)
    {
        // SEARCH FOR AN EXIT POINT THAT CONTAINS THE SPECIFIED WORLD POSITION.
        for (auto& exit_point : ExitPoints)
        {
            // CHECK IF THE CURRENT EXIT POINT INCLUDES THE WORLD POSITION.
            bool exit_point_contains_world_position = exit_point.BoundingBox.Contains(world_position.X, world_position.Y);
            if (exit_point_contains_world_position)
            {
                // CHECK IF THE EXIT POINT IS PART OF AN ARK PIECE.
                OBJECTS::ArkPiece* ark_piece_for_exit = GetArkPieceAtWorldPosition(world_position);
                if (ark_piece_for_exit)
                {
                    // MAKE SURE THE ARK PIECE HAS BEEN BUILT AND IS FOR A DOORWAY.
                    // If the ark piece hasn't been built yet or isn't a doorway,
                    // then the exit point shouldn't be usable yet.
                    bool ark_piece_is_built_external_doorway = (ark_piece_for_exit->Built && ark_piece_for_exit->IsExternalDoorway);
                    if (ark_piece_is_built_external_doorway)
                    {
                        // RETURN THE USABLE EXIT POINT.
                        return &exit_point;
                    }
                }
                else
                {
                    // RETURN THE EXIT POINT.
                    return &exit_point;
                }
            }
        }

        // INDICATE THAT NO EXIT POINT COULD BE FOUND.
        return nullptr;
    }

    /// Updates the tile map based on elapsed time, simulating interactions within the map.
    /// @param[in]  elapsed_time - The elapsed time for which to update the tile map.
    /// @param[in,out]  speakers - The speakers out of which to play any audio.
    void TileMap::Update(const sf::Time& elapsed_time, AUDIO::Speakers& speakers)
    {
        // UPDATE THE CURRENT TILE MAP'S TILES.
        unsigned int map_height_in_tiles = Ground.Tiles.GetHeight();
        unsigned int map_width_in_tiles = Ground.Tiles.GetWidth();
        for (unsigned int tile_y = 0; tile_y < map_height_in_tiles; ++tile_y)
        {
            // UPDATE TILES ACROSS THE CURRENT ROW.
            for (unsigned int tile_x = 0; tile_x < map_width_in_tiles; ++tile_x)
            {
                // UPDATE THE CURRENT TILE.
                auto current_tile = Ground.Tiles(tile_x, tile_y);
                if (current_tile)
                {
                    current_tile->Sprite.Play();
                    current_tile->Sprite.Update(elapsed_time);
                }
            }
        }

        // UPDATE THE CURRENT TILE MAP'S ANIMALS.
        for (auto& animal : Animals)
        {
            animal->Sprite.Update(elapsed_time);
        }

        // UPDATE THE CURRENT TILE MAP'S TREES.
        for (auto tree = Trees.begin(); tree != Trees.end(); ++tree)
        {
            // UPDATE THE TREE.
            tree->Update(elapsed_time);

            // START PLAYING THE TREE SHAKING SOUND EFFECT IF APPROPRIATE.
            bool is_shaking = tree->IsShaking();
            if (is_shaking)
            {
                // ONLY START PLAYING THE SOUND IF IT ISN'T ALREADY PLAYING.
                // This results in a smoother sound experience.
                bool tree_shake_sound_playing = speakers.SoundIsPlaying(RESOURCES::AssetId::TREE_SHAKE_SOUND);
                if (!tree_shake_sound_playing)
                {
                    speakers.PlaySoundEffect(RESOURCES::AssetId::TREE_SHAKE_SOUND);
                }
            }
        }

        // UPDATE THE CURRENT TILE MAP'S DUST CLOUDS.
        for (auto dust_cloud = DustClouds.begin(); dust_cloud != DustClouds.end();)
        {
            // UPDATE THE CURRENT DUST CLOUD.
            dust_cloud->Update(elapsed_time);

            // REMOVE THE DUST CLOUD IF IT HAS DISAPPEARED.
            bool dust_cloud_disappeared = dust_cloud->HasDisappeared();
            if (dust_cloud_disappeared)
            {
                // REMOVE THE DUST CLOUD.
                dust_cloud = DustClouds.erase(dust_cloud);
            }
            else
            {
                // MOVE TO UPDATING THE NEXT DUST CLOUD.
                ++dust_cloud;
            }
        }
    }
}