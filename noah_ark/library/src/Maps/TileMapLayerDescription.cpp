#include "Maps/TileMapLayerDescription.h"

namespace MAPS
{
    bool TileMapLayerDescription::operator == (const TileMapLayerDescription& rhs) const
    {
        // The descriptions aren't equal if a single field differs.
        if (Name != rhs.Name) return false;
        if (Type != rhs.Type) return false;
        if (TileIds != rhs.TileIds) return false;
        if (Objects != rhs.Objects) return false;

        // All fields were equal.
        return true;
    }
}