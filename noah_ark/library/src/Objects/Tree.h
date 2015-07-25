#pragma once

#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Sprite.h"
#include "Maps/TileMapObjectDescription.h"
#include "Math/Rectangle.h"
#include "Resources/Assets.h"

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
        // CONSTRUCTION.
        /// Attempts to create a tree.
        /// @param[in]  description - A description of the tree to create.
        ///     It is expected that this description has already been verified
        ///     to be for a tree.
        /// @param[in,out]  assets - The collection of assets from which to
        ///     retrieve graphics for the tree.
        /// @return A tree, if successfully created; null otherwise.
        static std::shared_ptr<Tree> Create(
            const MAPS::TileMapObjectDescription& description,
            RESOURCES::Assets& assets);
        /// Constructor.
        /// @param[in]  sprite - The visual sprite for the tree.
        explicit Tree(const std::shared_ptr<GRAPHICS::Sprite>& sprite);
    
        // GRAPHICS.
        /// Gets the sprite for the tree.
        /// @return The tree's sprite.
        std::shared_ptr<GRAPHICS::Sprite> GetSprite() const;

        // POSITIONING/BOUNDARIES.
        /// Gets the bounding box of the tree, in world coordinates.
        /// @return The tree's bounding box.
        MATH::FloatRectangle GetBoundingBox() const;

    private:

        /// The graphical sprite for the tree.
        std::shared_ptr<GRAPHICS::Sprite> Sprite;
    };
}
