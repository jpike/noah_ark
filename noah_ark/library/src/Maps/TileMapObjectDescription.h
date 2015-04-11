#pragma once

#include <string>
#include "Math/Vector2.h"

namespace MAPS
{
    /// A description of an object in a tile map object layer.
    struct TileMapObjectDescription
    {
        /// The name identifying the object.
        std::string Name;
        /// The type of the object.  An arbitrary string
        /// as set in the tile map file.
        std::string Type;
        /// The top left position of the object, relative
        /// to the top left position of its containing map.
        MATH::Vector2ui TopLeftPositionInPixels;
        /// The width of the object in pixels.
        unsigned int WidthInPixels;
        /// The height of the object in pixels.
        unsigned int HeightInPixels;

        /// Determines if two object descriptions are equal.
        /// @param[in]  rhs - The object to compare with.
        /// @return True if this object and the provided object are equal;
        ///     false otherwise.
        bool operator==(const TileMapObjectDescription& rhs) const;
    };
}
