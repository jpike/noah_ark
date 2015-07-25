#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/IGraphicsComponent.h"
#include "Graphics/Texture.h"
#include "Math/ITransformable.h"
#include "Math/Rectangle.h"
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
        /// Creates an visible sprite positioned at (0,0,0).
        explicit Sprite();
        /// Creates an invisible sprite based on the provided texture information.
        /// @param[in]  texture - The texture containing graphics for the sprite.
        /// @param[in]  texture_sub_rectangle - The sub-rectangle of the texture
        /// defining which portion should be used for the sprite.
        explicit Sprite(
            const Texture& texture,
            const MATH::FloatRectangle& texture_sub_rectangle);
        /// Constructor.  The sprite is visible by default.
        /// @param[in]  sprite - The SFML sprite resource to use.
        explicit Sprite(const std::shared_ptr<sf::Sprite>& sprite);
        /// Destructor.
        virtual ~Sprite();

        /// Creates a deep copy of this sprite, except for
        /// any underlying texture resource.
        /// @return A deep copy of this sprite.
        std::shared_ptr<Sprite> Clone() const;

        /// Sets the sub-rectangle of the sprite's texture that should be used
        /// for rendering.
        /// @param[in]  texture_rectangle - The rectangle of the texture to
        ///     render for the sprite.  Relative to (0,0) being the top-left
        ///     corner of the texture.
        void SetTextureRectangle(const MATH::IntRectangle& texture_rectangle);

        /// Sets if the sprite is visible or not.
        /// @param[in]  is_visible - True if the sprite should be visible;
        /// false if the sprite should be hidden.
        void SetVisible(const bool is_visible);

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

        // MOVEMENT.
        /// Moves the sprite's world position up the specified distance.
        /// @param[in]  distance_to_move_in_pixels - The distance to move, in pixels.
        void MoveUp(const float distance_to_move_in_pixels);
        /// Moves the sprite's world position down the specified distance.
        /// @param[in]  distance_to_move_in_pixels - The distance to move, in pixels.
        void MoveDown(const float distance_to_move_in_pixels);
        /// Moves the sprite's world position left the specified distance.
        /// @param[in]  distance_to_move_in_pixels - The distance to move, in pixels.
        void MoveLeft(const float distance_to_move_in_pixels);
        /// Moves the sprite's world position right the specified distance.
        /// @param[in]  distance_to_move_in_pixels - The distance to move, in pixels.
        void MoveRight(const float distance_to_move_in_pixels);

        // DIMENSIONS.
        /// Gets the bounding box of the sprite, in world pixel coordinates.
        /// @return The bounding box of the sprite.
        MATH::FloatRectangle GetBoundingBox() const;
        /// Gets the width of the sprite, in pixels.
        /// @return The width of the sprite.
        float GetWidthInPixels() const;
        /// Gets the height of the sprite, in pixels.
        /// @return The height of the sprite.
        float GetHeightInPixels() const;

        // ROTATION.
        /// Sets the rotation of the sprite, relative to its origin.
        /// @param[in]  angle_in_degrees - The angle for the rotation.
        void SetRotation(const float angle_in_degrees);
        /// Sets the local origin of the sprite used for rotation,
        /// relative to its top-left coordinate of (0,0).
        /// @param[in]  origin - The origin, in pixel coordinates.
        void SetOrigin(const MATH::Vector2f& origin);

        // SCALING.
        /// Sets the scaling of the sprite.  1.0f represents
        /// the normal, unscaled value for a dimension.
        /// @param[in]  scale - The scaling amount for the sprite.
        void SetScale(const MATH::Vector2f& scale);

    private:
        // MEMBER VARIABLES.
        std::shared_ptr<sf::Sprite> m_sprite;   ///< The underlying HGE sprite resource.
        bool m_visible; ///< Whether or not the sprite is visible.
        std::shared_ptr<MATH::Vector2f> m_worldPositionInPixels;  ///< The position of the sprite in the world.
        float m_zPosition;  ///< The z-position of the sprite.
    };
}