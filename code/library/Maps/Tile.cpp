#include <array>
#include <limits>
#include "Debugging/DebugConsole.h"
#include "Maps/Tile.h"

namespace MAPS
{
    const std::vector<TileType::Id> TileType::WATER_TYPES = 
    {
        TileType::WATER,
        TileType::WATER_ANIMATED_1,
        TileType::WATER_ANIMATED_2
    };
    
    /// Determines if the specified type of tile is a water tile.
    /// @param[in]  tile_type_id - The ID of the tile type to check.
    /// @return True if the specified type of tile is for water; false if not.
    bool TileType::IsForWater(const Id tile_type_id)
    {
        bool is_for_water = (
            WATER == tile_type_id ||
            WATER_ANIMATED_1 == tile_type_id ||
            WATER_ANIMATED_2 == tile_type_id);
        return is_for_water;
    }

    /// Determines if the specified type of tile is for the ark.
    /// @param[in]  tile_type_id - The ID of the tile type to check.
    /// @return True if the specified type of tile is for the ark; false if not.
    bool TileType::IsForArk(const Id tile_type_id)
    {
        bool is_for_ark = (
            ARK_BUILDING_PLOT == tile_type_id ||
            ARK_INTERIOR_WOOD_FLOOR == tile_type_id ||
            ARK_INTERIOR_LEFT_EXIT == tile_type_id ||
            ARK_INTERIOR_CENTER_EXIT == tile_type_id ||
            ARK_INTERIOR_RIGHT_EXIT == tile_type_id ||
            ARK_INTERIOR_UP_STAIRS == tile_type_id ||
            ARK_INTERIOR_DOWN_STAIRS == tile_type_id ||
            ARK_INTERIOR_LEFT_EXIT_CLOSED == tile_type_id ||
            ARK_INTERIOR_CENTER_EXIT_CLOSED == tile_type_id ||
            ARK_INTERIOR_RIGHT_EXIT_CLOSED == tile_type_id ||
            ANIMAL_PEN_FENCE == tile_type_id ||
            ANIMAL_PEN_GROUND == tile_type_id ||
            ANIMAL_PEN_ENTRANCE == tile_type_id);
        return is_for_ark;
    }

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
        const GRAPHICS::AnimatedSprite& sprite) :
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
        return tile_bounding_box.LeftTop.X;
    }

    /// Gets the right X position of the tile's bounding box, in world coordinates.
    /// @return The right X position of the tile.
    float Tile::GetRightXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.RightBottom.X;
    }

    /// Gets the top Y position of the tile's bounding box, in world coordinates.
    /// @return The top Y position of the tile.
    float Tile::GetTopYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.LeftTop.Y;
    }

    /// Gets the bottomt Y position of the tile's bounding box, in world coordinates.
    /// @return The bottom Y position of the tile.
    float Tile::GetBottomYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetWorldBoundingBox();
        return tile_bounding_box.RightBottom.Y;
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
        tile_is_walkable[TileType::ARK_INTERIOR_WOOD_FLOOR] = true;
        tile_is_walkable[TileType::ARK_INTERIOR_LEFT_EXIT] = true;
        tile_is_walkable[TileType::ARK_INTERIOR_CENTER_EXIT] = true;
        tile_is_walkable[TileType::ARK_INTERIOR_RIGHT_EXIT] = true;
        tile_is_walkable[TileType::ARK_INTERIOR_UP_STAIRS] = true;
        tile_is_walkable[TileType::ARK_INTERIOR_DOWN_STAIRS] = true;
        tile_is_walkable[TileType::ARK_INTERIOR_LEFT_EXIT_CLOSED] = false;
        tile_is_walkable[TileType::ARK_INTERIOR_CENTER_EXIT_CLOSED] = false;
        tile_is_walkable[TileType::ARK_INTERIOR_RIGHT_EXIT_CLOSED] = false;
        tile_is_walkable[TileType::WATER_ANIMATED_1] = false;
        tile_is_walkable[TileType::WATER_ANIMATED_2] = false;
        tile_is_walkable[TileType::ANIMAL_PEN_FENCE] = false;
        tile_is_walkable[TileType::ANIMAL_PEN_GROUND] = true;
        tile_is_walkable[TileType::ANIMAL_PEN_ENTRANCE] = true;

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
