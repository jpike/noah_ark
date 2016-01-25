#include "Objects/WoodLogs.h"

namespace OBJECTS
{
    MATH::FloatRectangle WoodLogs::GetWorldBoundingBox() const
    {
        // The bounding box is shrunk some from its visible representation.
        // This helps avoid an issue where the wood logs already disappear
        // when Noah is too close to them when chopping down a tree.
        MATH::FloatRectangle sprite_bounding_box = Sprite.GetWorldBoundingBox();

        /// @todo   Think about removing this method since it doesn't fully resolve the problem.
        const float BOX_DIMENSION_SCALE_FACTOR = 0.3f;
        float wood_logs_width = sprite_bounding_box.GetWidth() * BOX_DIMENSION_SCALE_FACTOR;
        float wood_logs_height = sprite_bounding_box.GetHeight() * BOX_DIMENSION_SCALE_FACTOR;

        MATH::FloatRectangle wood_logs_bounding_box = MATH::FloatRectangle::FromCenterAndDimensions(
            sprite_bounding_box.GetCenterXPosition(),
            sprite_bounding_box.GetCenterYPosition(),
            wood_logs_width,
            wood_logs_height);
        return wood_logs_bounding_box;
    }
}
