#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Texture.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
    /// A graphical sprite.
    class Sprite
    {
    public:
        // CONSTRUCTION/DESTRUCTION.
        /// Creates an invisible sprite positioned at (0,0,0).
        explicit Sprite();
        /// Creates an invisible sprite based on the provided texture information.
        /// @param[in]  texture - The texture containing graphics for the sprite.
        /// @param[in]  texture_sub_rectangle - The sub-rectangle of the texture
        /// defining which portion should be used for the sprite.
        explicit Sprite(
            const std::shared_ptr<Texture>& texture,
            const MATH::FloatRectangle& texture_sub_rectangle);

        /// Sets the sub-rectangle of the sprite's texture that should be used
        /// for rendering.
        /// @param[in]  texture_rectangle - The rectangle of the texture to
        ///     render for the sprite.  Relative to (0,0) being the top-left
        ///     corner of the texture.
        void SetTextureRectangle(const MATH::IntRectangle& texture_rectangle);

        /// @copydoc    IGraphicsComponent::Render(sf::RenderTarget& renderTarget)
        virtual void Render(sf::RenderTarget& render_target) const;

        // POSITIONING.
        /// Gets the world position of the sprite.
        /// @return The world position of the sprite, in pixels.
        MATH::Vector2f GetWorldPosition() const;
        /// Sets the world position of the sprite.
        /// @param[in]  x_position_in_pixels - The x-coordinate of the sprite in the world.
        /// @param[in]  y_position_in_pixels - The y-coordinate of the sprite in the world.
        void SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels);

        // DIMENSIONS.
        /// Gets the bounding box of the sprite, in world pixel coordinates.
        /// @return The bounding box of the sprite.
        MATH::FloatRectangle GetWorldBoundingBox() const;
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

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The texture providing graphics for this sprite.
        /// Stored here to ensure the texture stays in memory as long as the sprite
        /// is in memory.  Does not seem to result in any noticeable increases in
        /// memory usage.
        std::shared_ptr<Texture> SpriteTexture;
        /// The underlying SFML sprite resource.
        sf::Sprite SpriteResource;
        /// Whether or not the sprite is visible.
        bool IsVisible;
    };
}