#include "Graphics/Sprite.h"

using namespace GRAPHICS;

Sprite::Sprite(const std::shared_ptr<hgeSprite>& sprite) :
    m_sprite(sprite),
    m_visible(true),
    m_worldPositionInPixels(new MATH::Vector2f(0.0f, 0.0f))
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
    m_sprite->Render(m_worldPositionInPixels->X, m_worldPositionInPixels->Y);
}

void Sprite::Update(const float elapsedTimeInSeconds)
{
    // Nothing to do for a basic sprite.
}

void Sprite::SetZPosition(const float zPosition)
{
    m_sprite->SetZ(zPosition);
}

void Sprite::SetPositionComponent(const std::shared_ptr<MATH::Vector2f>& positionComponent)
{
    m_worldPositionInPixels = positionComponent;
}

void Sprite::SetWorldPosition(const float xPositionInPixels, const float yPositionInPixels)
{
    m_worldPositionInPixels->X = xPositionInPixels;
    m_worldPositionInPixels->Y = yPositionInPixels;
}

void Sprite::SetFlip(const bool flippedHorizontally, const bool flippedVertically)
{
    m_sprite->SetFlip(flippedHorizontally, flippedVertically);
}

MATH::FloatRectangle Sprite::GetBoundingBox() const
{
    // GET THE HGE RECTANGLE.
    hgeRect hgeRectangle;
    m_sprite->GetBoundingBox(
        m_worldPositionInPixels->X,
        m_worldPositionInPixels->Y,
        &hgeRectangle);

    // CONVERT THE HGE RECTANGLE TO OUR CUSTOM RECTANGLE TYPE.
    return MATH::FloatRectangle(hgeRectangle);
}