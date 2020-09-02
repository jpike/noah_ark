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
            Textures[asset_id] = texture;
        }
    }
}
