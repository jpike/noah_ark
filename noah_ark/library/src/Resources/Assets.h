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
#include "Maps/OverworldMapFile.h"
#include "Maps/TileMapFile.h"
#include "Maps/Tileset.h"

/// Holds code related to resources/assets for the game.
namespace RESOURCES
{
    // ASSET IDs.
    extern const std::string ARK_TEXTURE_ID;
    extern const std::string AXE_TEXTURE_ID;
    extern const std::string DUST_CLOUD_TEXTURE_ID;
    extern const std::string FONT_TEXTURE_ID;
    extern const std::string GROUND_TILESET_TEXTURE_ID;
    extern const std::string NOAH_TEXTURE_ID;
    extern const std::string TREE_TEXTURE_ID;
    extern const std::string WOOD_LOG_TEXTURE_ID;
    extern const std::string DUST_CLOUD_ANIMATION_ID;
    extern const std::string AXE_HIT_SOUND_ID;
    extern const std::string COLLECT_BIBLE_VERSE_SOUND_ID;
    extern const std::string TREE_SHAKE_SOUND_ID;
    extern const std::string OVERWORLD_BACKGROUND_MUSIC_ID;

    /// Defines all of the available shaders.
    enum class ShaderId
    {
        /// A shader that tints a texture based on a color parameter.
        COLORED_TEXTURE
    };

    /// Provides access to all assets needed for the game.
    /// When first requesting an asset, it will be loaded
    /// from the filesystem.  Afterwards, it may be cached
    /// in-memory within this class (but this is not required).
    class Assets
    {
    public:
        // ASSET RETRIEVAL.
        std::shared_ptr<GRAPHICS::Texture> GetTexture(const std::string& texture_id);
        std::shared_ptr<GRAPHICS::GUI::Font> GetFont(const std::string& font_texture_id);
        std::shared_ptr<sf::Shader> GetShader(const ShaderId shader_id) const;
        std::shared_ptr<AUDIO::SoundEffect> GetSound(const std::string& sound_id);
        std::shared_ptr<sf::Music> GetMusic(const std::string& music_id);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Textures that have been loaded.  They need to remain in memory to allow them to be used.
        /// They are mapped by the texture resource IDs.
        std::unordered_map< std::string, std::shared_ptr<GRAPHICS::Texture> > Textures = {};
        /// Fonts that have been loaded.
        std::unordered_map< std::string, std::shared_ptr<GRAPHICS::GUI::Font> > Fonts = {};
        /// Shaders that have been loaded.  They need to remain in memory to allow them to be used.
        /// They are mapped by shader ID.
        std::unordered_map< ShaderId, std::shared_ptr<sf::Shader> > Shaders = {};
        /// Audio samples for sounds that have been loaded.  They need to remain in memory to allow them to be used.
        /// They are mapped by the sound resource IDs.
        std::unordered_map< std::string, std::shared_ptr<sf::SoundBuffer> > AudioSamples = {};
        /// Music that has been loaded.
        std::unordered_map< std::string, std::shared_ptr<sf::Music> > Music = {};
    };
}