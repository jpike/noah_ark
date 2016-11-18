#pragma once

#include <memory>
#include <string>
#include "Graphics/Gui/Font.h"
#include "Graphics/Renderer.h"
#include "Graphics/Screen.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
namespace GUI
{
    /// A graphical representation of text suitable for rendering.
    class Text
    {
    public:
        static Text CenteredOnScreen(
            const std::shared_ptr<const GRAPHICS::GUI::Font>& font,
            const std::string& characters,
            const Screen& screen);
        explicit Text(
            const std::shared_ptr<const GRAPHICS::GUI::Font>& font, 
            const std::string& characters,
            const MATH::Vector2ui& screen_top_left_position_in_pixels);

        void Render(Renderer& renderer, Screen& screen) const;

        unsigned int GetWidthInPixels() const;

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
