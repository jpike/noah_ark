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
        // CALCULATE THE TOP-LEFT COORDINATES OF THE GLYPH.
        const char CHARACTERS_PER_ROW_COUNT = 16;
        char character_horizontal_index = character % CHARACTERS_PER_ROW_COUNT;
        float glyph_left_position = static_cast<float>(character_horizontal_index * Glyph::WIDTH_IN_PIXELS);
        
        char character_vertical_index = character / CHARACTERS_PER_ROW_COUNT;
        float glyph_top_position = static_cast<float>(character_vertical_index * Glyph::HEIGHT_IN_PIXELS);

        // RETURN THE GLYPH.
        Glyph glyph;
        glyph.Texture = Texture;
        glyph.TextureSubRectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            glyph_left_position,
            glyph_top_position,
            static_cast<float>(Glyph::WIDTH_IN_PIXELS),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));
        return glyph;
    }
}
}
