#pragma once

#include <cstdint>
#include <vector>
#include "Containers/Array2D.h"

namespace MAPS
{
namespace DATA
{
    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_0_TILE_MAP_0_0;
    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_0_TILE_MAP_1_0;
    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_0_TILE_MAP_2_0;

    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_1_TILE_MAP_0_0;
    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_1_TILE_MAP_1_0;
    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_1_TILE_MAP_2_0;

    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_2_TILE_MAP_0_0;
    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_2_TILE_MAP_1_0;
    extern const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_2_TILE_MAP_2_0;

    extern const CONTAINERS::Array2D< const CONTAINERS::Array2D<uint8_t>* > ARK_INTERIOR_LAYER_0_MAP_DATA;
    extern const CONTAINERS::Array2D< const CONTAINERS::Array2D<uint8_t>* > ARK_INTERIOR_LAYER_1_MAP_DATA;
    extern const CONTAINERS::Array2D< const CONTAINERS::Array2D<uint8_t>* > ARK_INTERIOR_LAYER_2_MAP_DATA;

    extern const std::vector<
        const CONTAINERS::Array2D< const CONTAINERS::Array2D<uint8_t>* >* > ARK_INTERIOR_LAYER_DATA_FROM_BOTTOM_TO_TOP;
}
}
