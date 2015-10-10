#include "Objects/Tree.h"

namespace OBJECTS
{
    Tree::Tree(const GRAPHICS::Sprite& sprite) :
    Sprite(sprite)
    {}

    MATH::FloatRectangle Tree::GetBoundingBox() const
    {
        return Sprite.GetBoundingBox();
    }
}
