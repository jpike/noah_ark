#include <array>
#include "Maps/Tile.h"

using namespace MAPS;

Tile::Tile(
    const unsigned int tileId,
    const std::shared_ptr<GRAPHICS::Sprite>& sprite) :
    m_tileId(tileId),
    m_sprite(sprite)
{
    // Nothing else to do.
}

Tile::Tile(const Tile& tileToCopy) :
    m_tileId(),
    m_sprite()
{
    Copy(tileToCopy);
}

Tile::~Tile()
{
    // Nothing else to do.
}

Tile& Tile::operator= (const Tile& rhsTile)
{
    // CHECK FOR SELF-ASSIGNMENT.
    bool selfAssignment = ( this == &rhsTile );
    if (!selfAssignment)
    {
        // Copy the values of the right-hand side tile into this tile.
        Copy(rhsTile);
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
    std::array<bool, TILESET_TILE_COUNT> tileIsWalkable;
    tileIsWalkable[BLACK_TEST_TILE_ID] = false;
    tileIsWalkable[WHITE_TEST_TILE_ID] = false;
    tileIsWalkable[SAND_TILE_ID] = true;
    tileIsWalkable[GRASS_TILE_ID] = true;
    tileIsWalkable[WATER_TILE_ID] = false;
    tileIsWalkable[BROWN_DIRT_TILE_ID] = true;
    tileIsWalkable[GRAY_STONE_TILE_ID] = true;

    // CHECK IF THIS TILE'S ID IS VALID.
    bool tileIdValid = (m_tileId < TILESET_TILE_COUNT);
    if (tileIdValid)
    {
        // Return the walkable status of the valid tile.
        return tileIsWalkable[m_tileId];
    }
    else
    {
        // The tile ID is invalid, so assume the tile isn't walkable.
        return false;
    }
}

float Tile::GetHeightInPixels() const
{
    MATH::FloatRectangle tileBoundingBox = m_sprite->GetBoundingBox();
    return tileBoundingBox.GetHeight();
} 

float Tile::GetLeftXPosition() const
{
    MATH::FloatRectangle tileBoundingBox = m_sprite->GetBoundingBox();
    return tileBoundingBox.GetLeftXPosition();
}

float Tile::GetRightXPosition() const
{
    MATH::FloatRectangle tileBoundingBox = m_sprite->GetBoundingBox();
    return tileBoundingBox.GetRightXPosition();
}

float Tile::GetTopYPosition() const
{
    MATH::FloatRectangle tileBoundingBox = m_sprite->GetBoundingBox();
    return tileBoundingBox.GetTopYPosition();
}

float Tile::GetBottomYPosition() const
{
    MATH::FloatRectangle tileBoundingBox = m_sprite->GetBoundingBox();
    return tileBoundingBox.GetBottomYPosition();
}

void Tile::Copy(const Tile& tileToCopy)
{
    this->m_tileId = tileToCopy.m_tileId;
    this->m_sprite = tileToCopy.m_sprite;
}
