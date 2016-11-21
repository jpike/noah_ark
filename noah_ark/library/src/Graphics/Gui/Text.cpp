#include "Core/NullChecking.h"
#include "Graphics/Sprite.h"
#include "Graphics/Gui/Glyph.h"
#include "Graphics/Gui/Text.h"

namespace GRAPHICS
{
namespace GUI
{
    /// Creates text that is centered on the screen (both horizontally and vertically).
    /// @param[in]  font - The font to use for the text.
    /// @param[in]  characters - The sequence characters in the text.
    /// @throws std::exception - Thrown if the font is null.
    Text Text::CenteredOnScreen(
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
        const std::string& characters,
        const Screen& screen)
    {
        // MAKE SURE A FONT EXISTS.
        CORE::ThrowInvalidArgumentExceptionIfNull(font, "Font provided for text must not be null.");

        // INSERT NEWLINES TO BREAK THE TEXT ONTO MULTIPLE LINES.
        const char NEWLINE = '\n';
        const unsigned int NEWLINE_CHARACTER_COUNT = 1;
        std::string multiline_text = characters;
        unsigned int screen_width_in_pixels = screen.WidthInPixels<unsigned int>();
        unsigned int max_characters_per_line = screen_width_in_pixels / Glyph::WIDTH_IN_PIXELS;
        for (unsigned int character_index = max_characters_per_line; character_index < multiline_text.length(); character_index += max_characters_per_line)
        {
            /// @todo   Handle breaking up based on words.
            multiline_text.insert(character_index, NEWLINE_CHARACTER_COUNT, NEWLINE);
        }

        // CALCULATE THE DIMENSIONS OF THE TEXT.
        /// @todo   Handle multiline and fix these calculations.
        unsigned int text_width_in_pixels = Glyph::WIDTH_IN_PIXELS * multiline_text.length();
        unsigned int line_count = 1;
        while (text_width_in_pixels > screen_width_in_pixels)
        {
            // COUNT THE EXTRA LINE THAT IS NEEDED.
            ++line_count;

            // ACCOUNT FOR THE TEXT THAT HAS BEEN MOVED TO A SEPARATE LINE.
            text_width_in_pixels -= screen_width_in_pixels;
            bool text_shrunk_to_less_than_screen_width = (text_width_in_pixels < screen_width_in_pixels);
            if (text_shrunk_to_less_than_screen_width)
            {
                // The text was already at least as wide as the screen, so ensure that the width
                // doesn't get smaller than that in order to maintain proper centering.
                text_width_in_pixels = screen_width_in_pixels;
            }
        }

        unsigned int text_height_in_pixels = Glyph::HEIGHT_IN_PIXELS * line_count;

        // CALCULATE THE TOP-LEFT POSITION OF THE TEXT.
        unsigned int text_half_width_in_pixels = text_width_in_pixels / 2;
        unsigned int text_half_height_in_pixels = text_height_in_pixels / 2;

        unsigned int screen_center_x_position = screen_width_in_pixels / 2;
        unsigned int screen_center_y_position = screen.HeightInPixels<unsigned int>() / 2;

        unsigned int text_left_screen_position = screen_center_x_position - text_half_width_in_pixels;
        unsigned int text_top_screen_position = screen_center_y_position - text_half_height_in_pixels;

        MATH::Vector2ui text_top_left_screen_position(text_left_screen_position, text_top_screen_position);

        // CREATE THE TEXT.
        Text text(font, multiline_text, text_top_left_screen_position);
        return text;
    }

    /// Constructor.
    /// @param[in]  font - The font to use for the text.
    /// @param[in]  characters - The sequence characters in the text.
    /// @param[in]  screen_top_left_position_in_pixels - The top-left screen position
    ///     of where the text should be rendered, in units of pixels.
    /// @throws std::exception - Thrown if the font is null.
    Text::Text(
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
        const std::string& characters,
        const MATH::Vector2ui& screen_top_left_position_in_pixels) :
        Font(font),
        Characters(characters),
        ScreenTopLeftPositionInPixels(screen_top_left_position_in_pixels)
    {
        // MAKE SURE A FONT EXISTS.
        CORE::ThrowInvalidArgumentExceptionIfNull(font, "Font provided for text must not be null.");
    }

    /// Renders the text to the specified render target.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void Text::Render(Renderer& renderer) const
    {
        // CALCULATE THE WORLD COORDINATES OF THE TOP-LEFT STARTING POSITION OF THE TEXT.
        sf::Vector2i text_top_left_position_in_pixels(
            static_cast<int>(ScreenTopLeftPositionInPixels.X),
            static_cast<int>(ScreenTopLeftPositionInPixels.Y));
        /// @todo   Pass default identity transform to render target to render in screen space?
        /// http://www.sfml-dev.org/tutorials/2.4/graphics-transform.php#custom-transforms
        sf::Vector2f text_top_left_world_position = renderer.Screen.RenderTarget->mapPixelToCoords(text_top_left_position_in_pixels);
        MATH::Vector2f current_character_top_left_world_position(text_top_left_world_position.x, text_top_left_world_position.y);

        GRAPHICS::Color black;
        renderer.RenderText(Characters, current_character_top_left_world_position, black);

        // RENDER EACH CHARACTER.
        /*for (const char character : Characters)
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
            screen.RenderTarget->draw(current_character_sprite.SpriteResource, render_states);

            // CALCULATE THE TOP-LEFT WORLD POSITION OF THE NEXT CHARACTER.
            current_character_top_left_world_position.X += glyph_width;
        }*/
    }

    /// Gets the width of the text, in pixels.
    /// @return The width of the text, in pixels.
    unsigned int Text::GetWidthInPixels() const
    {
        unsigned int width_in_pixels = Glyph::WIDTH_IN_PIXELS * Characters.size();
        return width_in_pixels;
    }
}
}
