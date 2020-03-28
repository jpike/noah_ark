#pragma once

#include <string>
#include "Graphics/Color.h"
#include "Math/Vector2.h"
#include "Resources/AssetId.h"

namespace GRAPHICS::GUI
{
    /// Text that can be rendered on a graphical GUI.
    /// Units for this class are in screen/pixel units.
    class Text
    {
    public:
        // STATIC CONSTANTS.
        /// The horizontal spacing (in pixels) between characters in text.
        /// One pixel of spacing on each side of a character is used for better readability.
        static constexpr float HORIZONTAL_SPACING_IN_PIXELS_BETWEEN_EACH_CHARACTER = 2.0f;

        // DIMENSION METHODS.
        template <typename NumericType>
        static NumericType Width(const std::string& text, const float scale_factor);
        template <typename NumericType>
        static NumericType Height(const float scale_factor);

        // POSITION METHODS.
        float RightPosition() const;
        float BottomPosition() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The string of characters for the text.
        std::string String = "";
        /// The font to use for rendering the text.
        RESOURCES::AssetId FontId = RESOURCES::AssetId::FONT_TEXTURE;
        /// The left, top (x, y) screen position at which the text should be rendered.
        MATH::Vector2f LeftTopPosition = MATH::Vector2f(0.0f, 0.0f);
        /// The scale of the text, relative to the default scale (its normal size of 1).
        /// This is a multiplicative factor that can be applied to increase or decrease
        /// the size of the rendered text.
        float ScaleFactor = 1.0f;
        /// The color of the rendered text.
        GRAPHICS::Color Color = GRAPHICS::Color::BLACK;
    };

    /// Calculates the width of some text, in pixels.
    /// This method is static to support usage in cases where they may not be a fully-formed "Text"
    /// object and someone just wants to get the text width for string.
    /// @tparam NumericType - The type of the returned value.  Will be directly casted.
    /// @param[in]  text - The text whose total width to calculate.
    /// @param[in]  scale_factor - The scaling factor of the glyphs.  1 is normal scaling.
    /// @return The width of the text, in pixels.
    template <typename NumericType>
    NumericType Text::Width(const std::string& text, const float scale_factor)
    {
        float text_width_in_pixels = 0.0f;

        // COUNT THE WIDTH OF EACH GLYPH.
        for (const char character : text)
        {
            const MATH::FloatRectangle& glyph_bounding_box = Glyph::TEXTURE_SUB_RECTANGLES[character];
            float glyph_default_width_in_pixels = glyph_bounding_box.Width();
            float glyph_scaled_width_in_pixels = scale_factor * glyph_default_width_in_pixels;
            text_width_in_pixels += glyph_scaled_width_in_pixels + HORIZONTAL_SPACING_IN_PIXELS_BETWEEN_EACH_CHARACTER;
        }

        return static_cast<NumericType>(text_width_in_pixels);
    }

    /// Calculates the height of the text, based on the provided scale factor.
    /// This method is static for consistency with the Width() method.
    /// @tparam NumericType - The type of the returned value.  Will be directly casted.
    /// @param[in]  scale_factor - The scaling factor of the text.  1 is normal scaling.
    /// @return The scaled height of the text, in pixels.
    template <typename NumericType>
    NumericType Text::Height(const float scale_factor)
    {
        float text_height = Glyph::HeightInPixels<float>(scale_factor);
        return static_cast<NumericType>(text_height);
    }
}
