#include "Maps/TileMapObjectDescription.h"

namespace MAPS
{
    bool TileMapObjectDescription::operator == (const TileMapObjectDescription& rhs) const
    {
        // The descriptions aren't equal if a single field differs.
        if (Name != rhs.Name) return false;
        if (Type != rhs.Type) return false;
        if (TopLeftPositionInPixels != rhs.TopLeftPositionInPixels) return false;
        if (WidthInPixels != rhs.WidthInPixels) return false;
        if (HeightInPixels != rhs.HeightInPixels) return false;

        // All fields were equal.
        return true;
    }
}