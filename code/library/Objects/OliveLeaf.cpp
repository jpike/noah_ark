#include "Objects/OliveLeaf.h"

namespace OBJECTS
{
    /// Constructor.
    /// @param[in]  world_position - The position of the olive leaf.
    OliveLeaf::OliveLeaf(const MATH::Vector2f& world_position) :
        Sprite()
    {
        // FINISH CREATING THE SPRITE.
        const MATH::FloatRectangle TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 16, 16, 16);
        GRAPHICS::Sprite sprite(RESOURCES::AssetId::OTHER_OBJECT_TEXTURE, TEXTURE_SUB_RECTANGLE);
        sprite.WorldPosition = world_position;
        // The sprite origin should be the graphical center of its sub-rectangle.
        sprite.Origin = TEXTURE_SUB_RECTANGLE.Center() - TEXTURE_SUB_RECTANGLE.LeftTop;
        // The sprite should be visible by default.
        sprite.IsVisible = true;
        Sprite = sprite;
    }
}
