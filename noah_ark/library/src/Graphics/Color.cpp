#include "Graphics/Color.h"

namespace GRAPHICS
{
    const Color Color::BLACK(0, 0, 0);
    const Color Color::BROWN(128, 64, 0);
    const Color Color::GREEN(0, Color::MAX_COLOR_COMPONENT, 0);
    const Color Color::RED(Color::MAX_COLOR_COMPONENT, 0, 0);
    const Color Color::RED_BROWN(128, 32, 0);
    const Color Color::WHITE(Color::MAX_COLOR_COMPONENT, Color::MAX_COLOR_COMPONENT, Color::MAX_COLOR_COMPONENT);
    const Color Color::YELLOW(Color::MAX_COLOR_COMPONENT, Color::MAX_COLOR_COMPONENT, 0);

    /// Constructor.
    /// @param[in]  red - The red component of the color.
    /// @param[in]  green - The green component of the color.
    /// @param[in]  blue - The blue component of the color.
    /// @param[in]  alpha - The alpha component of the color.
    Color::Color(
        const uint8_t red,
        const uint8_t green,
        const uint8_t blue,
        const uint8_t alpha) :
    Red(red),
    Green(green),
    Blue(blue),
    Alpha(alpha)
    {}
}
