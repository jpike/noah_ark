#pragma once

#include "Graphics/Texture.h"
#include "Math/Rectangle.h"

namespace GRAPHICS
{
    /// A glyph is a graphical representation of a single character within a font.
    class Glyph
    {
    public:
        /// The texture containing this glyph.
        std::shared_ptr<Texture> Texture = nullptr;
        /// The sub-rectangle of the texture identifying the boundaries of the glyph.
        /// (0,0) indicates the top-left of the texture, with x coordinates increasing
        /// from left-to-right and y coordinates increasing from top-to-bottom.
        MATH::FloatRectangle TextureSubRectangle = MATH::FloatRectangle();
    };
}
