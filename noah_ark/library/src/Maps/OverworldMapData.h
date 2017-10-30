#pragma once

#include "Core/Array2D.h"
#include "Maps/TileMapData.h"

namespace MAPS
{
    /// The width of the overworld in tile maps.
    constexpr unsigned int OVERWORLD_WIDTH_IN_TILE_MAPS = 7;
    /// The height of the overworld in tile maps.
    constexpr unsigned int OVERWORLD_HEIGHT_IN_TILE_MAPS = 7;

    extern const CORE::Array2D<TileMapData> OVERWORLD_MAP_DATA;
}
