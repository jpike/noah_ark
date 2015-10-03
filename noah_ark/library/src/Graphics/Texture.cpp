#include "Graphics/Texture.h"

namespace GRAPHICS
{
    std::shared_ptr<Texture> Texture::Load(const std::string& filepath)
    {
        // LOAD THE IMAGE FOR THE TEXTURE.
        // The transparent color can only be set on an image.
        sf::Image image;
        bool image_loaded = image.loadFromFile(filepath);
        if (!image_loaded)
        {
            // CREATE A DUMMY IMAGE FOR DEBUGGING.
            // To allow the game to continue to run (rather than fail),
            // a dummy image is created with a specific color to make
            // it easy to debug.
            const unsigned int DUMMY_IMAGE_WIDTH_IN_PIXELS = 8;
            const unsigned int DUMMY_IMAGE_HEIGHT_IN_PIXELS = 8;
            const sf::Color DUMMY_IMAGE_COLOR = sf::Color::Cyan;
            image.create(DUMMY_IMAGE_WIDTH_IN_PIXELS, DUMMY_IMAGE_HEIGHT_IN_PIXELS, DUMMY_IMAGE_COLOR);
        }

        // ADD TRANSPARENCY TO THE IMAGE.
        // All images are expected to use magenta (R=255, G=0, B=255) for their parts
        // that should be transparent.
        const sf::Color TRANSPARENT_COLOR = sf::Color::Magenta;
        const uint8_t MAKE_COMPLETELY_TRANSPARENT = 0;
        image.createMaskFromColor(TRANSPARENT_COLOR, MAKE_COMPLETELY_TRANSPARENT);

        // CREATE THE TEXTURE FROM THE IMAGE.
        std::shared_ptr<sf::Texture> texture_resource = std::make_shared<sf::Texture>();
        bool texture_loaded_from_image = texture_resource->loadFromImage(image);
        if (!texture_loaded_from_image)
        {
            // Continue processing so that the calling code can continue to execute.
            // It will just have an "empty" texture tohave to deal with.
        }

        // WRAP THE SFML TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> texture = std::make_shared<GRAPHICS::Texture>(texture_resource);
        return texture;
    }

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

    void Texture::Copy(const Texture& texture_to_copy)
    {
        this->TextureResource = texture_to_copy.TextureResource;
    }
}
