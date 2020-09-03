#include "Graphics/Color.h"
#include "Graphics/GraphicsDevice.h"

namespace GRAPHICS
{
    /// Attempts to load a texture into the graphics device.
    /// @param[in]  asset_id - The ID of the texture.
    /// @param[in]  binary_data - The binary data of the texture.
    ///     Must be in one of the formats supported by SFML
    ///     (https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Image.php#a9e4f2aa8e36d0cabde5ed5a4ef80290b).
    void GraphicsDevice::LoadTexture(const RESOURCES::AssetId asset_id, const std::string& binary_data)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(Mutex);

        // LOAD THE TEXTURE INTO THE DEVICE IF POSSIBLE.
        std::shared_ptr<GRAPHICS::Texture> texture = GRAPHICS::Texture::LoadFromMemory(binary_data);
        if (texture)
        {
            Textures.emplace(asset_id, texture);
        }
    }

    /// Gets the texture corresponding to the provided asset ID.
    /// @param[in]  asset_id - The ID of the texture.
    /// @return The texture corresponding to the specified texture ID.
    ///     If a texture with the specified ID hasn't yet been loaded into the graphics device,
    ///     then a new texture will be allocated for returning in order to assist in debugging
    ///     and allow the game to still run until the texture is loaded.
    MEMORY::NonNullSharedPointer<GRAPHICS::Texture> GraphicsDevice::GetTexture(const RESOURCES::AssetId asset_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        
        // CHECK IF THE REQUESTED TEXTURE'S ALREADY BEEN LOADED.
        bool texture_loaded = Textures.contains(asset_id);
        if (texture_loaded)
        {
            // RETURN THE PREVIOUSLY LOADED TEXTURE.
            MEMORY::NonNullSharedPointer<GRAPHICS::Texture> texture = Textures.at(asset_id);
            return texture;
        }
        else
        {
            // CREATE A NEW TEXTURE TO TEMPORARILY USE UNTIL A PROPER ONE IS LOADED.
            // The size is arbitrary, but the color is chosen to be based on the asset ID to
            // help differentiate between different debug textures.
            constexpr unsigned int DEBUG_TEXTURE_DIMENSION_IN_PIXELS = 8;
            uint8_t debug_texture_color_component = static_cast<uint8_t>(asset_id);
            Color debug_texture_color(debug_texture_color_component, debug_texture_color_component, debug_texture_color_component);
            MEMORY::NonNullSharedPointer<GRAPHICS::Texture> debug_texture = GRAPHICS::Texture::CreateSolid(
                DEBUG_TEXTURE_DIMENSION_IN_PIXELS,
                DEBUG_TEXTURE_DIMENSION_IN_PIXELS,
                debug_texture_color);

            // The debug texture is stored for future retrieval without needing to recreate it.
            // If a proper texture gets loaded eventually, then it should overwrite this texture.
            Textures.emplace(asset_id, debug_texture);

            return debug_texture;
        }
    }

    /// Attempts to load a shader into the graphics device.
    /// @param[in]  asset_id - The ID of the shader being loaded.
    /// @param[in]  shader_type - The type of the shader being loaded.
    /// @param[in]  binary_data - The raw binary data of the shader source code.
    void GraphicsDevice::LoadShader(const RESOURCES::AssetId asset_id, const sf::Shader::Type shader_type, const std::string& binary_data)
    {
        // STORE THE SHADER IF IT CAN BE LOADED.
        std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
        bool shader_loaded = shader->loadFromMemory(binary_data, shader_type);
        if (shader_loaded)
        {
            Shaders.emplace(asset_id, shader);
        }
    }

    /// Attempts to get the specified shader previously loaded into the graphics device.
    /// @param[in]  asset_id - The ID of the shader to load.
    /// @return The shader, if successfully found; null otherwise.
    std::shared_ptr<sf::Shader> GraphicsDevice::GetShader(const RESOURCES::AssetId asset_id)
    {
        // RETURN THE SHADER ONLY IF IT'S BEEN LOADED.
        bool shader_loaded = Shaders.contains(asset_id);
        if (shader_loaded)
        {
            MEMORY::NonNullSharedPointer<sf::Shader> shader = Shaders.at(asset_id);
            return shader;
        }
        else
        {
            return nullptr;
        }
    }
}
