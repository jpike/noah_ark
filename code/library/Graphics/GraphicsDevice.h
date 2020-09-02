#pragma once

#include <mutex>
#include <unordered_map>
#include "Graphics/Texture.h"
#include "Memory/Pointers.h"
#include "Resources/AssetId.h"

namespace GRAPHICS
{
    /// A graphics device (also known as a graphics card or GPU).
    class GraphicsDevice
    {
    public:
        // PUBLIC METHODS.
        void LoadTexture(const RESOURCES::AssetId asset_id, const std::string& binary_data);
        
    private:
        // PRIVATE MEMBER VARIABLES.
        /// A mutex to provide thread-safety for this class.
        std::recursive_mutex Mutex = {};
        /// Textures to use for rendering.
        std::unordered_map<RESOURCES::AssetId, MEMORY::NonNullSharedPointer<GRAPHICS::Texture>> Textures = {};
    };
}
