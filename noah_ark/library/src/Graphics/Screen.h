#pragma once

namespace GRAPHICS
{
    /// Represents the screen onto which the game may be rendered.
    ///
    /// This only encompasses the actual area in which the game
    /// is displayed - it doesn't include any additional area
    /// like window borders that may be included in an actual
    /// hardware display.
    ///
    /// Right now, only constants for the dimensions of the screen
    /// exist.  The intent is that other code will be migrated to
    /// use this class for final rendering to the screen, but time
    /// has not been dedicated yet to do that.
    class Screen
    {
    public:
        /// The width of the screen, in pixels.
        /// Set to be the width of a single tile map.
        static const unsigned int WIDTH_IN_PIXELS = 512;
        /// The height of the screen, in pixels.
        /// Set to be the height of a single tile map.
        static const unsigned int HEIGHT_IN_PIXELS = 384;
    };
}
