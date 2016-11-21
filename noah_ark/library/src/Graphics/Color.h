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
        /// The color red.
        static const Color RED;
        /// The color green.
        static const Color GREEN;
        /// The color back.
        static const Color BLACK;

        // CONSTRUCTION.
        explicit Color(
            const uint8_t red = 0,
            const uint8_t green = 0,
            const uint8_t blue = 0,
            const uint8_t alpha = MAX_COLOR_COMPONENT);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The red component of the color.
        uint8_t Red;
        /// The green component of the color.
        uint8_t Green;
        /// The blue component of the color.
        uint8_t Blue;
        /// The alpha component of the color.
        uint8_t Alpha;
    };
}
