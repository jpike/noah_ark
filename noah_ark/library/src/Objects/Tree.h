#pragma once

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
        /// Damages the tree by taking a hit.
        void TakeHit();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the tree.
        GRAPHICS::Sprite Sprite;
        /// The number of hit points the tree currently has.
        unsigned int HitPoints;
    };
}
