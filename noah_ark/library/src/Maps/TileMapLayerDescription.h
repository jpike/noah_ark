#pragma once

#include <string>
#include <vector>
#include "Core/Array2D.h"
#include "Maps/Tile.h"
#include "Maps/TileMapObjectDescription.h"

namespace MAPS
{
    /// Identifies the different types of layers that may
    /// exist in a tile map.
    enum class TileMapLayerType
    {
        /// The layer contains tiles in a grid.
        TILE_LAYER,
        /// The layer contains objects defined by rectangles.
        OBJECT_LAYER
    };

    /// A description of a layer in a tile map.
    /// Many fields of layers in Tiled map files (ex. position/dimensions)
    /// are excluded because they only provide redundant data that could
    /// be figured out from other tile map properties.
    struct TileMapLayerDescription
    {
        /// The name identifying the tile map layer.
        std::string Name;
        /// The type of the tile map layer.
        TileMapLayerType Type;
        /// The IDs of each tile in the layer.  Only populated if the type
        /// of layer in this description is for a tile layer.  The tile
        /// IDs are contained in 1D container to simplify interaction logic.
        /// The tiles start with the top row, going from left to right,
        /// with rows further down in the layer following.
        CORE::Array2D<TileId> TileIds;
        /// A list of objects in the layer.  Only populated if the type
        /// of layer in this description is for an object layer.
        std::vector<TileMapObjectDescription> Objects;

        /// Determines if two layer descriptions are equal.
        /// @param[in]  rhs - The layer to compare with.
        /// @return True if this layer and the provided layer are equal;
        ///     false otherwise.
        bool operator==(const TileMapLayerDescription& rhs) const;
    };
}