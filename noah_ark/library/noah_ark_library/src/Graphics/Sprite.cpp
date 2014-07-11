#include "Graphics/Sprite.h"

using namespace GRAPHICS;

Sprite::Sprite(const std::shared_ptr<hgeSprite>& sprite) :
    m_sprite(sprite),
    m_visible(true),
    m_worldPositionInPixels()
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
    m_sprite->Render(m_worldPositionInPixels.X, m_worldPositionInPixels.Y);
}

void Sprite::SetZPosition(const float zPosition)
{
    m_sprite->SetZ(zPosition);
}

void Sprite::SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels)
{
    m_worldPositionInPixels.X = xPositionInPixels;
    m_worldPositionInPixels.Y = yPositionInPixels;
}

void Sprite::SetFlip(const bool flippedHorizontally, const bool flippedVertically)
{
    m_sprite->SetFlip(flippedHorizontally, flippedVertically);
}