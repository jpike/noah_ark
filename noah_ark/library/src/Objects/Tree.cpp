#include "Objects/Tree.h"

namespace OBJECTS
{
    Tree::Tree(const GRAPHICS::Sprite& sprite) :
    Sprite(sprite),
    HitPoints(Tree::INITIAL_HIT_POINTS)
    {}

    MATH::FloatRectangle Tree::GetWorldBoundingBox() const
    {
        return Sprite.GetWorldBoundingBox();
    }

    void Tree::TakeHit()
    {
        // CHECK IF THE TREE HAS ANY REMAINING HIT POINTS.
        // Damage should only be taken if hit points remain
        // to prevent numeric wraparound.
        bool hit_points_remain = (HitPoints > 0);
        if (hit_points_remain)
        {
            // TAKE A HIT.
            --HitPoints;
        }
    }
}
