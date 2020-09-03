#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "Graphics/Color.h"
#include "Math/Vector2.h"
#include "Memory/Pointers.h"

namespace GRAPHICS
{
    /// A texture resource.
    class Texture
    {
    public:
        // STATIC METHODS.
        static MEMORY::NonNullSharedPointer<GRAPHICS::Texture> CreateSolid(
            const unsigned int width_in_pixels,
            const unsigned int height_in_pixels,
            const GRAPHICS::Color& color);
        static std::shared_ptr<Texture> Load(const sf::Image& image);
        static std::shared_ptr<Texture> Load(const std::string& filepath);
        static std::shared_ptr<Texture> LoadFromMemory(const std::string& binary_data);

        // DIMENSIONS.
        MATH::Vector2ui GetSize() const;

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The underlying SFML texture resource.
        sf::Texture TextureResource;
    };
}