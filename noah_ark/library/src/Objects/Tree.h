#pragma once

#include "Core/Direction.h"
#include "Graphics/Sprite.h"
#include "Math/Rectangle.h"

namespace OBJECTS
{
    /// A tree that can be placed in the overworld.
    /// Trees will provide a source of wood and potentially fruit.
    /// Types of supported trees:
    /// - Small trees (16x16)
    /// - Tall trees (16x32)
    /// - Large trees (32x32)
    class Tree
    {
    public:
        // STATIC CONSTANTS.
        /// The initial number of hit points a tree has.
        static const unsigned int INITIAL_HIT_POINTS = 3;

        // CONSTRUCTION.
        /// Constructor.
        /// @param[in]  sprite - The visual sprite for the tree.
        explicit Tree(const GRAPHICS::Sprite& sprite);

        // POSITIONING/BOUNDARIES.
        /// Gets the bounding box of the tree, in world coordinates.
        /// @return The tree's bounding box.
        MATH::FloatRectangle GetWorldBoundingBox() const;

        // HITTING.
        /// Damages the tree by taking one hit.
        void TakeHit();
        /// Starts shaking the tree.  Intended
        /// for whenever the tree gets hit.
        void StartShaking();

        // UPDATING.
        /// Updates the shaking of the tree, if the tree is currently shaking.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time for which to
        ///     update the tree's shaking.
        void Update(const float elapsed_time_in_seconds);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the tree.
        GRAPHICS::Sprite Sprite;
        /// The number of hit points the tree currently has.
        unsigned int HitPoints;

    private:
        /// True if the tree is currently shaking; false otherwise.
        bool Shaking;
        /// True if the tree is currently shaking outward; false otherwise.
        bool ShakingOut;
        /// The current rotation angle for the tree's sprite, if the tree is currently shaking.
        float CurrentShakeRotationInDegrees;
        /// The current elapsed time for the tree shaking outward, if the tree is currently shaking outward.
        float CurrentShakeOutElapsedTimeInSeconds;
        /// The current elapsed time for the tree shaking inward, if the tree is currently shaking inward.
        float CurrentShakeInElapsedTimeInSeconds;
        /// The current direction for the tree shaking outward, if the tree is currently shaking.
        /// Only left and right are valid.  The shaking in direction is opposite of this direction.
        CORE::Direction ShakeOutDirection;
        /// The initial shake out direction for a tree shake, if the tree is currently shaking.
        /// Only left and right are valid.  This is tracked separately to allow the tree to
        /// shake in both directions for a given shake.
        CORE::Direction InitialShakeOutDirection;
    };
}
