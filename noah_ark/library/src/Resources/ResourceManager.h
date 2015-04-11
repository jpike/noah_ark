#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>
#include <SFML/Graphics.hpp>
#include "Graphics/AnimationSequence.h"

/// Holds code for dealing with resources (assets) in the game.
namespace RESOURCES
{
    ///////////////////////////////////////////////////////////
    /// @brief  Manages loading of resources.
    ///////////////////////////////////////////////////////////
    class ResourceManager
    {
    public:
        /// @brief      Constructor.
        /// @param[in]  resourcesFilepath - Filepath to the JSON-formatted file describing all resources.
        /// @throws     boost::property_tree::json_parser::json_parser_error - Thrown if the file at
        ///             the provided path is ill-formed.
        explicit ResourceManager(const std::string& resourcesFilepath);
        /// @brief  Destructor.
        ~ResourceManager();

        /// @brief      Retrieves the texture resource with the given name.
        /// @param[in]  textureName - The name of the texture resource to retrieve.
        /// @return     The texture resource, if successfully retrieved; nullptr otherwise.
        std::shared_ptr<sf::Texture> GetTexture(const std::string& textureName);

        /// @brief      Retrieves the sprite resource with the given name.
        /// @param[in]  spriteName - The name of the sprite resource to retrieve.
        /// @return     The sprite resource, if successfully retrieved; nullptr otherwise.
        std::shared_ptr<sf::Sprite> GetSprite(const std::string& spriteName);

        /// @brief      Retrieves the animation resource with the given name.
        /// @param[in]  animationName - The name of the animation resource to retrieve.
        /// @return     The animation resource, if successfully retrieved; nullptr otherwise.
        std::shared_ptr<GRAPHICS::AnimationSequence> GetAnimation(const std::string& animationName);

    private:
        ResourceManager(const ResourceManager& managerToCopy); ///< Private to disallow copying.
        ResourceManager& operator= (const ResourceManager& rhsManager);    ///< Private to disallow assignment.

        /// @brief      Helper method to parse frame information from the provided frame properties node.
        /// @param[in]  frameProperties - The frame properties to parse.  Should be an entry in the "frames" array of the JSON.
        /// @return     The rectangle parsed from the frame properties.
        /// @throws     boost::property_tree::ptree_error - Thrown if a parsing error occurs.
        sf::IntRect ParseAnimationFrame(const boost::property_tree::ptree::value_type& frameProperties);

        boost::property_tree::ptree m_resourceTree; ///< The tree of resource information.

        /// Textures that have been loaded.  They need to remain in memory to allow them to be used.
        /// They are mapped by the texture resource names.
        std::unordered_map< std::string, std::shared_ptr<sf::Texture> > m_textures;
    };
}