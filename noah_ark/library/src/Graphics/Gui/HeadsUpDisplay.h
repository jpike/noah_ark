#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Gui/Font.h"
#include "Graphics/Gui/InventoryGui.h"
#include "Graphics/Gui/TextBox.h"
#include "Graphics/Screen.h"
#include "Graphics/Texture.h"
#include "Objects/Inventory.h"

namespace GRAPHICS
{
namespace GUI
{
    /// The main heads-up-display (HUD) of the game.
    /// Responsible for displaying helpful information
    /// to the player, including information about the
    /// player's inventory.
    class HeadsUpDisplay
    {
    public:
        // CONSTRUCTION.
        explicit HeadsUpDisplay(
            const std::shared_ptr<const OBJECTS::Inventory>& inventory,
            const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
            const std::shared_ptr<const Texture>& axe_texture,
            const std::shared_ptr<const Texture>& wood_texture);

        // INPUT.
        void RespondToInput(const sf::Keyboard::Key key);

        // RENDERING.
        void Render(GRAPHICS::Screen& screen) const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The main text box for displaying messages to the player.
        GRAPHICS::GUI::TextBox MainTextBox;
        /// True if the full inventory GUI is displayed; false otherwise.
        bool InventoryOpened;
        /// The GUI for displaying the full inventory.
        GRAPHICS::GUI::InventoryGui InventoryGui;

    private:
        // PRIVATE MEMBER VARIABLES.
        /// The font to use for rendering text.
        std::shared_ptr<const GRAPHICS::GUI::Font> Font;
        /// The texture to use for rendering an axe icon.
        std::shared_ptr<const Texture> AxeTexture;
        /// The texture to use for rendering a wood icon.
        std::shared_ptr<const Texture> WoodTexture;
        
        /// The player's inventory to display in the HUD.
        std::shared_ptr<const OBJECTS::Inventory> Inventory;
    };
}
}
