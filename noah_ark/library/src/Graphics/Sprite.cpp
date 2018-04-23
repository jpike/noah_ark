#include "Graphics/Sprite.h"

namespace GRAPHICS
{
    /// Creates an invisible sprite positioned at (0,0,0).
    Sprite::Sprite() :
    SpriteTexture(),
    SpriteResource(),
    IsVisible(false)
    {
        // INITIALIZE THE POSITION OF THE SPRITE.
        SpriteResource.setPosition(0.0f, 0.0f);
    }

    /// Creates an invisible sprite based on the provided texture information.
    /// @param[in]  texture - The texture containing graphics for the sprite.
    /// @param[in]  texture_sub_rectangle - The sub-rectangle of the texture
    /// defining which portion should be used for the sprite.
    Sprite::Sprite(
        const std::shared_ptr<const Texture>& texture,
        const MATH::FloatRectangle& texture_sub_rectangle) :
    SpriteTexture(texture),
    SpriteResource(),
    IsVisible(false)
    {
        // CONVERT THE PARAMETERS TO THE FORMAT NEEDED FOR SFML.
        sf::IntRect texture_rectangle(
            static_cast<int>(texture_sub_rectangle.GetLeftXPosition()),
            static_cast<int>(texture_sub_rectangle.GetTopYPosition()),
            static_cast<int>(texture_sub_rectangle.GetWidth()),
            static_cast<int>(texture_sub_rectangle.GetHeight()));

        // CREATE THE SFML SPRITE.
        SpriteResource = sf::Sprite(SpriteTexture->TextureResource);
        SpriteResource.setTextureRect(texture_rectangle);

        // Sprites should be centered within their texture rectangle by default.
        float texture_half_width_in_pixels = texture_sub_rectangle.GetWidth() / 2.0f;
        float texture_half_height_in_pixels = texture_sub_rectangle.GetHeight() / 2.0f;
        SpriteResource.setOrigin(texture_half_width_in_pixels, texture_half_height_in_pixels);
    }

    /// Sets the sub-rectangle of the sprite's texture that should be used
    /// for rendering.
    /// @param[in]  texture_rectangle - The rectangle of the texture to
    ///     render for the sprite.  Relative to (0,0) being the top-left
    ///     corner of the texture.
    void Sprite::SetTextureRectangle(const MATH::IntRectangle& texture_rectangle)
    {
        sf::IntRect sfml_rectangle(
            texture_rectangle.GetLeftXPosition(),
            texture_rectangle.GetTopYPosition(),
            texture_rectangle.GetWidth(),
            texture_rectangle.GetHeight());
        SpriteResource.setTextureRect(sfml_rectangle);
    }

    /// Renders the sprite to the provided screen.
    /// @param[in,out]  screen - The screento render to.
    void Sprite::Render(Screen& screen) const
    {
        screen.RenderTarget.draw(SpriteResource);
    }

    /// Gets the world position of the sprite.
    /// @return The world position of the sprite, in pixels.
    MATH::Vector2f Sprite::GetWorldPosition() const
    {
        const sf::Vector2f& sprite_position = SpriteResource.getPosition();
        MATH::Vector2f world_position(
            sprite_position.x,
            sprite_position.y);
        return world_position;
    }

    /// Sets the world position of the sprite.
    /// @param[in]  world_position - The position of the sprite in the world.
    void Sprite::SetWorldPosition(const MATH::Vector2f& world_position)
    {
        SetWorldPosition(world_position.X, world_position.Y);
    }

    /// Sets the world position of the sprite.
    /// @param[in]  x_position_in_pixels - The x-coordinate of the sprite in the world.
    /// @param[in]  y_position_in_pixels - The y-coordinate of the sprite in the world.
    void Sprite::SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels)
    {
        SpriteResource.setPosition(x_position_in_pixels, y_position_in_pixels);
    }

    /// Gets the bounding box of the sprite, in world pixel coordinates.
    /// @return The bounding box of the sprite.
    MATH::FloatRectangle Sprite::GetWorldBoundingBox() const
    {
        // GET THE SFML RECTANGLE.
        sf::FloatRect sfmlRectangle = SpriteResource.getGlobalBounds();

        // CONVERT THE SFML RECTANGLE TO OUR CUSTOM RECTANGLE TYPE.
        return MATH::FloatRectangle(sfmlRectangle);
    }

    /// Gets the width of the sprite, in pixels.
    /// @return The width of the sprite.
    float Sprite::GetWidthInPixels() const
    {
        MATH::FloatRectangle bounding_box = GetWorldBoundingBox();
        float width = bounding_box.GetWidth();
        return width;
    }

    /// Gets the height of the sprite, in pixels.
    /// @return The height of the sprite.
    float Sprite::GetHeightInPixels() const
    {
        MATH::FloatRectangle bounding_box = GetWorldBoundingBox();
        float height = bounding_box.GetHeight();
        return height;
    }

    /// Gets the local origin (center) of the sprite,
    /// relative to its top-left coordinate of (0,0).
    /// @return The sprite's origin, in pixel coordinates.
    MATH::Vector2f Sprite::GetOrigin() const
    {
        sf::Vector2f sprite_origin = SpriteResource.getOrigin();
        MATH::Vector2f origin(sprite_origin.x, sprite_origin.y);
        return origin;
    }

    /// Sets the local origin (center) of the sprite used for rotation,
    /// relative to its top-left coordinate of (0,0).
    /// @param[in]  origin - The origin, in pixel coordinates.
    void Sprite::SetOrigin(const MATH::Vector2f& origin)
    {
        SpriteResource.setOrigin(origin.X, origin.Y);
    }

    /// Sets the rotation of the sprite, relative to its origin.
    /// @param[in]  angle_in_degrees - The angle for the rotation.
    void Sprite::SetRotation(const float angle_in_degrees)
    {
        SpriteResource.setRotation(angle_in_degrees);
    }

    /// Sets the scaling of the sprite (in both X and Y dimensions).
    /// 1.0f represents the normal, unscaled value for a dimension.
    /// @param[in]  scale - The scaling amount for the sprite.
    void Sprite::SetScale(const float scale)
    {
        SpriteResource.setScale(scale, scale);
    }

    /// Sets the scaling of the sprite.  1.0f represents
    /// the normal, unscaled value for a dimension.
    /// @param[in]  scale - The scaling amount for the sprite.
    void Sprite::SetScale(const MATH::Vector2f& scale)
    {
        SpriteResource.setScale(scale.X, scale.Y);
    }

    /// Gets the color of the sprite.
    /// @return The color of the sprite.
    GRAPHICS::Color Sprite::GetColor() const
    {
        // CONVERT THE COLOR FROM ITS SFML FORMAT.
        sf::Color sfml_color = SpriteResource.getColor();
        
        GRAPHICS::Color color;
        color.Red = sfml_color.r;
        color.Green = sfml_color.g;
        color.Blue = sfml_color.b;
        color.Alpha = sfml_color.a;
        return color;
    }

    /// Sets the color of the sprite.  This color modulates the
    /// sprite's texture values and can be used for things like
    /// changing its tint or opacity.
    /// @param[in]  color - The color to set for the sprite.
    void Sprite::SetColor(const GRAPHICS::Color& color)
    {
        SpriteResource.setColor(
            sf::Color(color.Red, color.Green, color.Blue, color.Alpha));
    }
}