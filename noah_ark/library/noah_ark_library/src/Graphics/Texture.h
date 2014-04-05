#pragma once

#include <hge.h>

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A texture resource.
    ///////////////////////////////////////////////////////////
    class Texture
    {
    public:
        static const HTEXTURE INVALID_TEXTURE_HANDLE;   ///< A constant for an invalid handle.
        
        /// @brief      Constructor.
        /// @param[in]  textureHandle - The HGE texture handle to the underlying resource.
        ///             An invalid handle is allowed to be passed here to allow "empty"
        ///             textures to be created (an expected use case).
        explicit Texture(const HTEXTURE textureHandle);
        /// @brief      Copy constructor.
        /// @param[in]  textureToCopy - The texture to copy.
        explicit Texture(const Texture& textureToCopy);
        /// @brief      Destructor.
        ~Texture();

        /// @brief      Assignment operator.
        /// @param[in]  rhsTexture - The texture to assign from.
        Texture& operator= (const Texture& rhsTexture);

        /// @brief      Returns the HGE texture handle to the underlying resource.
        /// @return     The HGE texture handle.
        const HTEXTURE GetTextureHandle() const;

    private:
        /// @brief      Helper method for copying a texture.
        /// @param[in]  textureToCopy - The texture to copy.
        void Copy(const Texture& textureToCopy);

        HTEXTURE m_textureHandle;   ///< The handle to the HGE texture resource.
    };
}