#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Graphics/Texture.h"
#include "Memory/Pointers.h"
#include "Resources/AssetId.h"

namespace GRAPHICS
{
    /// A graphics device (also known as a graphics card or GPU).
    class GraphicsDevice
    {
    public:
        // TEXTURES.
        void LoadTexture(const RESOURCES::AssetId asset_id, const std::string& binary_data);
        MEMORY::NonNullSharedPointer<GRAPHICS::Texture> GetTexture(const RESOURCES::AssetId asset_id);

        // SHADERS.
        void LoadShader(const RESOURCES::AssetId asset_id, const sf::Shader::Type shader_type, const std::string& binary_data);
        std::shared_ptr<sf::Shader> GetShader(const RESOURCES::AssetId asset_id);
        
    private:
        // PRIVATE MEMBER VARIABLES.
        /// A mutex to provide thread-safety for this class.
        std::recursive_mutex Mutex = {};
        /// Textures to use for rendering.
        std::unordered_map<RESOURCES::AssetId, MEMORY::NonNullSharedPointer<GRAPHICS::Texture>> Textures = {};
        /// Shaders to use for rendering.
        std::unordered_map<RESOURCES::AssetId, MEMORY::NonNullSharedPointer<sf::Shader>> Shaders = {};
    };
}
