#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "Graphics/AnimationSequence.h"
#include "Graphics/Texture.h"

namespace RESOURCES
{
    // ASSET IDs.
    extern const std::string AXE_TEXTURE_ID;
    extern const std::string TREE_TEXTURE_ID;
    extern const std::string AXE_SWING_RIGHT_ANIMATION_ID;
    extern const std::string AXE_SWING_LEFT_ANIMATION_ID;
    extern const std::string AXE_SWING_UP_ANIMATION_ID;
    extern const std::string AXE_SWING_DOWN_ANIMATION_ID;

    /// Provides access to all assets needed for the game.
    /// When first requesting an asset, it will be loaded
    /// from the filesystem.  Afterwards, it may be cached
    /// in-memory within this class (but this is not required).
    class Assets
    {
    public:
        /// Constructor to initialize an empty collection of assets.
        explicit Assets();

        /// Attempts to retrieve the texture identified by the specified ID.
        /// @param[in]  texture_id - The ID of the texture to load.
        /// @return The requested texture, if successfully loaded; null otherwise.
        std::shared_ptr<GRAPHICS::Texture> GetTexture(const std::string& texture_id);

        /// Attempts to retrieve the animation sequence identified by the specified ID.
        /// @param[in]  animation_id - The ID of the animation sequence to load.
        /// @return The requested animation sequence, if successfully loaded; null otherwise.
        std::shared_ptr<GRAPHICS::AnimationSequence> GetAnimationSequence(const std::string& animation_id);

    private:

        /// Textures that have been loaded.  They need to remain in memory to allow them to be used.
        /// They are mapped by the texture resource IDs.
        std::unordered_map< std::string, std::shared_ptr<GRAPHICS::Texture> > Textures;
    };
}