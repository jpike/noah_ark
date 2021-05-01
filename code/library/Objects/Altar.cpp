#include "Objects/Altar.h"
#include "Resources/AssetId.h"

namespace OBJECTS
{
    /// Creates a present.
    /// @param[in]  center_world_position - The center world position of the present.
    Altar::Altar(const MATH::Vector2f& center_world_position) :
        Sprite()
    {
        // FINISH CREATING THE SPRITE.
        const MATH::FloatRectangle TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(16, 0, 32, 16);
        GRAPHICS::Sprite sprite(RESOURCES::AssetId::OTHER_OBJECT_TEXTURE, TEXTURE_SUB_RECTANGLE);
        sprite.WorldPosition = center_world_position;
        // The sprite origin should be the graphical center of its sub-rectangle.
        sprite.Origin = TEXTURE_SUB_RECTANGLE.Center();
        // The sprite should be visible by default.
        sprite.IsVisible = true;
        Sprite = sprite;
    }
}
