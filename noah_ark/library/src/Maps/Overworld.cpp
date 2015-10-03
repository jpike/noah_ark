#include "Maps/Overworld.h"

namespace MAPS
{
    Overworld::Overworld(
        const unsigned int width_in_tile_maps,
        const unsigned int height_in_tile_maps) :
    TileMaps(width_in_tile_maps, height_in_tile_maps)
    {}
}