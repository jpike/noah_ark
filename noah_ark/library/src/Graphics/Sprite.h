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
    /// A graphical sprite.
    ///////////////////////////////////////////////////////////
    class Sprite : public IGraphicsComponent, public MATH::ITransformable
    {
    public:
        // CONSTRUCTION/DESTRUCTION.
        /// Creates to create a visible sprite from the provided data.
        /// @param[in]  texture - The texture resource to use for the sprite.
        /// @param[in]  x_position_in_texels - The x-coordinate of the left of the rectangle within 
        ///     the texture to use for the sprite image.
        /// @param[in]  y_position_in_texels - The y-coordinate of the top of the rectangle within
        ///     the texture to use for the sprite image.
        /// @param[in]  width_in_texels - The width of the rectangle within the texture to use for the sprite image.
        /// @param[in]  height_in_texels - The height of the rectangle within the texture to use for the sprite image.
        /// @return Null if the sprite failed to be created.
        static std::shared_ptr<Sprite> Create(
            const std::shared_ptr<Texture>& texture,
            const float x_position_in_texels,
            const float y_position_in_texels,
            const float width_in_texels,
            const float height_in_texels);
        /// Constructor.  The sprite is visible by default.
        /// @param[in]  sprite - The SFML sprite resource to use.
        explicit Sprite(const std::shared_ptr<sf::Sprite>& sprite);
        /// Destructor.
        virtual ~Sprite();

        /// Creates a deep copy of this sprite, except for
        /// any underlying texture resource.
        /// @return A deep copy of this sprite.
        std::shared_ptr<Sprite> Clone() const;

        // INTERFACE IMPLEMENTATION - IGraphicsComponent.
        /// @copydoc    IGraphicsComponent::IsVisible() const
        virtual bool IsVisible() const;

        /// @copydoc    IGraphicsComponent::Render(sf::RenderTarget& renderTarget)
        virtual void Render(sf::RenderTarget& renderTarget);

        /// @copydoc    IGraphicsComponent::Update(const float elapsedTimeInSeconds)
        virtual void Update(const float elapsedTimeInSeconds);

        /// @copydoc    IGraphicsComponent::SetZPosition(const float zPosition)
        virtual void SetZPosition(const float zPosition);

        // INTERFACE IMPLEMENTATION - ITransformable.
        /// @copydoc    ITransformable::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
        virtual void SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent);

        // POSITIONING.
        /// Gets the world position of the sprite.
        /// @return     The world position of the sprite, in pixels.
        MATH::Vector2f GetWorldPosition() const;
        /// Sets the world position of the sprite.
        /// @param[in]  xPositionInPixels - The x-coordinate of the sprite in the world.
        /// @param[in]  yPositionInPixels - The y-coordinate of the sprite in the world.
        void SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels);

        /// Gets the bounding box of the tile, in world pixel coordinates.
        /// @return The bounding box of the tile.
        MATH::FloatRectangle GetBoundingBox() const;

    private:
        // MEMBER VARIABLES.
        std::shared_ptr<sf::Sprite> m_sprite;   ///< The underlying HGE sprite resource.
        bool m_visible; ///< Whether or not the sprite is visible.
        std::shared_ptr<MATH::Vector2f> m_worldPositionInPixels;  ///< The position of the sprite in the world.
        float m_zPosition;  ///< The z-position of the sprite.
    };
}