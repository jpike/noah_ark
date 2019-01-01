#include "Core/NullChecking.h"
#include "Graphics/Gui/Font.h"

namespace GRAPHICS
{
namespace GUI
{
    /// Creates a font to use the specified texture.
    /// @param[in]  texture - The texture to use for the font.
    /// @throws std::exception - Thrown if the texture is null.
    Font::Font(const std::shared_ptr<GRAPHICS::Texture>& texture) :
        Texture(texture)
    {
        // MAKE SURE A TEXTURE EXISTS.
        CORE::ThrowInvalidArgumentExceptionIfNull(Texture, "Texture provided for font must not be null.");       
    }

    /// Gets the glyph for the specified character.
    /// @param[in]  character - The character whose glyph to get.
    /// @return     The glyph for the character.
    Glyph Font::GetGlyph(const char character) const
    {
        Glyph glyph;
        glyph.Texture = Texture;
        glyph.TextureSubRectangle = Glyph::TEXTURE_SUB_RECTANGLES[character];
        return glyph;
    }
}
}
