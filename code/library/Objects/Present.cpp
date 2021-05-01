#include "Objects/Present.h"
#include "Resources/AssetId.h"

namespace OBJECTS
{
    /// Creates a present.
    /// @param[in]  center_world_position - The center world position of the present.
    /// @param[in]  bible_verse - The Bible verse contained within the present.
    Present::Present(const MATH::Vector2f& center_world_position, const BIBLE::BibleVerse& bible_verse) :
        Sprite(),
        BibleVerse(bible_verse)
    {
        // FINISH CREATING THE SPRITE.
        const MATH::FloatRectangle TEXTURE_SUB_RECTANGLE = MATH::FloatRectangle::FromLeftTopAndDimensions(0, 0, 16, 16);
        GRAPHICS::Sprite sprite(RESOURCES::AssetId::OTHER_OBJECT_TEXTURE, TEXTURE_SUB_RECTANGLE);
        sprite.WorldPosition = center_world_position;
        // The sprite origin should be the graphical center of its sub-rectangle.
        sprite.Origin = TEXTURE_SUB_RECTANGLE.Center();
        // The sprite should be visible by default.
        sprite.IsVisible = true;
        Sprite = sprite;
    }
}
