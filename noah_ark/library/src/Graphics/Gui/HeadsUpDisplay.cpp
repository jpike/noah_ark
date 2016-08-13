#include "Core/NullChecking.h"
#include "Graphics/Gui/HeadsUpDisplay.h"
#include "Graphics/Gui/Text.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Graphics/Sprite.h"

namespace GRAPHICS
{
namespace GUI
{
    /// Constructor.
    /// @param[in]  inventory - The inventory to display in the HUD.
    /// @param[in]  width_in_pixels - The width of the main text box, in pixels.
    /// @param[in]  height_in_pixels - The height of the main text box, in pixels.
    /// @param[in]  font - The font to use for rendering text on the HUD.
    /// @param[in]  axe_texture - The texture to use for rendering an
    ///     axe icon on the HUD.
    /// @param[in]  wood_texture - The texture to use for rendering a
    ///     wood icon on the HUD.
    /// @throws std::exception - Thrown if a parameter is null.
    /// @todo   Re-think how we pass assets to this class.
    HeadsUpDisplay::HeadsUpDisplay(
        const std::shared_ptr<const OBJECTS::Inventory>& inventory,
        const unsigned int main_text_box_width_in_pixels,
        const unsigned int main_text_box_height_in_pixels,
        const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
        const std::shared_ptr<const Texture>& axe_texture,
        const std::shared_ptr<const Texture>& wood_texture) :
    MainTextBox(main_text_box_width_in_pixels, main_text_box_height_in_pixels, font),
    InventoryOpened(false),
    InventoryGui(inventory, font),
    Font(font),
    AxeTexture(axe_texture),
    WoodTexture(wood_texture),
    Inventory(inventory)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(
            Inventory,
            "Null inventory provided to HUD.");
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

    /// Has the HUD respond to the provided key being pressed.
    /// @param[in]  key - The key that was pressed.
    void HeadsUpDisplay::RespondToInput(const sf::Keyboard::Key key)
    {
        // FORWARD INPUT TO THE INVENTORY GUI IF IT IS OPENED.
        if (InventoryOpened)
        {
            InventoryGui.RespondToInput(key);
        }
    }

