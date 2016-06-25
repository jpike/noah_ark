#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Texture.h"
#include "Graphics/Gui/Font.h"
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
        /// Constructor.
        /// @param[in]  font - The font to use for rendering text on the HUD.
        /// @param[in]  axe_texture - The texture to use for rendering an
        ///     axe icon on the HUD.
        /// @param[in]  wood_texture - The texture to use for rendering a
        ///     wood icon on the HUD.
        /// @throws std::exception - Thrown if a parameter is null.
        /// @todo   Re-think how we pass assets to this class.
        explicit HeadsUpDisplay(
            const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
            const std::shared_ptr<const Texture>& axe_texture,
            const std::shared_ptr<const Texture>& wood_texture);

        /// Updates the inventory information displayed in the HUD.
        /// @param[in]  inventory - The inventory whose information
        ///     to display in the HUD.
        void Update(const OBJECTS::Inventory& inventory);

        /// Renders the HUD to the provided target.
        /// @param[in]  render_target - The target to render to.
        void Render(sf::RenderTarget& render_target);

    private:
        /// The font to use for rendering text.
        std::shared_ptr<const GRAPHICS::GUI::Font> Font;
        /// The texture to use for rendering an axe icon.
        std::shared_ptr<const Texture> AxeTexture;
        /// The texture to use for rendering a wood icon.
        std::shared_ptr<const Texture> WoodTexture;

        /// The amount of wood collected by the player.
        unsigned int WoodCount;
    };
}
}
