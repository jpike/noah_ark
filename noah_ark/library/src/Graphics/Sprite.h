#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/IGraphicsComponent.h"
#include "Graphics/Texture.h"
#include "Math/FloatRectangle.h"
#include "Math/ITransformable.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A graphical sprite.
    ///////////////////////////////////////////////////////////
    class Sprite : public IGraphicsComponent, public MATH::ITransformable
    {
    public:
        /// @brief      Constructor.  The sprite is visible by default.
        /// @param[in]  sprite - The SFML sprite resource to use.
        explicit Sprite(const std::shared_ptr<sf::Sprite>& sprite);
        
        /// @brief  Destructor.
        virtual ~Sprite();

        /// @copydoc    IGraphicsComponent::IsVisible() const
        virtual bool IsVisible() const;

        /// @copydoc    IGraphicsComponent::Render(sf::RenderTarget& renderTarget)
        virtual void Render(sf::RenderTarget& renderTarget);

        /// @copydoc    IGraphicsComponent::Update(const float elapsedTimeInSeconds)
        virtual void Update(const float elapsedTimeInSeconds);

        /// @copydoc    IGraphicsComponent::SetZPosition(const float zPosition)
        virtual void SetZPosition(const float zPosition);

        /// @copydoc    ITransformable::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
        virtual void SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent);

        /// @brief      Gets the world position of the sprite.
        /// @return     The world position of the sprite, in pixels.
        MATH::Vector2f GetWorldPosition() const;
        /// @brief      Sets the world position of the sprite.
        /// @param[in]  xPositionInPixels - The x-coordinate of the sprite in the world.
        /// @param[in]  yPositionInPixels - The y-coordinate of the sprite in the world.
        void SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels);

        /// @brief  Gets the bounding box of the tile, in world pixel coordinates.
        /// @return The bounding box of the tile.
        MATH::FloatRectangle GetBoundingBox() const;

    private:
        std::shared_ptr<sf::Sprite> m_sprite;   ///< The underlying HGE sprite resource.
        bool m_visible; ///< Whether or not the sprite is visible.
        std::shared_ptr<MATH::Vector2f> m_worldPositionInPixels;  ///< The position of the sprite in the world.
        float m_zPosition;  ///< The z-position of the sprite.
    };
}