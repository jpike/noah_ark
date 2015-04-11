#pragma once

#include <string>

namespace MAPS
{
    /// A basic description of a tileset as read from
    /// a Tiled tile map.  Only basic properties that
    /// are currently needed are included as fields,
    /// but this list may be expanded later as necessary.
    struct TilesetDescription
    {
        /// The name of the tileset.  Identifies the texture
        /// resource for the tileset.
        std::string Name;
        /// The globally unique tile ID of the first tile in the tileset.
        unsigned int FirstTileId;
        /// The width of a tile in the tileset in pixels.
        unsigned int TileWidthInPixels;
        /// The height of a tile in the tileset in pixels.
        unsigned int TileHeightInPixels;
        /// The transparent color in the tileset image.
        /// Must be represented as an HTML hexadecimal HTML color
        /// code like #00ffcc.  Capitalization doesn't matter.
        std::string TransparentColor;

        /// Determines if two tileset descriptions are equal.
        /// @param[in]  rhs - The tileset to compare with.
        /// @return True if this tileset and the provided tileset are equal;
        ///     false otherwise.
        bool operator==(const TilesetDescription& rhs) const;
    };
}