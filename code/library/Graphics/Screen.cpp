#include "Graphics/Screen.h"

namespace GRAPHICS
{
    /// Attempts to create a screen with the provided dimensions.
    /// @param[in]  width_in_pixels - The width (in pixels) for the screen.
    /// @param[in]  height_in_pixels - The height (in pixels) for the screen.
    /// @return The screen, if successfully created; null otherwise.
    std::unique_ptr<Screen> Screen::Create(const unsigned int width_in_pixels, const unsigned int height_in_pixels)
    {
        // CREATE THE RENDER TARGET FOR THE SCREEN.
        std::unique_ptr<Screen> screen = std::make_unique<Screen>();
        bool render_target_created = screen->RenderTarget.create(width_in_pixels, height_in_pixels);
        if (render_target_created)
        {
            return screen;
        }
        else
        {
            return nullptr;
        }
    }

    /// Clears the screen to black.
    void Screen::Clear()
    {
        RenderTarget.clear();
    }
}
