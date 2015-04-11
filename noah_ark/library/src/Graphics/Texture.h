#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "Math/Vector2.h"

namespace GRAPHICS
{
    /// A texture resource.
    class Texture
    {
    public:        
        /// Constructor.
        /// @param[in]  texture_resource - The SFML texture that is the underlying resource.
        explicit Texture(const std::shared_ptr<sf::Texture>& texture_resource);
        /// Copy constructor.
        /// @param[in]  other - The texture to copy.
        Texture(const Texture& other);
        /// Destructor.
        ~Texture();

        /// Assignment operator.
        /// @param[in]  rhs - The texture to assign from.
        Texture& operator= (const Texture& rhs);

        /// Gets the dimensions of the texture, in pixels.
        /// @return The dimensions of the texture image.
        MATH::Vector2ui GetSize() const;

        /// Returns the underlying SFML texture resource.
        /// @return The SFML texture resource.
        const std::shared_ptr<sf::Texture> GetTextureResource() const;

    private:
        /// Helper method for copying a texture.
        /// @param[in]  texture_to_copy - The texture to copy.
        void Copy(const Texture& texture_to_copy);

        std::shared_ptr<sf::Texture> TextureResource;   ///< The underlying SFML texture resource.
    };
}