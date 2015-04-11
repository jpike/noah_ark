#include "Maps/TilesetDescription.h"

namespace MAPS
{
    bool TilesetDescription::operator == (const TilesetDescription& rhs) const
    {
        // The descriptions aren't equal if a single field differs.
        if (Name != rhs.Name) return false;
        if (FirstTileId != rhs.FirstTileId) return false;
        if (TileWidthInPixels != rhs.TileWidthInPixels) return false;
        if (TileHeightInPixels != rhs.TileHeightInPixels) return false;
        if (TransparentColor != rhs.TransparentColor) return false;

        // All fields were equal.
        return true;
    }
}