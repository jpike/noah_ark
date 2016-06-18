#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Graphics/Gui/Font.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
namespace GUI
{
    /// A graphical representation of text suitable for rendering.
    class Text
    {
    public:
        /// Constructor.
        /// @param[in]  font - The font to use for the text.
        /// @param[in]  characters - The sequence characters in the text.
        /// @param[in]  screen_top_left_position_in_pixels - The top-left screen position
        ///     of where the text should be rendered, in units of pixels.
        /// @throws std::exception - Thrown if the font is null.
        explicit Text(
            const std::shared_ptr<const GRAPHICS::GUI::Font>& font, 
            const std::string& characters,
            const MATH::Vector2ui& screen_top_left_position_in_pixels);

        /// Renders the text to the specified render target.
        /// @param[in,out]  render_target - The target to render to.
        void Render(sf::RenderTarget& render_target) const;

    private:
        /// The font for rendering the text.
        std::shared_ptr<const GRAPHICS::GUI::Font> Font = nullptr;
        /// The sequence of characters in the text.
        std::string Characters = "";
        /// THe top-left screen position of where the text should be rendered, in units of pixels.
        MATH::Vector2ui ScreenTopLeftPositionInPixels = MATH::Vector2ui();
    };
}
}
