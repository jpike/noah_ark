#include <array>
#include <limits>
#include "Maps/Tile.h"

namespace MAPS
{
    /// Constructor to create an invalid tile.
    Tile::Tile() :
    Id(std::numeric_limits<TileId>::max()),
    Type(TileType::INVALID),
    Sprite()
    {}

    /// Constructor accepting all of a tile's data.
    /// @param[in]  id - A numeric ID identifying the tile in a tileset.
    /// @param[in]  sprite - The sprite used for rendering the tile.
    Tile::Tile(
        const TileId id,
        const GRAPHICS::Sprite& sprite) :
    Id(id),
    Type(static_cast<TileType::Id>(id)),
    Sprite(sprite)
    {}

    /// Sets the world position of the center of the tile.
    /// @param[in]  world_x_position - The world x position of the center of the tile.
    /// @param[in]  world_y_position - The world y position of the center of the tile.
    void Tile::SetWorldPosition(const float world_x_position, const float world_y_position)
    {
        Sprite.SetWorldPosition(world_x_position, world_y_position);
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
        // MAP EACH TILE ID TO A WALKABLE STATUS.
        std::array<bool, TileType::COUNT> tile_is_walkable;
        tile_is_walkable[TileType::INVALID] = false;
        tile_is_walkable[TileType::BLACK_TEST] = false;
        tile_is_walkable[TileType::WHITE_TEST] = false;
        tile_is_walkable[TileType::SAND] = true;
        tile_is_walkable[TileType::GRASS] = true;
        tile_is_walkable[TileType::WATER] = false;
        tile_is_walkable[TileType::BROWN_DIRT] = true;
        tile_is_walkable[TileType::GRAY_STONE] = true;
        tile_is_walkable[TileType::ARK_BUILDING_PLOT] = true;

        // CHECK IF THIS TILE'S ID IS VALID.
        bool tile_id_valid = (Id < TileType::COUNT);
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
