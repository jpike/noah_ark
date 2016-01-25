#pragma once

#include <cstdint>

namespace GRAPHICS
{
    /// Represents an RGBA color.
    /// Currently only uses [0, 255] integer components, rather than
    /// including float support, since float support is not yet needed.
    /// A default color represents an opaque black color.
    class Color
    {
    public:
        // STATIC CONSTANTS.
        /// The max value for a single color component value.
        static const uint8_t MAX_COLOR_COMPONENT = 255;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The red component of the color.
        uint8_t Red = 0;
        /// The green component of the color.
        uint8_t Green = 0;
        /// The blue component of the color.
        uint8_t Blue = 0;
        /// The alpha component of the color.
        uint8_t Alpha = MAX_COLOR_COMPONENT;
    };
}
