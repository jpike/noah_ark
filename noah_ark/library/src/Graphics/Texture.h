#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

namespace GRAPHICS
{
    ///////////////////////////////////////////////////////////
    /// @brief  A texture resource.
    ///////////////////////////////////////////////////////////
    class Texture
    {
    public:        
        /// @brief      Constructor.
        /// @param[in]  textureResource - The SFML texture that is the underlying resource.
        explicit Texture(const std::shared_ptr<sf::Texture>& textureResource);
        /// @brief      Copy constructor.
        /// @param[in]  textureToCopy - The texture to copy.
        explicit Texture(const Texture& textureToCopy);
        /// @brief      Destructor.
        ~Texture();

        /// @brief      Assignment operator.
        /// @param[in]  rhsTexture - The texture to assign from.
        Texture& operator= (const Texture& rhsTexture);

        /// @brief      Returns the underlying SFML texture resource.
        /// @return     The SFML texture resource.
        const std::shared_ptr<sf::Texture> GetTextureResource() const;

    private:
        /// @brief      Helper method for copying a texture.
        /// @param[in]  textureToCopy - The texture to copy.
        void Copy(const Texture& textureToCopy);

        std::shared_ptr<sf::Texture> m_textureResource;   ///< The underlying SFML texture resource.
    };
}