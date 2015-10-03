#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Math/Vector2.h"

namespace GRAPHICS
{
    /// A texture resource.
    class Texture
    {
    public:
        // STATIC METHODS.
        /// Attempts to load a texture from the provided filepath.
        /// Pixels in the texture with a color of magenta will be
        /// considered transparent.
        /// @param[in]  filepath - The path to the texture file to load.
        ///     May be absolute or relative but must be accessible
        ///     from the program executing this code.
        /// @return The texture, if successfully loaded; a dummy
        ///     texture will be returned if part of loading fails.
        static std::shared_ptr<Texture> Load(const std::string& filepath);

        // CONSTRUCTION/DESTRUCTION.
        /// Constructor.
        /// @param[in]  texture_resource - The SFML texture that is the underlying resource.
        explicit Texture(const std::shared_ptr<sf::Texture>& texture_resource);
        /// Copy constructor.
        /// @param[in]  other - The texture to copy.
        Texture(const Texture& other);
        /// Destructor.
        ~Texture();

        // OPERATORS.
        /// Assignment operator.
        /// @param[in]  rhs - The texture to assign from.
        Texture& operator= (const Texture& rhs);

        // DIMENSIONS.
        /// Gets the dimensions of the texture, in pixels.
        /// @return The dimensions of the texture image.
        MATH::Vector2ui GetSize() const;

        // SFML INTEROPERABILITY.
        /// Returns the underlying SFML texture resource.
        /// @return The SFML texture resource.
        const std::shared_ptr<sf::Texture> GetTextureResource() const;

    private:
        // COPYING.
        /// Helper method for copying a texture.
        /// @param[in]  texture_to_copy - The texture to copy.
        void Copy(const Texture& texture_to_copy);

        // MEMBER VARIABLES.
        std::shared_ptr<sf::Texture> TextureResource;   ///< The underlying SFML texture resource.
    };
}