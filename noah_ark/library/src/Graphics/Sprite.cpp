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

    Sprite::Sprite(const std::shared_ptr<sf::Sprite>& sprite) :
    m_sprite(sprite),
    m_visible(true),
    m_worldPositionInPixels(new MATH::Vector2f(0.0f, 0.0f)),
    m_zPosition(0.0f)
    {
        // Nothing else to do.
    }
        
    Sprite::~Sprite()
    {
        // Nothing else to do.
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
}