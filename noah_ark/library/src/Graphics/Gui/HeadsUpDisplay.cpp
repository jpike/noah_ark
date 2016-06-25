#include <stdexcept>
#include "Core/NullChecking.h"
#include "Graphics/Screen.h"
#include "Graphics/Sprite.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Gui/Text.h"

namespace GRAPHICS
{
namespace GUI
{
    HeadsUpDisplay::HeadsUpDisplay(
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
        const std::shared_ptr<const Texture>& axe_texture,
        const std::shared_ptr<const Texture>& wood_texture) :
    Font(font),
    AxeTexture(axe_texture),
    WoodTexture(wood_texture),
    WoodCount(0)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Font,
            "Null font provided to HUD.");
        CORE::ThrowInvalidArgumentExceptionIfNull(
            AxeTexture,
            "Null axe texture provided to HUD.");
        CORE::ThrowInvalidArgumentExceptionIfNull(
            WoodTexture,
            "Null wood texture provided to HUD.");
    }

    void HeadsUpDisplay::Update(const OBJECTS::Inventory& inventory)
    {
        WoodCount = inventory.WoodCount;
    }

    void HeadsUpDisplay::Render(sf::RenderTarget& render_target)
    {
        // CREATE A RECTANGLE TO RESEMBLE A KEY ON A KEYBOARD.
        const sf::Color DARK_GRAY(128, 128, 128);
        const sf::Color LIGHT_GRAY(176, 176, 176);
        sf::RectangleShape key_background_icon;
        key_background_icon.setFillColor(LIGHT_GRAY);
        key_background_icon.setOutlineColor(DARK_GRAY);
        key_background_icon.setOutlineThickness(2.0f);
        key_background_icon.setSize(sf::Vector2f(
            static_cast<float>(Glyph::WIDTH_IN_PIXELS), 
            static_cast<float>(Glyph::HEIGHT_IN_PIXELS)));

        // RENDER COMPONENTS INDICATING HOW TO SWING THE AXE.
        MATH::Vector2ui TOP_LEFT_SCREEN_POSITION_IN_PIXELS(0, 0);
        key_background_icon.setPosition(
            static_cast<float>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.X),
            static_cast<float>(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y));
        render_target.draw(key_background_icon);
        Text axe_text(Font, "Z", TOP_LEFT_SCREEN_POSITION_IN_PIXELS);
        axe_text.Render(render_target);

        /// @todo   This stuff was copied from main.cpp.
        /// Find a way so that it is centralized, not duplicated.
        const float AXE_SPRITE_X_OFFSET_IN_PIXELS = 52.0f;
        const float AXE_SPRITE_Y_OFFSET_IN_PIXELS = 0.0f;
        const float AXE_WIDTH_IN_PIXELS = 11.0f;
        const float AXE_HEIGHT_IN_PIXELS = 14.0f;
        MATH::FloatRectangle axe_texture_sub_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            AXE_SPRITE_X_OFFSET_IN_PIXELS,
            AXE_SPRITE_Y_OFFSET_IN_PIXELS,
            AXE_WIDTH_IN_PIXELS,
            AXE_HEIGHT_IN_PIXELS);
        Sprite axe_sprite(AxeTexture, axe_texture_sub_rectangle);
        sf::Vector2i axe_text_screen_position(TOP_LEFT_SCREEN_POSITION_IN_PIXELS.X, TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y);
        sf::Vector2f axe_sprite_world_position = render_target.mapPixelToCoords(axe_text_screen_position);
        axe_sprite_world_position.x += axe_text.GetWidthInPixels();
        axe_sprite_world_position.x += axe_texture_sub_rectangle.GetWidth() / 2.0f;
        axe_sprite_world_position.y += axe_texture_sub_rectangle.GetHeight() / 2.0f;
        MATH::Vector2f axe_icon_world_position(axe_sprite_world_position.x, axe_sprite_world_position.y);
        axe_sprite.SetWorldPosition(axe_icon_world_position);
        axe_sprite.Render(render_target);

        // RENDER COMPONENTS INDICATING HOW MUCH WOOD HAS BEEN COLLECTED.
        /// @todo   This has been duplicated from Collisions.cpp.
        /// Find a way so that it is centralized, not duplicated.
        const MATH::FloatRectangle WOOD_LOG_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromTopLeftAndDimensions(
            32.0f, 32.0f, 16.0f, 16.0f);
        GRAPHICS::Sprite wood_log_sprite(WoodTexture, WOOD_LOG_TEXTURE_SUB_RECTANGLE);

        MATH::Vector2f wood_log_sprite_world_position = axe_icon_world_position;
        wood_log_sprite_world_position.X += axe_sprite.GetWidthInPixels();
        wood_log_sprite_world_position.X += axe_text.GetWidthInPixels();
        wood_log_sprite_world_position.X += WOOD_LOG_TEXTURE_SUB_RECTANGLE.GetWidth() / 2.0f;
        wood_log_sprite.SetWorldPosition(wood_log_sprite_world_position);
        wood_log_sprite.Render(render_target);

        std::string wood_count_string = "x" + std::to_string(WoodCount);
        sf::Vector2i wood_icon_screen_position = render_target.mapCoordsToPixel(
            sf::Vector2f(wood_log_sprite_world_position.X, wood_log_sprite_world_position.Y));
        MATH::Vector2ui wood_text_top_left_screen_position_in_pixels(wood_icon_screen_position.x, TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y);
        wood_text_top_left_screen_position_in_pixels.X += static_cast<unsigned int>(wood_log_sprite.GetWidthInPixels());
        Text wood_count_text(Font, wood_count_string, wood_text_top_left_screen_position_in_pixels);
        wood_count_text.Render(render_target);

        // RENDER COMPONENTS INDICATING HOW TO OPEN THE INVENTORY.
        MATH::Vector2ui TOP_RIGHT_SCREEN_POSITION_IN_PIXELS(
            GRAPHICS::Screen::WIDTH_IN_PIXELS,
            TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y);
        const std::string OPEN_INVENTORY_TEXT = "Inventory";
        const unsigned int OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS = (Glyph::WIDTH_IN_PIXELS * OPEN_INVENTORY_TEXT.size());
        MATH::Vector2ui open_inventory_text_top_left_screen_position_in_pixels = TOP_RIGHT_SCREEN_POSITION_IN_PIXELS;
        open_inventory_text_top_left_screen_position_in_pixels.X -= OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS;
        Text open_inventory_text(Font, OPEN_INVENTORY_TEXT, open_inventory_text_top_left_screen_position_in_pixels);
        open_inventory_text.Render(render_target);

        /// @todo   Figure out what key is to be pressed.
        MATH::Vector2ui open_inventory_key_text_top_left_screen_position_in_pixels = open_inventory_text_top_left_screen_position_in_pixels;
        open_inventory_key_text_top_left_screen_position_in_pixels.X -= Glyph::WIDTH_IN_PIXELS;
        key_background_icon.setPosition(
            static_cast<float>(open_inventory_key_text_top_left_screen_position_in_pixels.X),
            static_cast<float>(open_inventory_key_text_top_left_screen_position_in_pixels.Y));
        render_target.draw(key_background_icon);
        Text open_inventory_key_text(Font, "X", open_inventory_key_text_top_left_screen_position_in_pixels);
        open_inventory_key_text.Render(render_target);
    }
}
}
