#include "Objects/Tree.h"

namespace OBJECTS
{
    Tree::Tree(const GRAPHICS::Sprite& sprite) :
    Sprite(sprite)
    {}

    MATH::FloatRectangle Tree::GetWorldBoundingBox() const
    {
        return Sprite.GetWorldBoundingBox();
    }
}
