#include "Core/NullChecking.h"
#include "Graphics/Screen.h"

namespace GRAPHICS
{
    /// Constructor to connect the screen to the provided render target.
    /// @param[in]  render_target - The render target for the screen.
    /// @throws std::exception - Thrown if the provided render target is null.
    Screen::Screen(const std::shared_ptr<sf::RenderTarget>& render_target) :
        RenderTarget(render_target)
    {
        CORE::ThrowInvalidArgumentExceptionIfNull(
            render_target,
            "Screen render target cannot be null.");
    }

    /// Clears the screen to black.
    void Screen::Clear()
    {
        RenderTarget->clear();
    }
}
