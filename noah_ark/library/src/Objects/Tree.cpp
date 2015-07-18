#include <stdexcept>
#include "Objects/Tree.h"

namespace OBJECTS
{
    std::shared_ptr<Tree> Tree::Create(
        const MAPS::TileMapObjectDescription& description,
        RESOURCES::Assets& assets)
    {
        // GET THE TREE TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> tree_texture = assets.GetTexture(RESOURCES::TREE_TEXTURE_ID);
        bool tree_texture_retrieved = (nullptr != tree_texture);
        if (!tree_texture_retrieved)
        {
            // The tree cannot be created without a valid texture.
            return nullptr;
        }

        // DETERMINE THE SUB-RECTANGLE OF THE TEXTURE TO USE FOR THE TREE.
        // Different sub-rectangles are used depending on the tree's size.
        sf::IntRect tree_texture_sub_rectangle;
        const MATH::Vector2ui SMALL_TREE_DIMENSIONS_IN_PIXELS(16, 16);
        const MATH::Vector2ui TALL_TREE_DIMENSIONS_IN_PIXELS(16, 32);
        const MATH::Vector2ui LARGE_TREE_DIMENSIONS_IN_PIXELS(32, 32);
        MATH::Vector2ui tree_dimensions_in_pixels(description.WidthInPixels, description.HeightInPixels);
        if (SMALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
        {
            sf::IntRect SMALL_TREE_TEXTURE_SUB_RECTANGLE(0, 0, 16, 16);
            tree_texture_sub_rectangle = SMALL_TREE_TEXTURE_SUB_RECTANGLE;
        }
        else if (TALL_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
        {
            sf::IntRect TALL_TREE_TEXTURE_SUB_RECTANGLE(32, 0, 16, 32);
            tree_texture_sub_rectangle = TALL_TREE_TEXTURE_SUB_RECTANGLE;
        }
        else if (LARGE_TREE_DIMENSIONS_IN_PIXELS == tree_dimensions_in_pixels)
        {
            sf::IntRect LARGE_TREE_TEXTURE_SUB_RECTANGLE(0, 16, 32, 32);
            tree_texture_sub_rectangle = LARGE_TREE_TEXTURE_SUB_RECTANGLE;
        }
        else
        {
            // An invalid size was specified for the tree.
            return nullptr;
        }

        // CREATE THE TREE'S SPRITE.
        std::shared_ptr<sf::Sprite> tree_sprite_resource = std::make_shared<sf::Sprite>(
            *tree_texture->GetTextureResource(), 
            tree_texture_sub_rectangle);

        // The center of the sprite should be the center of its visible portion of the texture.
        float tree_local_center_x = static_cast<float>(tree_texture_sub_rectangle.width) / 2.0f;
        float tree_local_center_y = static_cast<float>(tree_texture_sub_rectangle.height) / 2.0f;
        tree_sprite_resource->setOrigin(tree_local_center_x, tree_local_center_y);

        float tree_world_x_position = static_cast<float>(description.TopLeftPositionInPixels.X) + tree_local_center_x;
        float tree_world_y_position = static_cast<float>(description.TopLeftPositionInPixels.Y) + tree_local_center_y;
        tree_sprite_resource->setPosition(tree_world_x_position, tree_world_y_position);

        std::shared_ptr<GRAPHICS::Sprite> tree_sprite = std::make_shared<GRAPHICS::Sprite>(tree_sprite_resource);

        // CREATE THE TREE.
        std::shared_ptr<OBJECTS::Tree> tree = std::make_shared<OBJECTS::Tree>(tree_sprite);

        return tree;
    }

    Tree::Tree(const std::shared_ptr<GRAPHICS::Sprite>& sprite) :
    Sprite(sprite)
    {}

    std::shared_ptr<GRAPHICS::Sprite> Tree::GetSprite() const
    {
        return Sprite;
    }

    MATH::FloatRectangle Tree::GetBoundingBox() const
    {
        return Sprite->GetBoundingBox();
    }
}
