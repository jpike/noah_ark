#pragma once

namespace MAPS
{
    /// Enumerates the different types of maps in the game.
    enum class MapType
    {
        /// An invalid type of map.
        INVALID = 0,
        /// A map that is part of the overworld.
        OVERWORLD,
        /// A map that is part of the interior of the ark.
        ARK_INTERIOR
    };
}
