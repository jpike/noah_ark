#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Graphics/Color.h"
#include "Graphics/Screen.h"
#include "Graphics/Texture.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"
#include "Resources/AssetId.h"

namespace GRAPHICS
{
    /// A graphical sprite.
    class Sprite
    {
    public:
        // CONSTRUCTION/DESTRUCTION.
        explicit Sprite() = default;
        explicit Sprite(
            const RESOURCES::AssetId texture_id,
            const MATH::FloatRectangle& texture_sub_rectangle);

        // DIMENSIONS.
        MATH::FloatRectangle GetWorldBoundingBox() const;
        float GetWidthInPixels() const;
        float GetHeightInPixels() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Whether or not the sprite is visible.
        bool IsVisible = false;
        /// The world position of the sprite.
        MATH::Vector2f WorldPosition = MATH::Vector2f();
        /// The origin the of the sprite within its texture sub-rectangle
        /// (relative to the top-left corner).
        MATH::Vector2f Origin = MATH::Vector2f();
        /// The angle of rotation of the sprite (relative to its origin), in degrees.
        float RotationAngleInDegrees = 0.0f;
        /// The scale of the sprite.
        MATH::Vector2f Scale = MATH::Vector2f(1.0f, 1.0f);
        /// The color of the sprite (can be used for tinting).
        Color Color = Color::WHITE;
        /// The ID of the texture providing graphics for this sprite.
        RESOURCES::AssetId TextureId = RESOURCES::AssetId::INVALID;
        /// The sub-rectangle of the texture for the sprite's graphics.
        MATH::FloatRectangle TextureSubRectangle = MATH::FloatRectangle();
    };
}