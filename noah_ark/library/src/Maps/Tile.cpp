#include <array>
#include <limits>
#include "Maps/Tile.h"

namespace MAPS
{
    Tile::Tile() :
    TileId(std::numeric_limits<unsigned int>::max()),
    Sprite()
    {}

    Tile::Tile(
        const unsigned int tile_id,
        const std::shared_ptr<GRAPHICS::Sprite>& sprite) :
    TileId(tile_id),
    Sprite(sprite)
    {}

    Tile::Tile(const Tile& tile_to_copy) :
    TileId(),
    Sprite()
    {
        Copy(tile_to_copy);
    }

    Tile::~Tile()
    {
        // Nothing else to do.
    }

    Tile& Tile::operator= (const Tile& rhs_tile)
    {
        // CHECK FOR SELF-ASSIGNMENT.
        bool self_assignment = (this == &rhs_tile);
        if (!self_assignment)
        {
            // Copy the values of the right-hand side tile into this tile.
            Copy(rhs_tile);
        }

        // RETURN THIS OBJECT.
        return (*this);
    }

    bool Tile::IsWalkable() const
    {
        // DEFINE ID CONSTANTS FOR EACH TILE IN THE TILESET.
        const unsigned int BLACK_TEST_TILE_ID = 0;
        const unsigned int WHITE_TEST_TILE_ID = 1;
        const unsigned int SAND_TILE_ID = 2;
        const unsigned int GRASS_TILE_ID = 3;
        const unsigned int WATER_TILE_ID = 4;
        const unsigned int BROWN_DIRT_TILE_ID = 5;
        const unsigned int GRAY_STONE_TILE_ID = 6;

        // MAP EACH TILE ID TO A WALKABLE STATUS.
        const unsigned int TILESET_TILE_COUNT = 7;
        std::array<bool, TILESET_TILE_COUNT> tile_is_walkable;
        tile_is_walkable[BLACK_TEST_TILE_ID] = false;
        tile_is_walkable[WHITE_TEST_TILE_ID] = false;
        tile_is_walkable[SAND_TILE_ID] = true;
        tile_is_walkable[GRASS_TILE_ID] = true;
        tile_is_walkable[WATER_TILE_ID] = false;
        tile_is_walkable[BROWN_DIRT_TILE_ID] = true;
        tile_is_walkable[GRAY_STONE_TILE_ID] = true;

        // CHECK IF THIS TILE'S ID IS VALID.
        bool tile_id_valid = (TileId < TILESET_TILE_COUNT);
        if (tile_id_valid)
        {
            // Return the walkable status of the valid tile.
            return tile_is_walkable[TileId];
        }
        else
        {
            // The tile ID is invalid, so assume the tile isn't walkable.
            return false;
        }
    }

    float Tile::GetHeightInPixels() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetHeight();
    }

    float Tile::GetLeftXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetLeftXPosition();
    }

    float Tile::GetRightXPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetRightXPosition();
    }

    float Tile::GetTopYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetTopYPosition();
    }

    float Tile::GetBottomYPosition() const
    {
        MATH::FloatRectangle tile_bounding_box = Sprite->GetBoundingBox();
        return tile_bounding_box.GetBottomYPosition();
    }

    void Tile::Copy(const Tile& tile_to_copy)
    {
        this->TileId = tile_to_copy.TileId;
        this->Sprite = tile_to_copy.Sprite;
    }
}
