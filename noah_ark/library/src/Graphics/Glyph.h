#pragma once

#include "Graphics/Texture.h"
#include "Math/Rectangle.h"

namespace GRAPHICS
{
    /// A glyph is a graphical representation of a single character within a font.
    class Glyph
    {
    public:
        // CONSTANTS.
        /// The width of a glyph, in pixels.
        static const unsigned int WIDTH_IN_PIXELS = 16;
        /// The heght of a glyph, in pixels.
        static const unsigned int HEIGHT_IN_PIXELS = 16;

        // MEMBER VARIABLES.
        /// The texture containing this glyph.
        std::shared_ptr<Texture> Texture = nullptr;
        /// The sub-rectangle of the texture identifying the boundaries of the glyph.
        /// (0,0) indicates the top-left of the texture, with x coordinates increasing
        /// from left-to-right and y coordinates increasing from top-to-bottom.
        MATH::FloatRectangle TextureSubRectangle = MATH::FloatRectangle();
    };
}
