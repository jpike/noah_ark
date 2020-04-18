#pragma once

#include <memory>
#include "Graphics/Texture.h"
#include "Graphics/Gui/Glyph.h"

/// Holds code related to graphical user interfaces.
namespace GRAPHICS::GUI
{
    /// A font is a particular styling of characters that can be used for rendering text.
    /// Each character is represented by a glyph within the font.
    /// 
    /// This class currently only supports bitmap fonts for ASCII characters 0-255.
    /// A font's bitmap is expected to have 16 rows and 16 columns of glyphs.
    /// The top-left glyph should be for ASCII character 0, and the bottom-right
    /// glyph should be for ASCII character 255.  Each glyph must be represented
    /// in a 16x16 pixel rectangle.
    class Font
    {
    public:
        // STATIC CONSTANTS.
        /// The total number of characters supported by the font.
        static constexpr unsigned int CHARACTER_COUNT = 256;
        /// The dimension (width or height) of a single glyph, in pixels.
        static constexpr unsigned int GLYPH_MAX_DIMENSION_IN_PIXELS = 16;
        /// The number of glyphs in a single row or column of the font's bitmap.
        static constexpr unsigned int GLYPH_MAX_COUNT_PER_ROW_OR_COLUMN = 16;
        /// The dimension (width or height) of the glyph bitmap, in pixels.
        static constexpr unsigned int FONT_BITMAP_DIMENSION_IN_PIXELS = GLYPH_MAX_DIMENSION_IN_PIXELS * GLYPH_MAX_COUNT_PER_ROW_OR_COLUMN;

        // CONSTRUCTION.
        static std::shared_ptr<Font> LoadSystemDefaultFont(const int font_id);

        // MEMBER VARIABLES.
        /// The glyphs as stored by regular unsigned ASCII character value.
        std::array<Glyph, CHARACTER_COUNT> GlyphsByCharacter = {};
        /// The font's texture.
        std::shared_ptr<Texture> Texture = std::make_shared<GRAPHICS::Texture>();
    };
}
