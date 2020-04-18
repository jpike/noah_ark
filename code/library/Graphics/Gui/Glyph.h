#pragma once

#include <array>
#include <limits>
#include <string>
#include "Graphics/Texture.h"
#include "Math/Rectangle.h"

namespace GRAPHICS
{
namespace GUI
{
    /// A glyph is a graphical representation of a single character within a font.
    class Glyph
    {
    public:
        // CONSTANTS.
        /// The standard (scale of 1) width of a glyph, in pixels.
        static constexpr unsigned int DEFAULT_WIDTH_IN_PIXELS = 16;
        /// The standard (scale of 1) height of a glyph, in pixels.
        static constexpr unsigned int DEFAULT_HEIGHT_IN_PIXELS = 16;
        /// The total number of glyphs (characters).
        static constexpr unsigned int TOTAL_COUNT = 256;

        // DIMENSION METHODS.
        template <typename T>
        T WidthInPixels(const float scale_factor);
        template <typename T>
        T HeightInPixels(const float scale_factor);

        // MEMBER VARIABLES.
        /// The character represented by this glyph.
        char Character = 0;
        /// The texture containing this glyph.
        std::shared_ptr<Texture> Texture = nullptr;
        /// The sub-rectangle of the texture identifying the boundaries of the glyph.
        /// (0,0) indicates the top-left of the texture, with x coordinates increasing
        /// from left-to-right and y coordinates increasing from top-to-bottom.
        MATH::FloatRectangle TextureSubRectangle = MATH::FloatRectangle();
    };

    /// Calculates the width of a glyph, based on the provided scale factor.
    /// The max width for a glyph is assumed.
    /// @tparam T - The type of the returned value.  Will be directly casted.
    /// @param[in]  scale_factor - The scaling factor of the glyph.  1 is normal scaling.
    /// @return The scaled width of the glyph, in pixels.
    template <typename T>
    T Glyph::WidthInPixels(const float scale_factor)
    {
        float normal_width_in_pixels = TextureSubRectangle.Width();
        float scaled_width_in_pixels = scale_factor * normal_width_in_pixels;
        return static_cast<T>(scaled_width_in_pixels);
    }

    /// Calculates the height of a glyph, based on the provided scale factor.
    /// @tparam T - The type of the returned value.  Will be directly casted.
    /// @param[in]  scale_factor - The scaling factor of the glyph.  1 is normal scaling.
    /// @return The scaled height of the glyph, in pixels.
    template <typename T>
    T Glyph::HeightInPixels(const float scale_factor)
    {
        float normal_height_in_pixels = TextureSubRectangle.Height();
        float scaled_height_in_pixels = scale_factor * normal_height_in_pixels;
        return static_cast<T>(scaled_height_in_pixels);
    }
}
}
