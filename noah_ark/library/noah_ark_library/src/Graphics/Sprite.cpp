#include "Graphics/Sprite.h"

using namespace GRAPHICS;

Sprite::Sprite(const std::shared_ptr<hgeSprite>& sprite) :
    m_sprite(sprite),
    m_visible(true),
    m_screenPositionInPixels()
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

void Sprite::Render()
{
    m_sprite->Render(m_screenPositionInPixels.X, m_screenPositionInPixels.Y);
}

void Sprite::SetScreenPosition(const float xPositionInPixels, const float yPositionInPixels)
{
    m_screenPositionInPixels.X = xPositionInPixels;
    m_screenPositionInPixels.Y = yPositionInPixels;
}

void Sprite::SetFlip(const bool flippedHorizontally, const bool flippedVertically)
{
    m_sprite->SetFlip(flippedHorizontally, flippedVertically);
}