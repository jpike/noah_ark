#include "Graphics/Texture.h"

using namespace GRAPHICS;

// STATIC MEMBER CONSTANTS.
const HTEXTURE Texture::INVALID_TEXTURE_HANDLE = 0;

// METHODS.

Texture::Texture(const HTEXTURE textureHandle) :
    m_textureHandle(textureHandle)
{
    // Nothing else to do.
}

Texture::Texture(const Texture& textureToCopy) :
    m_textureHandle(INVALID_TEXTURE_HANDLE)
{
    Copy(textureToCopy);
}

Texture::~Texture()
{
    // Nothing else to do.
}

Texture& Texture::operator= (const Texture& rhsTexture)
{
    // CHECK FOR SELF-ASSIGNMENT.
    bool selfAssignment = ( this == &rhsTexture );
    if (!selfAssignment)
    {
        // Copy the values of the right-hand side texture into this texture.
        Copy(rhsTexture);
    }

    // RETURN THIS OBJECT.
    return (*this);
}

const HTEXTURE Texture::GetTextureHandle() const
{
    return m_textureHandle;
}

void Texture::Copy(const Texture& textureToCopy)
{
    this->m_textureHandle = textureToCopy.m_textureHandle;
}
