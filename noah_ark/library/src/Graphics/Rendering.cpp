#include "Graphics/Rendering.h"

namespace GRAPHICS
{
    void Render(const GRAPHICS::GUI::Text& text, sf::RenderTarget& render_target)
    {
        text.Render(render_target);
    }

    void RenderScreenRectangle(
        const MATH::FloatRectangle& rectangle,
        const GRAPHICS::Color& color,
        sf::RenderTarget& render_target)
    {
        // CONVERT THE RECTANGLE POSITION TO A WORLD POSITION.
        // This is necessary so that the rectangle can be rendered
        // appropriately on the screen regardless of how the camera
        // might move around the world.
        int left_screen_position = static_cast<int>(rectangle.GetLeftXPosition());
        int top_screen_position = static_cast<int>(rectangle.GetTopYPosition());
        sf::Vector2f top_left_world_position = render_target.mapPixelToCoords(sf::Vector2i(
            left_screen_position,
            top_screen_position));

        // CREATE THE RECTANGLE TO RENDER.
        sf::RectangleShape renderable_rectangle;
        renderable_rectangle.setFillColor(sf::Color(color.Red, color.Green, color.Blue));

        float width = rectangle.GetWidth();
        float height = rectangle.GetHeight();
        renderable_rectangle.setSize(sf::Vector2f(width, height));

        renderable_rectangle.setPosition(top_left_world_position);

        // RENDER THE RECTANGLE.
        render_target.draw(renderable_rectangle);
    }

    void RenderKeyIcon(
        const char key,
        const GRAPHICS::GUI::Font& font,
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        sf::RenderTarget& render_target)
    {
        // CONVERT THE SCREEN POSITION TO A WORLD POSITION.
        // This is necessary so that the key icon can be rendered
        // appropriately on the screen regardless of how the camera
        // might move around the world.
        sf::Vector2f top_left_world_position = render_target.mapPixelToCoords(sf::Vector2i(
            top_left_screen_position_in_pixels.X,
            top_left_screen_position_in_pixels.Y));

        // CREATE A RECTANGLE TO RESEMBLE A KEY ON A KEYBOARD.
        const sf::Color DARK_GRAY(128, 128, 128);
        const sf::Color LIGHT_GRAY(176, 176, 176);
        sf::RectangleShape key_background_icon;
        key_background_icon.setFillColor(LIGHT_GRAY);
        key_background_icon.setOutlineColor(DARK_GRAY);
        key_background_icon.setOutlineThickness(2.0f);
        key_background_icon.setSize(sf::Vector2f(
            static_cast<float>(GRAPHICS::GUI::Glyph::WIDTH_IN_PIXELS),
            static_cast<float>(GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS)));
        key_background_icon.setPosition(top_left_world_position);

        // RENDER THE BACKGROUND RECTANGLE FOR THE KEY.
        render_target.draw(key_background_icon);

        // GET THE GLYPH FOR THE KEY.
        GRAPHICS::GUI::Glyph glyph = font.GetGlyph(key);

        // CALCULATE THE CENTER WORLD POSITION OF THE GLYPH.
        float glyph_width = glyph.TextureSubRectangle.GetWidth();
        float glyph_half_width = glyph_width / 2.0f;
        float glyph_center_world_x_position = top_left_world_position.x + glyph_half_width;
        float glyph_height = glyph.TextureSubRectangle.GetHeight();
        float glyph_half_height = glyph_height / 2.0f;
        float glyph_center_world_y_position = top_left_world_position.y + glyph_half_height;
        MATH::Vector2f glyph_center_world_position(
            glyph_center_world_x_position,
            glyph_center_world_y_position);

        // CREATE A SPRITE FOR THE GLYPH.
        Sprite key_character_sprite(
            glyph.Texture,
            glyph.TextureSubRectangle);
        key_character_sprite.SetWorldPosition(glyph_center_world_position);

        // RENDER THE GLYPH FOR THE KEY.
        key_character_sprite.Render(render_target);
    }

    void RenderGuiIcon(
        const GRAPHICS::Texture& texture,
        const MATH::FloatRectangle& texture_sub_rectangle,
        const MATH::Vector2ui& top_left_screen_position_in_pixels,
        sf::RenderTarget& render_target)
    {
        // CREATE A SPRITE FOR THE ICON USING THE TEXTURE INFORMATION.
        sf::IntRect texture_rectangle;
        texture_rectangle.top = static_cast<int>(texture_sub_rectangle.GetTopYPosition());
        texture_rectangle.left = static_cast<int>(texture_sub_rectangle.GetLeftXPosition());
        texture_rectangle.width = static_cast<int>(texture_sub_rectangle.GetWidth());
        texture_rectangle.height = static_cast<int>(texture_sub_rectangle.GetHeight());
        sf::Sprite gui_icon(texture.TextureResource, texture_rectangle);

        // POSITION THE GUI ICON SPRITE.
        // The screen position must be converted to a world position so that the GUI icon
        // can be rendered appropriately on screen regardless of how the camera might
        // move around the world.
        sf::Vector2f top_left_world_position = render_target.mapPixelToCoords(sf::Vector2i(
            top_left_screen_position_in_pixels.X,
            top_left_screen_position_in_pixels.Y));
        gui_icon.setPosition(top_left_world_position);

        // RENDER THE GUI ICON.
        render_target.draw(gui_icon);
    }
}
