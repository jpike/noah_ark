#include "Graphics/Color.h"

namespace GRAPHICS
{
    const Color Color::BLACK(0, 0, 0);
    const Color Color::BROWN(128, 64, 0);
    const Color Color::GREEN(0, 255, 0);
    const Color Color::ORANGE(255, 127, 0);
    const Color Color::RED(255, 0, 0);
    const Color Color::RED_BROWN(128, 32, 0);
    const Color Color::WHITE(255, 255, 255);
    const Color Color::YELLOW(255, 255, 0);

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

    /// Scales the red, green, and blue color components down
    /// by dividing by the provided scale factor.
    /// No protection against truncation is performed.
    /// @param[in]  scale_factor - The amount to scale down
    ///     the red, green, and blue components.
    void Color::ScaleRgbDown(const uint8_t scale_factor)
    {
        Red /= scale_factor;
        Green /= scale_factor;
        Blue /= scale_factor;
    }
}
