#include <stdexcept>
#include "Graphics/Font.h"

namespace GRAPHICS
{
    Font::Font(const std::shared_ptr<GRAPHICS::Texture>& texture) :
        Texture(texture)
    {
        // MAKE SURE A TEXTURE EXISTS.
        bool texture_exists = (nullptr != Texture);
        if (!texture_exists)
        {
            throw std::invalid_argument("Texture provided for font must not be null.");
        }
    }

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
        glyph.TextureSubRectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            glyph_left_position,
            glyph_top_position,
            static_cast<float>(Glyph::WIDTH_IN_PIXELS),
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS));
        return glyph;
    }
}
