#pragma once

#include <hgesprite.h>
#include <memory>
#include "Graphics/IGraphicsComponent.h"
#include "Graphics/Texture.h"
#include "Math/Vector2.h"

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A graphical sprite.
    ///////////////////////////////////////////////////////////
    class Sprite : public IGraphicsComponent
    {
    public:
        /// @brief      Constructor.  The sprite is visible by default.
        /// @param[in]  sprite - The HGE sprite resource to use.
        explicit Sprite(const std::shared_ptr<hgeSprite>& sprite);
        
        /// @brief  Destructor.
        virtual ~Sprite();

        /// @brief  Indicates whether or not the sprite is visible.
        /// @return The visibility of the sprite.
        virtual bool IsVisible() const;

        /// @brief  Renders the sprite.
        virtual void Render();

        /// @brief      Sets the screen position of the sprite.
        /// @param[in]  xPositionInPixels - The x-coordinate of the sprite on screen.
        /// @param[in]  yPositionInPixels - The y-coordinate of the sprite on screen.
        void SetScreenPosition(const float xPositionInPixels, const float yPositionInPixels);

        /// @brief      Sets the flipping capabilities of the sprite.
        /// @param[in]  flippedHorizontally - Whether or not the sprite is flipped horizontally.
        /// @param[in]  flippedVertically - Whether or not the sprite is flipped horizontally.
        void SetFlip(const bool flippedHorizontally, const bool flippedVertically);

    private:
        std::shared_ptr<hgeSprite> m_sprite;   ///< The underlying HGE sprite resource.
        bool m_visible; ///< Whether or not the sprite is visible.
        MATH::Vector2f m_screenPositionInPixels;  ///< The position of the sprite on screen.
    };
}