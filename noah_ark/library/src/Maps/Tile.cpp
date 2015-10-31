#include <array>
#include <limits>
#include "Maps/Tile.h"

namespace MAPS
{
    Tile::Tile() :
    Id(std::numeric_limits<TileId>::max()),
    Sprite()
    {}

    Tile::Tile(
        const TileId id,
        const GRAPHICS::Sprite& sprite) :
    Id(id),
    Sprite(sprite)
    {}

    void Tile::SetWorldPosition(const float world_x_position, const float world_y_position)
    {
        Sprite.SetWorldPosition(world_x_position, world_y_position);
    }

    float Tile::GetWidthInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetBoundingBox();
        return tile_bounding_box.GetWidth();
    }

    float Tile::GetHeightInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetBoundingBox();
        return tile_bounding_box.GetHeight();
    }

    float Tile::GetLeftXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetBoundingBox();
        return tile_bounding_box.GetLeftXPosition();
    }

    float Tile::GetRightXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetBoundingBox();
        return tile_bounding_box.GetRightXPosition();
    }

    float Tile::GetTopYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetBoundingBox();
        return tile_bounding_box.GetTopYPosition();
    }

    float Tile::GetBottomYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite.GetBoundingBox();
        return tile_bounding_box.GetBottomYPosition();
    }

    bool Tile::IsWalkable() const
    {
        // DEFINE ID CONSTANTS FOR EACH TILE IN THE TILESET.
        /// @todo   Re-think invalid tile ID stuff here.
        /// A 0 entry had to be added here to account for
        /// the fact that the first tile ID read from
        /// the tile map file started at 1, not 0.
        const unsigned int INVALID_TILE_ID = 0;
        const unsigned int BLACK_TEST_TILE_ID = 1;
        const unsigned int WHITE_TEST_TILE_ID = 2;
        const unsigned int SAND_TILE_ID = 3;
        const unsigned int GRASS_TILE_ID = 4;
        const unsigned int WATER_TILE_ID = 5;
        const unsigned int BROWN_DIRT_TILE_ID = 6;
        const unsigned int GRAY_STONE_TILE_ID = 7;

        // MAP EACH TILE ID TO A WALKABLE STATUS.
        const unsigned int TILESET_TILE_COUNT = 8;
        std::array<bool, TILESET_TILE_COUNT> tile_is_walkable;
        tile_is_walkable[INVALID_TILE_ID] = false;
        tile_is_walkable[BLACK_TEST_TILE_ID] = false;
        tile_is_walkable[WHITE_TEST_TILE_ID] = false;
        tile_is_walkable[SAND_TILE_ID] = true;
        tile_is_walkable[GRASS_TILE_ID] = true;
        tile_is_walkable[WATER_TILE_ID] = false;
        tile_is_walkable[BROWN_DIRT_TILE_ID] = true;
        tile_is_walkable[GRAY_STONE_TILE_ID] = true;

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
