#include <stdexcept>
#include "Graphics/Sprite.h"

namespace GRAPHICS
{
    std::shared_ptr<Sprite> Sprite::Create(
        const std::shared_ptr<Texture>& texture,
        const float x_position_in_texels,
        const float y_position_in_texels,
        const float width_in_texels,
        const float height_in_texels)
    {
        // MAKE SURE A TEXTURE RESOURCE WAS PROVIDED.
        std::shared_ptr<sf::Texture> texture_resource = texture->GetTextureResource();
        bool texture_resource_exists = (nullptr != texture.get());
        if (!texture_resource_exists)
        {
            return nullptr;
        }

        // CREATE THE RECTANGLE SPECIFYING WHICH PORTION OF THE TEXTURE THE IMAGE SHOULD COME FROM.
        sf::IntRect texture_rectangle(
            static_cast<int>(x_position_in_texels),
            static_cast<int>(y_position_in_texels),
            static_cast<int>(width_in_texels),
            static_cast<int>(height_in_texels));

        // CREATE THE SPRITE RESOURCE.
        std::shared_ptr<sf::Sprite> sprite_resource = std::make_shared<sf::Sprite>(
            *texture_resource,
            texture_rectangle);

        // CREATE THE SPRITE.
        std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(sprite_resource);
        return sprite;
    }

    Sprite::Sprite(
        const Texture& texture,
        const MATH::FloatRectangle& texture_sub_rectangle) :
    m_sprite(),
    m_visible(false),
    m_worldPositionInPixels(new MATH::Vector2f(0.0f, 0.0f)),
    m_zPosition(0.0f)
    {
        // CONVERT THE PARAMETERS TO THE FORMAT NEEDED FOR SFML.
        std::shared_ptr<sf::Texture> texture_resource = texture.GetTextureResource();
        sf::IntRect texture_rectangle(
            static_cast<int>(texture_sub_rectangle.GetLeftXPosition()),
            static_cast<int>(texture_sub_rectangle.GetTopYPosition()),
            static_cast<int>(texture_sub_rectangle.GetWidth()),
            static_cast<int>(texture_sub_rectangle.GetHeight()));

        // CREATE THE SFML SPRITE.
        m_sprite = std::make_shared<sf::Sprite>(*texture_resource, texture_rectangle);

        // Sprites should be centered within their texture rectangle by default.
        float texture_half_width_in_pixels = texture_sub_rectangle.GetWidth() / 2.0f;
        float texture_half_height_in_pixels = texture_sub_rectangle.GetHeight() / 2.0f;
        m_sprite->setOrigin(texture_half_width_in_pixels, texture_half_height_in_pixels);
    }

    Sprite::Sprite(const std::shared_ptr<sf::Sprite>& sprite) :
    m_sprite(sprite),
    m_visible(true),
    m_worldPositionInPixels(new MATH::Vector2f(sprite->getPosition().x, sprite->getPosition().y)),
    m_zPosition(0.0f)
    {
        // Nothing else to do.
    }
        
    Sprite::~Sprite()
    {
        // Nothing else to do.
    }

    std::shared_ptr<Sprite> Sprite::Clone() const
    {
        // CLONE THE SPRITE RESOURCE IF IT EXISTS.
        std::shared_ptr<sf::Sprite> cloned_sprite_resource;
        bool sprite_resource_exists = (nullptr != m_sprite);
        if (sprite_resource_exists)
        {
            cloned_sprite_resource = std::make_shared<sf::Sprite>(*m_sprite);
        }

        // CLONE THE POSITION COMPONENT IF IT EXISTS.
        std::shared_ptr<MATH::Vector2f> cloned_position;
        bool position_component_exists = (nullptr != m_worldPositionInPixels);
        if (position_component_exists)
        {
            cloned_position = std::make_shared<MATH::Vector2f>(*m_worldPositionInPixels);
        }

        // CREATE THE CLONED SPRITE.
        std::shared_ptr<Sprite> cloned_sprite = std::make_shared<Sprite>(
            cloned_sprite_resource);
        cloned_sprite->m_visible = m_visible;
        cloned_sprite->SetPositionComponent(cloned_position);
        cloned_sprite->SetZPosition(m_zPosition);

        return cloned_sprite;
    }

    void Sprite::SetVisible(const bool is_visible)
    {
        m_visible = is_visible;
    }

    bool Sprite::IsVisible() const
    {
        return m_visible;
    }

    void Sprite::Render(sf::RenderTarget& renderTarget)
    {
        // Make sure the sprite's position is updated for rendering.
        m_sprite->setPosition(m_worldPositionInPixels->X, m_worldPositionInPixels->Y);
        renderTarget.draw(*m_sprite);
    }

    void Sprite::Update(const float elapsedTimeInSeconds)
    {
        // Nothing to do for a basic sprite.
    }

    void Sprite::SetZPosition(const float zPosition)
    {
        m_zPosition = zPosition;
    }

    void Sprite::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
    {
        m_worldPositionInPixels = positionComponent;
    }

    MATH::Vector2f Sprite::GetWorldPosition() const
    {
        return (*m_worldPositionInPixels);
    }

    void Sprite::SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels)
    {
        m_worldPositionInPixels->X = xPositionInPixels;
        m_worldPositionInPixels->Y = yPositionInPixels;
    }

    MATH::FloatRectangle Sprite::GetBoundingBox() const
    {
        // GET THE SFML RECTANGLE.
        sf::FloatRect sfmlRectangle = m_sprite->getGlobalBounds();

        // CONVERT THE SFML RECTANGLE TO OUR CUSTOM RECTANGLE TYPE.
        return MATH::FloatRectangle(sfmlRectangle);
    }

    float Sprite::GetWidthInPixels() const
    {
        MATH::FloatRectangle bounding_box = GetBoundingBox();
        float width = bounding_box.GetWidth();
        return width;
    }

    float Sprite::GetHeightInPixels() const
    {
        MATH::FloatRectangle bounding_box = GetBoundingBox();
        float height = bounding_box.GetHeight();
        return height;
    }

    void Sprite::SetOrigin(const MATH::Vector2f& origin)
    {
        m_sprite->setOrigin(origin.X, origin.Y);
    }

    void Sprite::SetRotation(const float angle_in_degrees)
    {
        m_sprite->setRotation(angle_in_degrees);
    }

    void Sprite::SetScale(const MATH::Vector2f& scale)
    {
        m_sprite->setScale(
            scale.X,
            scale.Y);
    }
}