#pragma once

#include "Core/Array2D.h"
#include "Maps/TileMapData.h"

namespace MAPS
{
    const unsigned int OVERWORLD_WIDTH_IN_TILE_MAPS = 7;
    const unsigned int OVERWORLD_HEIGHT_IN_TILE_MAPS = 7;

    extern const CORE::Array2D<TileMapData> OVERWORLD_MAP_DATA;
}
