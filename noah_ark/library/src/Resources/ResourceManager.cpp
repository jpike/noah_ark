#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Resources/ResourceManager.h"

using namespace RESOURCES;

ResourceManager::ResourceManager(const std::string& resourcesFilepath) :
    m_textures()
{
    // PARSE THE JSON RESOURCES FILE.
    // The boost parser may throw an exception, which we intentionally allow to escape.
    boost::property_tree::json_parser::read_json(resourcesFilepath, m_resourceTree);
}

ResourceManager::~ResourceManager()
{}

std::shared_ptr<sf::Texture> ResourceManager::GetTexture(const std::string& textureName)
{
    try
    {
        // CHECK IF THIS TEXTURE RESOURCE HAS ALREADY BEEN LOADED.
        auto preloadedTextureResource = m_textures.find(textureName);
        bool textureAlreadyLoaded = (preloadedTextureResource != m_textures.cend());
        if (textureAlreadyLoaded)
        {
            return preloadedTextureResource->second;
        }
        
        // GET THE FILEPATH FOR THE SPECIFIED TEXTURE.
        // Form the full path to the texture resource.
        const std::string TEXTURE_RESOURCE_PREFIX = "textures.";
        const std::string TEXTURE_RESOURCE_FILEPATH_PROPERTY_SUFFIX = ".filepath";
        std::string textureResourcePath = TEXTURE_RESOURCE_PREFIX + textureName + TEXTURE_RESOURCE_FILEPATH_PROPERTY_SUFFIX;
        // Retrieve the texture filepath.
        std::string textureFilepath = m_resourceTree.get<std::string>(textureResourcePath);
        bool textureFilepathFound = !textureFilepath.empty();
        if (!textureFilepathFound)
        {
            // We can't return a texture without a filepath.
            return nullptr;
        }

        // LOAD THE TEXTURE FROM FILE.
        std::shared_ptr<sf::Texture> textureResource = std::make_shared<sf::Texture>();
        bool textureLoadedSuccessfully = textureResource->loadFromFile(textureFilepath);
        if (textureLoadedSuccessfully)
        {
            // Store the texture in the resource manager so that we re-retrieve it.
            m_textures[textureName] = textureResource;

            return textureResource;
        }
        else
        {
            return nullptr;
        }
    }
    catch (const boost::property_tree::ptree_error& )
    {
        // The texture resource couldn't be found.
        return nullptr;
    }
}

std::shared_ptr<sf::Sprite> ResourceManager::GetSprite(const std::string& spriteName)
{
    try
    {
        // CREATE THE BASE PATH TO THE SPRITE RESOURCE.
        const std::string SPRITE_RESOURCE_PREFIX = "sprites.";
        std::string spriteResourcePath = SPRITE_RESOURCE_PREFIX + spriteName;

        // GET THE TEXTURE FOR THE SPRITE.
        // Get the name of the texture resource.
        const std::string TEXTURE_RESOURCE_NAME_SUFFIX = ".texture";
        std::string spriteTexturePath = spriteResourcePath + TEXTURE_RESOURCE_NAME_SUFFIX;
        std::string spriteTextureName = m_resourceTree.get<std::string>(spriteTexturePath);
        // Load the texture.
        std::shared_ptr<sf::Texture> spriteTexture = GetTexture(spriteTextureName);
        bool textureLoadedSuccessfully = (nullptr != spriteTexture);
        if (!textureLoadedSuccessfully)
        {
            // A texture is required for the sprite.
            return nullptr;
        }

        // GET THE CENTER OF THE SPRITE.
        const std::string CENTER_SUFFIX = ".center";
        std::string spriteCenterPath = spriteResourcePath + CENTER_SUFFIX;
        // Get the center X coordinate.
        const std::string CENTER_X_SUFFIX = ".x";
        std::string spriteCenterXPath = spriteCenterPath + CENTER_X_SUFFIX;
        float spriteCenterX = m_resourceTree.get<float>(spriteCenterXPath);
        // Get the center Y coordinate.
        const std::string CENTER_Y_SUFFIX = ".y";
        std::string spriteCenterYPath = spriteCenterPath + CENTER_Y_SUFFIX;
        float spriteCenterY = m_resourceTree.get<float>(spriteCenterYPath);

        // GET THE SIZE OF THE SPRITE.
        const std::string SIZE_SUFFIX = ".size";
        std::string spriteSizePath = spriteResourcePath + SIZE_SUFFIX;
        // Get the width.
        const std::string WIDTH_SUFFIX = ".width";
        std::string spriteWidthPath = spriteSizePath + WIDTH_SUFFIX;
        int spriteWidth = m_resourceTree.get<int>(spriteWidthPath);
        // Get the height.
        const std::string HEIGHT_SUFFIX = ".height";
        std::string spriteHeightPath = spriteSizePath + HEIGHT_SUFFIX;
        int spriteHeight = m_resourceTree.get<int>(spriteHeightPath);

        // Create the rectangle for the sprite's texture.
        // It will start off in the top-left corner of the texture.
        const int LEFT_CORNER = 0;
        const int TOP_CORNER = 0;
        sf::IntRect textureRectangle(LEFT_CORNER, TOP_CORNER, spriteWidth, spriteHeight);

        // CREATE THE SPRITE RESOURCE TO RETURN.
        std::shared_ptr<sf::Sprite> spriteResource = std::make_shared<sf::Sprite>(
            *spriteTexture,
            textureRectangle);

        // Set the center of the sprite.
        spriteResource->setOrigin(spriteCenterX, spriteCenterY);

        return spriteResource;
    }
    catch (const boost::property_tree::ptree_error&)
    {
        // The sprite resource couldn't be found.
        return nullptr;
    }
}

