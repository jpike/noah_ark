#include "Objects/Tree.h"

namespace OBJECTS
{
    /// Gets the bounding box of the tree, in world coordinates.
    /// @return The tree's bounding box.
    MATH::FloatRectangle Tree::GetWorldBoundingBox() const
    {
        return Sprite.GetWorldBoundingBox();
    }

    /// Gets the center world position of the tree's trunk.
    /// @return The center world position of the tree's trunk.
    MATH::Vector2f Tree::GetTrunkCenterWorldPosition() const
    {
        // GET THE BOUNDING BOX OF THE TREE.
        MATH::FloatRectangle world_bounding_box = GetWorldBoundingBox();

        // CALCULATE THE CENTER OF THE TREE'S TRUNK.
        // It is vertically placed halfway up the pixel grid.
        const float OFFSET_FROM_TREE_BOTTOM_TO_TRUNK_CENTER_IN_PIXELS = 8.0f;
        float tree_bottom_world_y_position = world_bounding_box.GetBottomYPosition();
        // Y gets smaller going up.
        float trunk_center_y_position = tree_bottom_world_y_position - OFFSET_FROM_TREE_BOTTOM_TO_TRUNK_CENTER_IN_PIXELS;
        float trunk_center_x_position = world_bounding_box.GetCenterXPosition();
        MATH::Vector2f trunk_center_world_position(
            trunk_center_x_position,
            trunk_center_y_position);
        return trunk_center_world_position;
    }

    /// Damages the tree by taking one hit.
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

    /// Starts shaking the tree.  Intended
    /// for whenever the tree gets hit.
    void Tree::StartShaking()
    {
        // START ANIMATING A NEW SHAKE.
        Sprite.UseAnimationSequence("TreeShake");
        Sprite.Play();
    }

    /// Updates the shaking of the tree, if the tree is currently shaking.
    /// @param[in]  elapsed_time - The elapsed time for which to
    ///     update the tree's shaking.
    void Tree::Update(const sf::Time& elapsed_time)
    {
        Sprite.Update(elapsed_time);
    }

    /// Checks if the tree is currently shaking or not.
    /// @return True if the tree is currently shaking; false otherwise.
    bool Tree::IsShaking() const
    {
        bool is_shaking = Sprite.IsAnimating();
        return is_shaking;
    }
}
