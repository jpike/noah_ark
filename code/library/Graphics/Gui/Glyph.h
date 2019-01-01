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
        static constexpr unsigned int MAX_WIDTH_IN_PIXELS = 16;
        /// The standard (scale of 1) height of a glyph, in pixels.
        static constexpr unsigned int MAX_HEIGHT_IN_PIXELS = 16;
        /// The total number of glyphs (characters).
        static constexpr unsigned int TOTAL_COUNT = 256;
        /// The texture sub-rectangles for each glyph (character).
        static const std::array<MATH::FloatRectangle, Glyph::TOTAL_COUNT> TEXTURE_SUB_RECTANGLES;

        // STATIC METHODS.
        template <typename T>
        static T WidthInPixels(const float scale_factor);
        template <typename T>
        static T HeightInPixels(const float scale_factor);
        template <typename T>
        static T SpaceOnLeftInPixels(const char character);
        template <typename T>
        static T SpaceOnRightInPixels(const char character);
        template <typename T>
        static T TextWidthInPixels(const std::string& text, const float text_scale_ratio);

        // MEMBER VARIABLES.
        /// The texture containing this glyph.
        std::shared_ptr<Texture> Texture = nullptr;
        /// The sub-rectangle of the texture identifying the boundaries of the glyph.
        /// (0,0) indicates the top-left of the texture, with x coordinates increasing
        /// from left-to-right and y coordinates increasing from top-to-bottom.
        MATH::FloatRectangle TextureSubRectangle = MATH::FloatRectangle();

    private:
        template <typename T>
        static T GetBoundingBoxLeftPositionInPixels(const char character);
    };

    /// Calculates the width of a glyph, based on the provided scale factor.
    /// The max width for a glyph is assumed.
    /// @tparam T - The type of the returned value.  Will be directly casted.
    /// @param[in]  scale_factor - The scaling factor of the glyph.  1 is normal scaling.
    /// @return The scaled width of the glyph, in pixels.
    template <typename T>
    T Glyph::WidthInPixels(const float scale_factor)
    {
        float scaled_width_in_pixels = scale_factor * static_cast<float>(MAX_WIDTH_IN_PIXELS);
        return static_cast<T>(scaled_width_in_pixels);
    }

    /// Calculates the height of a glyph, based on the provided scale factor.
    /// @tparam T - The type of the returned value.  Will be directly casted.
    /// @param[in]  scale_factor - The scaling factor of the glyph.  1 is normal scaling.
    /// @return The scaled height of the glyph, in pixels.
    template <typename T>
    T Glyph::HeightInPixels(const float scale_factor)
    {
        float scaled_height_in_pixels = scale_factor * static_cast<float>(MAX_HEIGHT_IN_PIXELS);
        return static_cast<T>(scaled_height_in_pixels);
    }

    /// Gets the space on the left of the specified character.
    /// @tparam T - The type of the returned value.  Will be directly casted.
    /// @param[in]  character - The character for which to get space on the left in the glyph.
    /// @return The amount of space on the left of the character's glyph, in pixels.
    template <typename T>
    T Glyph::SpaceOnLeftInPixels(const char character)
    {
        const MATH::FloatRectangle& texture_bounding_box = TEXTURE_SUB_RECTANGLES[character];
        float glyph_left_offset_in_pixels = GetBoundingBoxLeftPositionInPixels<float>(character);
        float glyph_left_x_position_in_pixels = texture_bounding_box.GetLeftXPosition();
        float space_on_left_in_pixels = glyph_left_x_position_in_pixels - glyph_left_offset_in_pixels;
        return static_cast<T>(space_on_left_in_pixels);
    }

    /// Gets the space on the right of the specified character.
    /// @tparam T - The type of the returned value.  Will be directly casted.
    /// @param[in]  character - The character for which to get space on the right in the glyph.
    /// @return The amount of space on the right of the character's glyph, in pixels.
    template <typename T>
    T Glyph::SpaceOnRightInPixels(const char character)
    {
        const MATH::FloatRectangle& texture_bounding_box = TEXTURE_SUB_RECTANGLES[character];
        float glyph_right_x_position_in_pixels = texture_bounding_box.GetRightXPosition();
        float glyph_left_offset_in_pixels = GetBoundingBoxLeftPositionInPixels<float>(character);
        float space_on_right_in_pixels = static_cast<float>(MAX_WIDTH_IN_PIXELS) - (glyph_right_x_position_in_pixels - glyph_left_offset_in_pixels);
        return static_cast<T>(space_on_right_in_pixels);
    }

    /// Calculates the width of some text, in pixels.
    /// @tparam T - The type of the returned value.  Will be directly casted.
    /// @param[in]  text - The text whose total width to calculate.
    /// @param[in]  scale_factor - The scaling factor of the glyphs.  1 is normal scaling.
    template <typename T>
    T Glyph::TextWidthInPixels(const std::string& text, const float scale_factor)
    {
        float text_width_in_pixels = 0.0f;

        // COUNT THE WIDTH OF EACH GLYPH.
        for (const char character : text)
        {
            const MATH::FloatRectangle& glyph_bounding_box = TEXTURE_SUB_RECTANGLES[character];
            float glyph_default_width_in_pixels = glyph_bounding_box.GetWidth();
            float glyph_scaled_width_in_pixels = scale_factor * glyph_default_width_in_pixels;

            /// @todo   Better centralize this constant.  It's duplicated in the Renderer.
            /// Some new text abstractions may be appropriate.
            constexpr float SPACING_IN_PIXELS_BETWEEN_EACH_CHARACTER = 2.0f;

            text_width_in_pixels += glyph_scaled_width_in_pixels + SPACING_IN_PIXELS_BETWEEN_EACH_CHARACTER;
        }

        return static_cast<T>(text_width_in_pixels);
    }

    /// Gets the left position of the glyph's bounding box for the character.
    /// @tparam T - The type of the returned value.  Will be directly casted.
    /// @param[in]  character - The character for which to get left position of the glyph's bounding box.
    /// @return The left position of the glyph's bounding box, in pixels.
    template <typename T>
    T Glyph::GetBoundingBoxLeftPositionInPixels(const char character)
    {
        // CALCULATE THE TOP-LEFT COORDINATES OF THE GLYPH.
        constexpr char CHARACTERS_PER_ROW_COUNT = 16;
        char character_horizontal_index = character % CHARACTERS_PER_ROW_COUNT;
        T glyph_left_position = static_cast<T>(character_horizontal_index * Glyph::MAX_WIDTH_IN_PIXELS);
        return glyph_left_position;
    }
}
}
