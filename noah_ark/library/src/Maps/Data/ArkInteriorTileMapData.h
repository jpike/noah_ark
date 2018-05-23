#pragma once

#include <cstdint>
#include "Core/Array2D.h"

namespace MAPS
{
namespace DATA
{
    extern const CORE::Array2D<uint8_t> ARK_INTERIOR_TILE_MAP_0_0;
    extern const CORE::Array2D<uint8_t> ARK_INTERIOR_TILE_MAP_1_0;
    extern const CORE::Array2D<uint8_t> ARK_INTERIOR_TILE_MAP_2_0;

    extern const CORE::Array2D< const CORE::Array2D<uint8_t>* > ARK_INTERIOR_MAP_DATA;
}
}
