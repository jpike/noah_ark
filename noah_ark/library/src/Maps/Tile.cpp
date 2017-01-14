#include <array>
#include <limits>
#include "Maps/Tile.h"

namespace MAPS
{
    /// Constructor to create an invalid tile.
    Tile::Tile() :
    Id(std::numeric_limits<TileId>::max()),
    Sprite()
    {}

    /// Constructor accepting all of a tile's data.
    /// @param[in]  id - A numeric ID identifying the tile in a tileset.
    /// @param[in]  sprite - The sprite used for rendering the tile.
    Tile::Tile(
        const TileId id,
        const GRAPHICS::Sprite& sprite) :
    Id(id),
    Sprite(sprite)
    {}

    /// Sets the world position of the center of the tile.
    /// @param[in]  world_x_position - The world x position of the center of the tile.
    /// @param[in]  world_y_position - The world y position of the center of the tile.
    void Tile::SetWorldPosition(const float world_x_position, const float world_y_position)
    {
        Sprite.SetWorldPosition(world_x_position, world_y_position);
    }

    /// Gets the width of the tile, in pixels.
    /// @return The width of the tile in pixels.
    float Tile::GetWidthInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.GetWidth();
    }

    /// Gets the height of the tile, in pixels.
    /// @return The height of the tile in pixels.
    float Tile::GetHeightInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.GetHeight();
    }

    /// Gets the left X position of the tile's bounding box, in world coordinates.
    /// @return The left X position of the tile.
    float Tile::GetLeftXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.GetLeftXPosition();
    }

    /// Gets the right X position of the tile's bounding box, in world coordinates.
    /// @return The right X position of the tile.
    float Tile::GetRightXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.GetRightXPosition();
    }

    /// Gets the top Y position of the tile's bounding box, in world coordinates.
    /// @return The top Y position of the tile.
    float Tile::GetTopYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.GetTopYPosition();
    }

    /// Gets the bottomt Y position of the tile's bounding box, in world coordinates.
    /// @return The bottom Y position of the tile.
    float Tile::GetBottomYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.GetBottomYPosition();
    }

    /// Determines if the tile is walkable.
    /// @return True if the tile is walkable; false otherwise.
    bool Tile::IsWalkable() const
    {
        // DEFINE ID CONSTANTS FOR EACH TILE IN THE TILESET.
        // A 0 entry had to be added here to account for
        // the fact that the first tile ID read from
        // the tile map file starts at 1, not 0.
        const unsigned int INVALID_TILE_ID = 0;
        const unsigned int BLACK_TEST_TILE_ID = 1;
        const unsigned int WHITE_TEST_TILE_ID = 2;
        const unsigned int SAND_TILE_ID = 3;
        const unsigned int GRASS_TILE_ID = 4;
        const unsigned int WATER_TILE_ID = 5;
        const unsigned int BROWN_DIRT_TILE_ID = 6;
        const unsigned int GRAY_STONE_TILE_ID = 7;
        const unsigned int ARK_BUILDING_PLOT_TILE_ID = 8;

        // MAP EACH TILE ID TO A WALKABLE STATUS.
        const unsigned int TILESET_TILE_COUNT = 9;
        std::array<bool, TILESET_TILE_COUNT> tile_is_walkable;
        tile_is_walkable[INVALID_TILE_ID] = false;
        tile_is_walkable[BLACK_TEST_TILE_ID] = false;
        tile_is_walkable[WHITE_TEST_TILE_ID] = false;
        tile_is_walkable[SAND_TILE_ID] = true;
        tile_is_walkable[GRASS_TILE_ID] = true;
        tile_is_walkable[WATER_TILE_ID] = false;
        tile_is_walkable[BROWN_DIRT_TILE_ID] = true;
        tile_is_walkable[GRAY_STONE_TILE_ID] = true;
        tile_is_walkable[ARK_BUILDING_PLOT_TILE_ID] = true;

        // CHECK IF THIS TILE'S ID IS VALID.
        bool tile_id_valid = (Id < TILESET_TILE_COUNT);
        if (tile_id_valid)
        {
            // Return the walkable status of the valid tile.
            return tile_is_walkable[Id];
        }
        else
        {
            // The tile ID is invalid, so assume the tile isn't walkable.
            return false;
        }
    }
}
