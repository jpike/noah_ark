#include "Graphics/Texture.h"

using namespace GRAPHICS;

Texture::Texture(const std::shared_ptr<sf::Texture>& textureResource) :
    m_textureResource(textureResource)
{
    // Nothing else to do.
}

Texture::Texture(const Texture& textureToCopy) :
    m_textureResource()
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

const std::shared_ptr<sf::Texture> Texture::GetTextureResource() const
{
    return m_textureResource;
}

void Texture::Copy(const Texture& textureToCopy)
{
    this->m_textureResource = textureToCopy.m_textureResource;
}
