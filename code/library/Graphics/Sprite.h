#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Color.h"
#include "Graphics/Screen.h"
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
        explicit Sprite();
        explicit Sprite(
            const std::shared_ptr<const Texture>& texture,
            const MATH::FloatRectangle& texture_sub_rectangle);

        // OTHER METHODS.
        void SetTextureRectangle(const MATH::IntRectangle& texture_rectangle);
        void Render(Screen& screen) const;

        // POSITIONING.
        MATH::Vector2f GetWorldPosition() const;
        void SetWorldPosition(const MATH::Vector2f& world_position);
        void SetWorldPosition(const float x_position_in_pixels, const float y_position_in_pixels);

        // DIMENSIONS.
        MATH::FloatRectangle GetWorldBoundingBox() const;
        float GetWidthInPixels() const;
        float GetHeightInPixels() const;

        // ROTATION.
        MATH::Vector2f GetOrigin() const;
        void SetOrigin(const MATH::Vector2f& origin);
        void SetRotation(const float angle_in_degrees);

        // SCALING.
        void SetScale(const float scale);
        void SetScale(const MATH::Vector2f& scale);

        // COLOR.
        GRAPHICS::Color GetColor() const;
        void SetColor(const GRAPHICS::Color& color);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The texture providing graphics for this sprite.
        /// Stored here to ensure the texture stays in memory as long as the sprite
        /// is in memory.  Does not seem to result in any noticeable increases in
        /// memory usage.
        std::shared_ptr<const Texture> SpriteTexture;
        /// The underlying SFML sprite resource.
        sf::Sprite SpriteResource;
        /// Whether or not the sprite is visible.
        bool IsVisible;
    };
}