std::shared_ptr<GRAPHICS::AnimationSequence> ResourceManager::GetAnimation(const std::string& animationName)
{
    try
    {
        // CREATE THE BASE PATH TO THE ANIMATION RESOURCE.
        const std::string ANIMATION_RESOURCE_PREFIX = "animations.";
        std::string animationResourcePath = ANIMATION_RESOURCE_PREFIX + animationName;
        
        // RETRIEVE THE LOOPING PROPERTY.
        const std::string LOOPING_PROPERTY_SUFFIX = ".loop";
        std::string loopingPropertyPath = animationResourcePath + LOOPING_PROPERTY_SUFFIX;
        bool isLooping = m_resourceTree.get<bool>(loopingPropertyPath);

        // RETRIEVE THE DURATION IN SECONDS.
        const std::string DURATION_PROPERTY_SUFFIX = ".duration_in_seconds";
        std::string durationPropertyPath = animationResourcePath + DURATION_PROPERTY_SUFFIX;
        float durationInSeconds = m_resourceTree.get<float>(durationPropertyPath);

        // RETRIEVE THE FRAMES.
        const std::string FRAMES_PROPERTY_SUFFIX = ".frames";
        std::string framesPropertyPath = animationResourcePath + FRAMES_PROPERTY_SUFFIX;
        std::vector<MATH::IntRectangle> frames;
        BOOST_FOREACH(const boost::property_tree::ptree::value_type& frameProperties, m_resourceTree.get_child(framesPropertyPath))
        {
            // CREATE A FRAME FROM THE CURRENT PROPERTIES.
            MATH::IntRectangle frame = ParseAnimationFrame(frameProperties);

            // ADD THE FRAME TO THE ANIMATION.
            frames.push_back(frame);
        }

        // CREATE THE ANIMATION SEQUENCE.
        std::shared_ptr<GRAPHICS::AnimationSequence> animationSequence = std::make_shared<GRAPHICS::AnimationSequence>(
            animationName,
            isLooping,
            sf::seconds(durationInSeconds),
            frames);

        return animationSequence;
    }
    catch (const boost::property_tree::ptree_error&)
    {
        // The animation resource couldn't be found.
        return nullptr;
    }
}

MATH::IntRectangle ResourceManager::ParseAnimationFrame(const boost::property_tree::ptree::value_type& frameProperties)
{
    // PARSE EACH VALUE FROM THE FRAME PROPERTIES.
    const std::string LEFT_PROPERTY_NAME = "left";
    int left = frameProperties.second.get<int>(LEFT_PROPERTY_NAME);

    const std::string TOP_PROPERTY_NAME = "top";
    int top = frameProperties.second.get<int>(TOP_PROPERTY_NAME);

    const std::string WIDTH_PROPERTY_NAME = "width";
    int width = frameProperties.second.get<int>(WIDTH_PROPERTY_NAME);

    const std::string HEIGHT_PROPERTY_NAME = "height";
    int height = frameProperties.second.get<int>(HEIGHT_PROPERTY_NAME);

    // CREATE A RECTANGLE FOR THE FRAME VALUES.
    MATH::IntRectangle frameRectangle = MATH::IntRectangle::FromTopLeftAndDimensions(left, top, width, height);
    return frameRectangle;
}
