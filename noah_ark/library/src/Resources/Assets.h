#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Audio/SoundEffect.h"
#include "Core/Array2D.h"
#include "Graphics/AnimationSequence.h"
#include "Graphics/Texture.h"
#include "Graphics/Gui/Font.h"
#include "Resources/AssetPackage.h"

/// Holds code related to resources/assets for the game.
namespace RESOURCES
{
    /// Provides access to all assets needed for the game.
    /// When first requesting an asset, it will be loaded
    /// from the filesystem.  Afterwards, it may be cached
    /// in-memory within this class (but this is not required).
    class Assets
    {
    public:
        // ASSET LOADING.
        bool Load(const AssetPackage& asset_package);

        // ASSET RETRIEVAL.
        std::shared_ptr<GRAPHICS::Texture> GetTexture(const AssetId texture_id);
        std::shared_ptr<GRAPHICS::GUI::Font> GetFont(const AssetId font_texture_id);
        std::shared_ptr<sf::Shader> GetShader(const AssetId shader_id);
        std::shared_ptr<sf::SoundBuffer> GetSound(const AssetId sound_id);
        std::shared_ptr<sf::Music> GetMusic(const AssetId music_id);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Textures that have been loaded.  They need to remain in memory to allow them to be used.
        std::unordered_map< AssetId, std::shared_ptr<GRAPHICS::Texture> > Textures = {};
        /// Fonts that have been loaded.
        std::unordered_map< AssetId, std::shared_ptr<GRAPHICS::GUI::Font> > Fonts = {};
        /// Shaders that have been loaded.  They need to remain in memory to allow them to be used.
        std::unordered_map< AssetId, std::shared_ptr<sf::Shader> > Shaders = {};
        /// Audio samples for sounds that have been loaded.  They need to remain in memory to allow them to be used.
        /// They are mapped by the sound resource IDs.
        std::unordered_map< AssetId, std::shared_ptr<sf::SoundBuffer> > AudioSamples = {};
        /// Music that has been loaded.
        std::unordered_map< AssetId, std::shared_ptr<sf::Music> > Music = {};

    private:
        // MEMBER VARIABLES.
        /// A mutex to provide thread-safety for this class.
        std::recursive_mutex AssetMutex = {};
    };
}