    /// Renders the HUD to the provided target.
    /// @param[in,out]  screen - The screen to render to.
    void HeadsUpDisplay::Render(GRAPHICS::Screen& screen) const
    {
        // RENDER COMPONENTS INDICATING HOW TO SWING THE AXE.
        // An icon is rendered to help players know which key to press.
        /// @todo   Couple this somehow to the input controller so that it remains in-sync?
        const char SWING_AXE_KEY = 'Z';
        MATH::Vector2ui TOP_LEFT_SCREEN_POSITION_IN_PIXELS(0, 0);
        Renderer::RenderKeyIcon(SWING_AXE_KEY, *Font, TOP_LEFT_SCREEN_POSITION_IN_PIXELS, screen);

        // An axe icon is rendered to help players know what the previously rendered key icon is for.
        /// @todo   This stuff was copied from main.cpp.
        /// Find a way so that it is centralized, not duplicated.
        const float AXE_SPRITE_X_OFFSET_IN_PIXELS = 52.0f;
        const float AXE_SPRITE_Y_OFFSET_IN_PIXELS = 0.0f;
        const float AXE_WIDTH_IN_PIXELS = 11.0f;
        const float AXE_HEIGHT_IN_PIXELS = 14.0f;
        const MATH::FloatRectangle AXE_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromTopLeftAndDimensions(
            AXE_SPRITE_X_OFFSET_IN_PIXELS,
            AXE_SPRITE_Y_OFFSET_IN_PIXELS,
            AXE_WIDTH_IN_PIXELS,
            AXE_HEIGHT_IN_PIXELS);
        // The axe icon should be positioned just to the right of its key icon.
        // Since a single character (glyph) is rendered for the key icon,
        // the width of the icon is the width of a single glyph.
        const unsigned int KEY_ICON_WIDTH_IN_PIXELS = Glyph::WIDTH_IN_PIXELS;
        MATH::Vector2ui axe_icon_screen_position = TOP_LEFT_SCREEN_POSITION_IN_PIXELS;
        axe_icon_screen_position.X += KEY_ICON_WIDTH_IN_PIXELS;
        Renderer::RenderGuiIcon(*AxeTexture, AXE_TEXTURE_SUB_RECTANGLE, axe_icon_screen_position, screen);

        // RENDER COMPONENTS INDICATING HOW MUCH WOOD HAS BEEN COLLECTED.
        // A wood icon is rendered to help players know what the text next to it corresponds to.
        /// @todo   This has been duplicated from Collisions.cpp.
        /// Find a way so that it is centralized, not duplicated.
        const MATH::FloatRectangle WOOD_LOG_TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromTopLeftAndDimensions(
            32.0f, 32.0f, 16.0f, 16.0f);
        // The wood icon should be next to the axe icon but with a little additional blank space
        // so that the two icons aren't too close together.
        const unsigned int PIXEL_BUFFER_SPACE_BETWEEN_AXE_ICON_AND_WOOD_ICON = 16;
        MATH::Vector2ui wood_icon_screen_position = axe_icon_screen_position;
        wood_icon_screen_position.X += static_cast<unsigned int>(AXE_TEXTURE_SUB_RECTANGLE.GetWidth());
        wood_icon_screen_position.X += PIXEL_BUFFER_SPACE_BETWEEN_AXE_ICON_AND_WOOD_ICON;
        Renderer::RenderGuiIcon(*WoodTexture, WOOD_LOG_TEXTURE_SUB_RECTANGLE, wood_icon_screen_position, screen);

        // Text is rendered for "x#" to communicate how much wood has been collected.
        // For example, "x10" (no quotes) would be rendered if the player has collected
        // 10 wood logs.
        const std::string TIMES_COUNT_TEXT = "x";
        std::string wood_count_string = TIMES_COUNT_TEXT + std::to_string(Inventory->WoodCount);
        // This text should be placed just to the right of the wood icon.
        MATH::Vector2ui wood_text_top_left_screen_position_in_pixels(wood_icon_screen_position.X, TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y);
        wood_text_top_left_screen_position_in_pixels.X += static_cast<unsigned int>(WOOD_LOG_TEXTURE_SUB_RECTANGLE.GetWidth());
        Text wood_count_text(Font, wood_count_string, wood_text_top_left_screen_position_in_pixels);
        wood_count_text.Render(screen);

        // RENDER COMPONENTS INDICATING HOW TO OPEN THE INVENTORY.
        // This text is rendered to the far-right of the screen so that its position isn't changed
        // if the space for other GUI elements (like the count of collected wood) changes such
        // that they could distractingly shift the position of this text.
        MATH::Vector2ui TOP_RIGHT_SCREEN_POSITION_IN_PIXELS(
            screen.WidthInPixels<unsigned int>(),
            TOP_LEFT_SCREEN_POSITION_IN_PIXELS.Y);
        const std::string OPEN_INVENTORY_TEXT = "Inventory";
        // One glyph is rendered per character.
        const unsigned int OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS = (Glyph::WIDTH_IN_PIXELS * OPEN_INVENTORY_TEXT.size());
        MATH::Vector2ui open_inventory_text_top_left_screen_position_in_pixels = TOP_RIGHT_SCREEN_POSITION_IN_PIXELS;
        open_inventory_text_top_left_screen_position_in_pixels.X -= OPEN_INVENTORY_TEXT_WIDTH_IN_PIXELS;
        Text open_inventory_text(Font, OPEN_INVENTORY_TEXT, open_inventory_text_top_left_screen_position_in_pixels);
        open_inventory_text.Render(screen);

        // An icon is rendered to help players know which key to press.  It is rendered after
        // the above text for the inventory since it is easier to correctly position here
        // such that it appears just to the left of the text.
        /// @todo   Figure out what key is to be pressed here.  This is just a temporary placeholder.
        const char OPEN_INVENTORY_KEY = 'X';
        MATH::Vector2ui open_inventory_key_text_top_left_screen_position_in_pixels = open_inventory_text_top_left_screen_position_in_pixels;
        open_inventory_key_text_top_left_screen_position_in_pixels.X -= KEY_ICON_WIDTH_IN_PIXELS;
        Renderer::RenderKeyIcon(OPEN_INVENTORY_KEY, *Font, open_inventory_key_text_top_left_screen_position_in_pixels, screen);

        // RENDER THE INVENTORY GUI IF IT IS OPENED.
        if (InventoryOpened)
        {
            InventoryGui.Render(screen);
        }

        // RENDER THE TEXT BOX IF IT IS VISIBLE.
        if (MainTextBox.IsVisible)
        {
            MainTextBox.Render(screen);
        }
    }
}
}
