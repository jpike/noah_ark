#include "Graphics/Texture.h"

namespace GRAPHICS
{
    /// Attempts to load a texture from the provided filepath.
    /// Pixels in the texture with a color of magenta will be
    /// considered transparent.
    /// @param[in]  filepath - The path to the texture file to load.
    ///     May be absolute or relative but must be accessible
    ///     from the program executing this code.
    /// @return The texture, if successfully loaded; a dummy
    ///     texture will be returned if part of loading fails.
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
        std::shared_ptr<GRAPHICS::Texture> texture = std::make_shared<GRAPHICS::Texture>();
        bool texture_loaded_from_image = texture->TextureResource.loadFromImage(image);
        // The return value is ignored so that the code can continue to execute.
        // It will just have an "empty" texture to have to deal with.
        return texture;
    }

    /// Gets the dimensions of the texture, in pixels.
    /// @return The dimensions of the texture image.
    MATH::Vector2ui Texture::GetSize() const
    {
        sf::Vector2u texture_resource_size = TextureResource.getSize();
        MATH::Vector2ui texture_size(texture_resource_size.x, texture_resource_size.y);
        return texture_size;
    }
}
