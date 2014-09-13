#pragma once

#include <hgesprite.h>
#include <memory>
#include "Graphics/IGraphicsComponent.h"
#include "Graphics/Texture.h"
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
        /// @param[in]  sprite - The HGE sprite resource to use.
        explicit Sprite(const std::shared_ptr<hgeSprite>& sprite);
        
        /// @brief  Destructor.
        virtual ~Sprite();

        /// @copydoc    IGraphicsComponent::IsVisible() const
        virtual bool IsVisible() const;

        /// @copydoc    IGraphicsComponent::Render()
        virtual void Render();

        /// @copydoc    IGraphicsComponent::Update(const float elapsedTimeInSeconds)
        virtual void Update(const float elapsedTimeInSeconds);

        /// @copydoc    IGraphicsComponent::SetZPosition(const float zPosition)
        virtual void SetZPosition(const float zPosition);

        /// @copydoc    ITransformable::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
        virtual void SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent);

        /// @brief      Sets the world position of the sprite.
        /// @param[in]  xPositionInPixels - The x-coordinate of the sprite in the world.
        /// @param[in]  yPositionInPixels - The y-coordinate of the sprite in the world.
        void SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels);

        /// @brief      Sets the flipping capabilities of the sprite.
        /// @param[in]  flippedHorizontally - Whether or not the sprite is flipped horizontally.
        /// @param[in]  flippedVertically - Whether or not the sprite is flipped horizontally.
        void SetFlip(const bool flippedHorizontally, const bool flippedVertically);

    private:
        std::shared_ptr<hgeSprite> m_sprite;   ///< The underlying HGE sprite resource.
        bool m_visible; ///< Whether or not the sprite is visible.
        std::shared_ptr<MATH::Vector2f> m_worldPositionInPixels;  ///< The position of the sprite in the world.
    };
}