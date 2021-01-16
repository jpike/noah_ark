#include "Maps/Ark.h"
#include "Maps/Data/ArkInteriorTileMapData.h"
#include "Maps/TileMap.h"

namespace MAPS
{
namespace DATA
{
    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_0_TILE_MAP_0_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 13, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_0_TILE_MAP_1_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 13, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 11, 11, 12, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_0_TILE_MAP_2_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 13, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_1_TILE_MAP_0_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 14, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 13, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        1, 9, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_1_TILE_MAP_1_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 14, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 13, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9,
        9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_1_TILE_MAP_2_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 14, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 13, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 20, 20, 20, 20, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 20, 21, 21, 21, 20, 9, 9, 9, 1,
        9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 20, 20, 22, 20, 20, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_2_TILE_MAP_0_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 14, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_2_TILE_MAP_1_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 14, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D<uint8_t> ARK_INTERIOR_LAYER_2_TILE_MAP_2_0(TileMap::WIDTH_IN_TILES, TileMap::HEIGHT_IN_TILES,
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 14, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    });

    const CONTAINERS::Array2D< const CONTAINERS::Array2D<uint8_t>* > ARK_INTERIOR_LAYER_0_MAP_DATA(Ark::INTERIOR_WIDTH_IN_TILE_MAPS, Ark::INTERIOR_HEIGHT_IN_TILE_MAPS,
    {
        &ARK_INTERIOR_LAYER_0_TILE_MAP_0_0,
        &ARK_INTERIOR_LAYER_0_TILE_MAP_1_0,
        &ARK_INTERIOR_LAYER_0_TILE_MAP_2_0,
    });

    const CONTAINERS::Array2D< const CONTAINERS::Array2D<uint8_t>* > ARK_INTERIOR_LAYER_1_MAP_DATA(Ark::INTERIOR_WIDTH_IN_TILE_MAPS, Ark::INTERIOR_HEIGHT_IN_TILE_MAPS,
    {
        &ARK_INTERIOR_LAYER_1_TILE_MAP_0_0,
        &ARK_INTERIOR_LAYER_1_TILE_MAP_1_0,
        &ARK_INTERIOR_LAYER_1_TILE_MAP_2_0,
    });

    const CONTAINERS::Array2D< const CONTAINERS::Array2D<uint8_t>* > ARK_INTERIOR_LAYER_2_MAP_DATA(Ark::INTERIOR_WIDTH_IN_TILE_MAPS, Ark::INTERIOR_HEIGHT_IN_TILE_MAPS,
    {
        &ARK_INTERIOR_LAYER_2_TILE_MAP_0_0,
        &ARK_INTERIOR_LAYER_2_TILE_MAP_1_0,
        &ARK_INTERIOR_LAYER_2_TILE_MAP_2_0,
    });

    const std::vector< const CONTAINERS::Array2D< const CONTAINERS::Array2D<uint8_t>* >* > ARK_INTERIOR_LAYER_DATA_FROM_BOTTOM_TO_TOP = 
    {
        &ARK_INTERIOR_LAYER_0_MAP_DATA,
        &ARK_INTERIOR_LAYER_1_MAP_DATA,
        &ARK_INTERIOR_LAYER_2_MAP_DATA
    };
}
}
