#include <stdexcept>
#include "Objects/Tree.h"

namespace OBJECTS
{
    Tree::Tree(
        const MAPS::TileMapObjectDescription& description,
        GRAPHICS::GraphicsSystem& graphics_system) :
    Sprite()
    {
        /// @todo   We need to figure out how to determine the sprite.
        /*std::shared_ptr<GRAPHICS::Texture> tree_texture = graphics_system.GetTexture("tree_texture");
        bool texture_retrieved = (nullptr != tree_texture);
        if (!texture_retrieved)
        {
            throw std::runtime_error("Failed to get texture for tree.");
        }

        float x_position_in_texels = 0;
        float y_position_in_texels = 0;
        float width_in_texels = 16;
        float height_in_texels = 16;
        Sprite = GRAPHICS::Sprite::Create(
            tree_texture,
            x_position_in_texels,
            y_position_in_texels,
            width_in_texels,
            height_in_texels); */
    }
}
