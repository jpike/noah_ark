#include <stdexcept>
#include "Graphics/Sprite.h"
#include "Graphics/Gui/Glyph.h"
#include "Graphics/Gui/Text.h"

namespace GRAPHICS
{
namespace GUI
{
    Text::Text(
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
        const std::string& characters,
        const MATH::Vector2ui& screen_top_left_position_in_pixels) :
        Font(font),
        Characters(characters),
        ScreenTopLeftPositionInPixels(screen_top_left_position_in_pixels)
    {
        // MAKE SURE A FONT EXISTS.
        bool font_exists = (nullptr != Font);
        if (!font_exists)
        {
            throw std::invalid_argument("Font provided for text must not be null.");
        }
    }

    void Text::Render(sf::RenderTarget& render_target) const
    {
        // CALCULATE THE WORLD COORDINATES OF THE TOP-LEFT STARTING POSITION OF THE TEXT.
        sf::Vector2i text_top_left_position_in_pixels(
            static_cast<int>(ScreenTopLeftPositionInPixels.X),
            static_cast<int>(ScreenTopLeftPositionInPixels.Y));
        sf::Vector2f text_top_left_world_position = render_target.mapPixelToCoords(text_top_left_position_in_pixels);
        MATH::Vector2f current_character_top_left_world_position(text_top_left_world_position.x, text_top_left_world_position.y);

        // RENDER EACH CHARACTER.
        for (const char character : Characters)
        {
            // GET THE GLYPH FOR THE CURRENT CHARACTER.
            Glyph glyph = Font->GetGlyph(character);
            
            // CALCULATE THE CENTER WORLD POSITION OF THE CURRENT GLYPH.
            float glyph_width = glyph.TextureSubRectangle.GetWidth();
            float glyph_half_width = glyph_width / 2.0f;
            float glyph_center_world_x_position = current_character_top_left_world_position.X + glyph_half_width;
            float glyph_height = glyph.TextureSubRectangle.GetHeight();
            float glyph_half_height = glyph_height / 2.0f;
            float glyph_center_world_y_position = current_character_top_left_world_position.Y + glyph_half_height;
            MATH::Vector2f current_glyph_center_world_position(
                glyph_center_world_x_position,
                glyph_center_world_y_position);

            // CREATE A SPRITE FOR THE CURRENT GLYPH.
            Sprite current_character_sprite(
                glyph.Texture,
                glyph.TextureSubRectangle);
            current_character_sprite.SetWorldPosition(current_glyph_center_world_position);

            // RENDER THE CURRENT GLYPH.
            current_character_sprite.Render(render_target);

            // CALCULATE THE TOP-LEFT WORLD POSITION OF THE NEXT CHARACTER.
            current_character_top_left_world_position.X += glyph_width;
        }
    }
}
}
