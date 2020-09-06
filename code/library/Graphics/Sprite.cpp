#include "Graphics/Sprite.h"

namespace GRAPHICS
{
    /// Creates an invisible sprite based on the provided texture information.
    /// @param[in]  texture_id - The ID of the texture containing graphics for the sprite.
    /// @param[in]  texture_sub_rectangle - The sub-rectangle of the texture
    /// defining which portion should be used for the sprite.
    Sprite::Sprite(
        const RESOURCES::AssetId texture_id,
        const MATH::FloatRectangle& texture_sub_rectangle) :
    IsVisible(false),
    WorldPosition(),
    TextureId(texture_id),
    TextureSubRectangle(texture_sub_rectangle)
    {
        // Sprites should be centered within their texture rectangle by default.
        Origin.X = texture_sub_rectangle.Width() / 2.0f;
        Origin.Y = texture_sub_rectangle.Height() / 2.0f;
    }

    /// Gets the bounding box of the sprite, in world pixel coordinates.
    /// @return The bounding box of the sprite.
    MATH::FloatRectangle Sprite::GetWorldBoundingBox() const
    {
        /// @todo Completely eliminate SFML usage here!
        sf::Sprite sfml_sprite;
        sfml_sprite.setColor(sf::Color(Color.Red, Color.Green, Color.Blue, Color.Alpha));
        sfml_sprite.setOrigin(Origin.X, Origin.Y);
        sfml_sprite.setPosition(WorldPosition.X, WorldPosition.Y);
        sfml_sprite.setRotation(RotationAngleInDegrees);
        sfml_sprite.setScale(Scale.X, Scale.Y);

        sf::IntRect texture_rectangle;
        texture_rectangle.left = (int)TextureSubRectangle.LeftTop.X;
        texture_rectangle.top = (int)TextureSubRectangle.LeftTop.Y;
        texture_rectangle.width = (int)TextureSubRectangle.Width();
        texture_rectangle.height = (int)TextureSubRectangle.Height();
        sfml_sprite.setTextureRect(texture_rectangle);

        MATH::FloatRectangle world_bounding_box(sfml_sprite.getGlobalBounds());
        return world_bounding_box;
    }

    /// Gets the width of the sprite, in pixels.
    /// @return The width of the sprite.
    float Sprite::GetWidthInPixels() const
    {
        MATH::FloatRectangle bounding_box = GetWorldBoundingBox();
        float width = bounding_box.Width();
        return width;
    }

    /// Gets the height of the sprite, in pixels.
    /// @return The height of the sprite.
    float Sprite::GetHeightInPixels() const
    {
        MATH::FloatRectangle bounding_box = GetWorldBoundingBox();
        float height = bounding_box.Height();
        return height;
    }
}