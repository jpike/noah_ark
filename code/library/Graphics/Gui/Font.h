#pragma once

#include <memory>
#include "Graphics/Texture.h"
#include "Graphics/Gui/Glyph.h"

namespace GRAPHICS
{
/// Holds code related to graphical user interfaces.
namespace GUI
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
        explicit Font(const std::shared_ptr<Texture>& texture);

        Glyph GetGlyph(const char character) const;

    private:
        /// The font's texture.
        std::shared_ptr<Texture> Texture = nullptr;
    };
}
}
