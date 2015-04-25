#pragma once

#include <vector>
#include "Maps/TileMapLayerDescription.h"
#include "Maps/TilesetDescription.h"

namespace MAPS
{
    /// An interface to allow for different sources of tile map
    /// data (ex. from different file type, etc.).
    class ITileMapData
    {
    public:
        /// Destructor for proper destruction of implementing classes.
        virtual ~ITileMapData() {};

        /// Gets the tile map's width (in tiles);
        /// @return The map's width.
        virtual unsigned int GetWidthInTiles() const = 0;
        /// Gets the tile map's height (in tiles);
        /// @return The map's height.
        virtual unsigned int GetHeightInTiles() const = 0;
        /// Gets the width (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        /// @return The width of a tile.
        virtual unsigned int GetTileWidthInPixels() const = 0;
        /// Gets the height (in pixels) of a tile in the map.  This only defines
        /// the general grid size of the map - individual tiles may have
        /// different sizes.
        /// @return The height of a tile.
        virtual unsigned int GetTileHeightInPixels() const = 0;
        /// Gets descriptions of tilesets for the tile map.
        /// @return Tilesets for the tile map.
        virtual std::vector<TilesetDescription> GetTilesets() const = 0;
        /// Gets descriptions of layers in the tile map.
        /// @return Layers for the tile map.
        virtual std::vector<TileMapLayerDescription> GetLayers() const = 0;
    };
}