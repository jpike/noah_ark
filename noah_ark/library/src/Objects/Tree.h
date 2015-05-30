#pragma once

#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Graphics/Sprite.h"
#include "Maps/TileMapObjectDescription.h"

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
        /// Constructor.
        /// @param[in]  description - A description of the tree to create.
        /// @param[in,out]  graphics_system - The graphics system supplying
        ///     graphical resources for the tree.
        explicit Tree(
            const MAPS::TileMapObjectDescription& description,
            GRAPHICS::GraphicsSystem& graphics_system);
    
    private:

        std::shared_ptr<GRAPHICS::Sprite> Sprite;
    };
}
