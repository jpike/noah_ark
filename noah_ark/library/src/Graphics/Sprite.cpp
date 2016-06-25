#include <stdexcept>
#include "Graphics/Sprite.h"

namespace GRAPHICS
{
    Sprite::Sprite() :
    SpriteTexture(),
    SpriteResource(),
    IsVisible(false)
    {
        // INITIALIZE THE POSITION OF THE SPRITE.
        SpriteResource.setPosition(0.0f, 0.0f);
    }

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

    void Sprite::SetTextureRectangle(const MATH::IntRectangle& texture_rectangle)
    {
        sf::IntRect sfml_rectangle(
            texture_rectangle.GetLeftXPosition(),
            texture_rectangle.GetTopYPosition(),
            texture_rectangle.GetWidth(),
            texture_rectangle.GetHeight());
        SpriteResource.setTextureRect(sfml_rectangle);
    }

    void Sprite::Render(sf::RenderTarget& render_target) const
    {
        render_target.draw(SpriteResource);
    }

    MATH::Vector2f Sprite::GetWorldPosition() const
    {
        const sf::Vector2f& sprite_position = SpriteResource.getPosition();
        MATH::Vector2f world_position(
            sprite_position.x,
            sprite_position.y);
        return world_position;
    }

    void Sprite::SetWorldPosition(const MATH::Vector2f& world_position)
    {
        SetWorldPosition(world_position.X, world_position.Y);
    }

    void Sprite::SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels)
    {
        SpriteResource.setPosition(x_position_in_pixels, y_position_in_pixels);
    }

    MATH::FloatRectangle Sprite::GetWorldBoundingBox() const
    {
        // GET THE SFML RECTANGLE.
        sf::FloatRect sfmlRectangle = SpriteResource.getGlobalBounds();

        // CONVERT THE SFML RECTANGLE TO OUR CUSTOM RECTANGLE TYPE.
        return MATH::FloatRectangle(sfmlRectangle);
    }

    float Sprite::GetWidthInPixels() const
    {
        MATH::FloatRectangle bounding_box = GetWorldBoundingBox();
        float width = bounding_box.GetWidth();
        return width;
    }

    float Sprite::GetHeightInPixels() const
    {
        MATH::FloatRectangle bounding_box = GetWorldBoundingBox();
        float height = bounding_box.GetHeight();
        return height;
    }

    void Sprite::SetOrigin(const MATH::Vector2f& origin)
    {
        SpriteResource.setOrigin(origin.X, origin.Y);
    }

    void Sprite::SetRotation(const float angle_in_degrees)
    {
        SpriteResource.setRotation(angle_in_degrees);
    }

    void Sprite::SetScale(const float scale)
    {
        SpriteResource.setScale(scale, scale);
    }

    void Sprite::SetScale(const MATH::Vector2f& scale)
    {
        SpriteResource.setScale(scale.X, scale.Y);
    }

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

    void Sprite::SetColor(const GRAPHICS::Color& color)
    {
        SpriteResource.setColor(
            sf::Color(color.Red, color.Green, color.Blue, color.Alpha));
    }
}