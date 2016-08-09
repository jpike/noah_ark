#include "Maps/TileMapObjectDescription.h"

namespace MAPS
{
    /// Determines if two object descriptions are equal.
    /// @param[in]  rhs - The object to compare with.
    /// @return True if this object and the provided object are equal;
    ///     false otherwise.
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