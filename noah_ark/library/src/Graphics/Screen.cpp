#include "Graphics/Screen.h"

namespace GRAPHICS
{
    /// Constructor to connect the screen to the provided render target.
    /// @param[in]  width_in_pixels - The width (in pixels) for the screen.
    /// @param[in]  height_in_pixels - The height (in pixels) for the screen.
    Screen::Screen(const unsigned int width_in_pixels, const unsigned int height_in_pixels) :
    RenderTarget()
    {
        /// @todo   Error handling!
        RenderTarget.create(width_in_pixels, height_in_pixels);
    }

    /// Clears the screen to black.
    void Screen::Clear()
    {
        RenderTarget.clear();
    }
}
