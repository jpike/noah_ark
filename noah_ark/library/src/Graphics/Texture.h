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

        // DIMENSIONS.
        /// Gets the dimensions of the texture, in pixels.
        /// @return The dimensions of the texture image.
        MATH::Vector2ui GetSize() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The underlying SFML texture resource.
        sf::Texture TextureResource;
    };
}