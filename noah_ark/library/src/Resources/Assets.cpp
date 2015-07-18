#include <cassert>
#include <cstdint>
#include "Resources/Assets.h"

namespace RESOURCES
{
    // ASSET IDs.
    // For assets that require accessing the filesystem, IDs are currently set to filepaths
    // (relative to the working directory) to simplify loading.
    const std::string AXE_TEXTURE_ID = "res/images/noah_sprite1.png";
    const std::string TREE_TEXTURE_ID = "res/images/tree_sprite1.png";
    const std::string AXE_SWING_RIGHT_ANIMATION_ID = "axe_swing_right";
    const std::string AXE_SWING_LEFT_ANIMATION_ID = "axe_swing_left";
    const std::string AXE_SWING_UP_ANIMATION_ID = "axe_swing_up";
    const std::string AXE_SWING_DOWN_ANIMATION_ID = "axe_swing_down";

    Assets::Assets() :
    Textures()
    {}

    std::shared_ptr<GRAPHICS::Texture> Assets::GetTexture(const std::string& texture_id)
    {
        // RETURN THE TEXTURE IF IT HAS ALREADY BEEN LOADED.
        auto id_with_texture = Textures.find(texture_id);
        bool texture_already_loaded = (Textures.cend() != id_with_texture);
        if (texture_already_loaded)
        {
            return id_with_texture->second;
        }

        // LOAD THE IMAGE FOR THE TEXTURE.
        // The transparent color can only be set on an image.
        // Texture IDs are currently directly mapped to filepaths.
        sf::Image image;
        bool image_loaded = image.loadFromFile(texture_id);
        if (!image_loaded)
        {
            /// @todo   Logging?  Or returning a null pointer?
            // CREATE A DUMMY IMAGE FOR DEBUGGING.
            // To allow the game to continue to run (rather than fail),
            // a dummy image is created with a specific color to make
            // it easy to debug.
            const unsigned int DUMMY_IMAGE_WIDTH_IN_PIXELS = 8;
            const unsigned int DUMMY_IMAGE_HEIGHT_IN_PIXELS = 8;
            const sf::Color DUMMY_IMAGE_COLOR = sf::Color::Cyan;
            image.create(DUMMY_IMAGE_WIDTH_IN_PIXELS, DUMMY_IMAGE_HEIGHT_IN_PIXELS, DUMMY_IMAGE_COLOR);
        }

        // ADD TRANSPARENCY TO THE IMAGE.
        // All images are expected to use magenta (R=255, G=0, B=255) for their parts
        // that should be transparent.
        const sf::Color TRANSPARENT_COLOR = sf::Color::Magenta;
        const uint8_t MAKE_COMPLETELY_TRANSPARENT = 0;
        image.createMaskFromColor(TRANSPARENT_COLOR, MAKE_COMPLETELY_TRANSPARENT);

        // CREATE THE TEXTURE FROM THE IMAGE.
        std::shared_ptr<sf::Texture> texture_resource = std::make_shared<sf::Texture>();
        bool texture_loaded_from_image = texture_resource->loadFromImage(image);
        if (!texture_loaded_from_image)
        {
            /// @todo   Logging?  Or returning a null pointer?
            assert(texture_loaded_from_image);
            // Continue processing so that the calling code can continue to execute.
            // It will just have an "empty" texture tohave to deal with.
        }

        // CACHE THE TEXTURE IN THIS COLLECTION OF ASSETS.
        std::shared_ptr<GRAPHICS::Texture> texture = std::make_shared<GRAPHICS::Texture>(texture_resource);
        Textures[texture_id] = texture;

        return texture;
    }

    std::shared_ptr<GRAPHICS::AnimationSequence> Assets::GetAnimationSequence(const std::string& animation_id)
    {
        if (AXE_SWING_RIGHT_ANIMATION_ID == animation_id)
        {
            /// @todo   This should go in a file at some point.
            const sf::Time DURATION = sf::seconds(3.0f);
            const bool NOT_LOOPING = false;
            // Left, Top, Width, Height
            const std::vector<sf::IntRect> FRAMES =
            {
                sf::IntRect(32, 53, 9, 11),
                sf::IntRect(32, 64, 11, 9),
                sf::IntRect(32, 53, 9, 11)
            };

            std::shared_ptr<GRAPHICS::AnimationSequence> animation = std::make_shared<GRAPHICS::AnimationSequence>(
                AXE_SWING_RIGHT_ANIMATION_ID,
                NOT_LOOPING,
                DURATION,
                FRAMES);
            return animation;
        }
        else if (AXE_SWING_LEFT_ANIMATION_ID == animation_id)
        {
            /// @todo   This should go in a file at some point.
            const sf::Time DURATION = sf::seconds(3.0f);
            const bool NOT_LOOPING = false;
            // Left, Top, Width, Height
            const std::vector<sf::IntRect> FRAMES =
            {
                sf::IntRect(3, 66, 9, 11),
                sf::IntRect(12, 68, 11, 9),
                sf::IntRect(3, 66, 9, 11)
            };

            std::shared_ptr<GRAPHICS::AnimationSequence> animation = std::make_shared<GRAPHICS::AnimationSequence>(
                AXE_SWING_LEFT_ANIMATION_ID,
                NOT_LOOPING,
                DURATION,
                FRAMES);
            return animation;
        }
        else if (AXE_SWING_UP_ANIMATION_ID == animation_id)
        {
            /// @todo   This should go in a file at some point.
            const sf::Time DURATION = sf::seconds(3.0f);
            const bool NOT_LOOPING = false;
            // Left, Top, Width, Height
            const std::vector<sf::IntRect> FRAMES =
            {
                sf::IntRect(3, 66, 9, 11),
                sf::IntRect(12, 68, 11, 9),
                sf::IntRect(3, 66, 9, 11)
            };

            std::shared_ptr<GRAPHICS::AnimationSequence> animation = std::make_shared<GRAPHICS::AnimationSequence>(
                AXE_SWING_UP_ANIMATION_ID,
                NOT_LOOPING,
                DURATION,
                FRAMES);
            return animation;
        }
        else if (AXE_SWING_DOWN_ANIMATION_ID == animation_id)
        {
            /// @todo   This should go in a file at some point.
            const sf::Time DURATION = sf::seconds(3.0f);
            const bool NOT_LOOPING = false;
            // Left, Top, Width, Height
            const std::vector<sf::IntRect> FRAMES =
            {
                sf::IntRect(32, 41, 9, 11),
                sf::IntRect(32, 32, 11, 9),
                sf::IntRect(32, 41, 9, 11)
            };

            std::shared_ptr<GRAPHICS::AnimationSequence> animation = std::make_shared<GRAPHICS::AnimationSequence>(
                AXE_SWING_DOWN_ANIMATION_ID,
                NOT_LOOPING,
                DURATION,
                FRAMES);
            return animation;
        }
        else
        {
            /// @todo   Logging?  Right now, we're just making things fail
            /// easily due to an supported animation being specified.
            assert(false);
            return nullptr;
        }
    }
}