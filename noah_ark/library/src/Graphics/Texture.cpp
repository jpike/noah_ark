#include "Graphics/Texture.h"

using namespace GRAPHICS;

Texture::Texture(const std::shared_ptr<sf::Texture>& texture_resource) :
TextureResource(texture_resource)
{
    // Nothing else to do.
}

Texture::Texture(const Texture& other) :
TextureResource()
{
    Copy(other);
}

Texture::~Texture()
{
    // Nothing else to do.
}

Texture& Texture::operator= (const Texture& rhs)
{
    // CHECK FOR SELF-ASSIGNMENT.
    bool self_assignment = (this == &rhs);
    if (!self_assignment)
    {
        // Copy the values of the right-hand side texture into this texture.
        Copy(rhs);
    }

    // RETURN THIS OBJECT.
    return (*this);
}

MATH::Vector2ui Texture::GetSize() const
{
    sf::Vector2u texture_resource_size = TextureResource->getSize();
    MATH::Vector2ui texture_size(texture_resource_size.x, texture_resource_size.y);
    return texture_size;
}

const std::shared_ptr<sf::Texture> Texture::GetTextureResource() const
{
    return TextureResource;
}

void Texture::Copy(const Texture& textureToCopy)
{
    this->TextureResource = textureToCopy.TextureResource;
}
