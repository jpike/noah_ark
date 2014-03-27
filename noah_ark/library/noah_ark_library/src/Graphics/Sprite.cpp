#include "Graphics/Sprite.h"

using namespace GRAPHICS;

Sprite::Sprite() :
    m_pSprite(nullptr)
{
    // Nothing else to do.
}
        
Sprite::~Sprite()
{
    // The actual memory will be freed by the sprite that created this sprite.
    m_pSprite = nullptr;
}