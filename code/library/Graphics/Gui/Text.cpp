#include "Graphics/Gui/Glyph.h"
#include "Graphics/Gui/Text.h"
#include "Math/Rectangle.h"

namespace GRAPHICS::GUI
{
    /// Gets the x position of the right side of the text's bounding box.
    /// @return The right position of the text's bounding box.
    float Text::RightPosition() const
    {
        float width = Width<float>(String, ScaleFactor);
        float right_position = LeftTopPosition.X + width;
        return right_position;
    }

    /// Gets the y position of the bottom side of the text's bounding box.
    /// @return The bottom position of the text's bounding box.
    float Text::BottomPosition() const
    {
        float text_height = Glyph::HeightInPixels<float>(ScaleFactor);
        float bottom_position = LeftTopPosition.Y + text_height;
        return bottom_position;
    }
}
