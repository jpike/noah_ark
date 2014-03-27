#include "Maps/Tile.h"

using namespace MAPS;

Tile::Tile() :
    m_sprite()
{
    // Nothing else to do.
}

Tile::Tile(const Tile& tileToCopy) :
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

void Tile::Copy(const Tile& tileToCopy)
{
    this->m_sprite = tileToCopy.m_sprite;
}
