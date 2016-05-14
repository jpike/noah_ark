#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include "Audio/SoundEffect.h"
#include "Core/Array2D.h"
#include "Graphics/AnimationSequence.h"
#include "Graphics/Font.h"
#include "Graphics/Texture.h"
#include "Maps/OverworldMapFile.h"
#include "Maps/TileMapFile.h"
#include "Maps/Tileset.h"

namespace RESOURCES
{
    // ASSET IDs.
    extern const std::string AXE_TEXTURE_ID;
    extern const std::string DUST_CLOUD_TEXTURE_ID;
    extern const std::string FONT_TEXTURE_ID;
    extern const std::string GROUND_TILESET_TEXTURE_ID;
    extern const std::string NOAH_TEXTURE_ID;
    extern const std::string TREE_TEXTURE_ID;
    extern const std::string WOOD_LOG_TEXTURE_ID;
    extern const std::string DUST_CLOUD_ANIMATION_ID;
    extern const std::string NOAH_WALK_FRONT_ANIMATION_ID;
    extern const std::string NOAH_WALK_BACK_ANIMATION_ID;
    extern const std::string NOAH_WALK_LEFT_ANIMATION_ID;
    extern const std::string NOAH_WALK_RIGHT_ANIMATION_ID;
    extern const std::string AXE_HIT_SOUND_ID;
    extern const std::string TREE_SHAKE_SOUND_ID;

    /// Provides access to all assets needed for the game.
    /// When first requesting an asset, it will be loaded
    /// from the filesystem.  Afterwards, it may be cached
    /// in-memory within this class (but this is not required).
    class Assets
    {
    public:
        /// Constructor to initialize an empty collection of assets.
        explicit Assets();

        /// Loads all assets needed for the game.
        /// @return True if loading completely succeeds; false otherwise.
        bool LoadAll();

        /// Gets a tileset created from the provided descriptions.
        /// @param[in]  tileset_descriptions - Descriptions of the tileset to create.
        /// @return The tileset created from the provided descriptions; null if an error occurs.
        std::shared_ptr<MAPS::Tileset> GetTileset(const std::vector<MAPS::TilesetDescription>& tileset_descriptions);

        /// Attempts to retrieve the texture identified by the specified ID.
        /// @param[in]  texture_id - The ID of the texture to load.
        /// @return The requested texture, if successfully loaded; null otherwise.
        std::shared_ptr<GRAPHICS::Texture> GetTexture(const std::string& texture_id);

        /// Attempts to retrieve the animation sequence identified by the specified ID.
        /// @param[in]  animation_id - The ID of the animation sequence to load.
        /// @return The requested animation sequence, if successfully loaded; null otherwise.
        std::shared_ptr<GRAPHICS::AnimationSequence> GetAnimationSequence(const std::string& animation_id);

        /// Attempts to retrieve the font identified by the specified texture ID.
        /// @param[in]  font_texture_id - The ID of the texture associated with the font.
        /// @return The requested font, if successfully loaded; null otherwise.
        std::shared_ptr<GRAPHICS::Font> GetFont(const std::string& font_texture_id);

        /// Attempts to retrieve the sound effect identified by the specified ID.
        /// The returned sound effect will be a new instance of a sound effect,
        /// but it may share the same buffer of audio samples as another instance.
        /// @param[in]  sound_id - The ID of the sound to load.
        /// @return The requested sound effect, if successfully loaded; null otherwise.
        std::shared_ptr<AUDIO::SoundEffect> GetSound(const std::string& sound_id);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// Textures that have been loaded.  They need to remain in memory to allow them to be used.
        /// They are mapped by the texture resource IDs.
        std::unordered_map< std::string, std::shared_ptr<GRAPHICS::Texture> > Textures;
        /// Audio samples for sounds that have been loaded.  They need to remain in memory to allow them to be used.
        /// They are mapped by the sound resource IDs.
        std::unordered_map< std::string, std::shared_ptr<sf::SoundBuffer> > AudioSamples;
        /// The overworld map file, if successfully loaded.
        std::unique_ptr<MAPS::OverworldMapFile> OverworldMapFile;
        /// The tile map files, mapped by their 2D grid coordinates (x = column, y = row) in the overworld.
        /// Positioning starts at (0,0) at the top-left of the overworld.
        CORE::Array2D< std::unique_ptr<MAPS::TileMapFile> > TileMapFiles;

    private:
        // LOADING HELPER METHODS.
        /// Attempts to load all textures into this collection of assets.
        /// Any previous textures will be cleared.
        /// @return True if all textures are loaded; false otherwise.
        bool LoadTextures();
        /// Attempts to load all sound resources into this collection of assets.
        /// Any previous sound resources will be cleared.
        /// @return True if all sounds are loaded; false otherwise.
        bool LoadSounds();
        /// Attempts to load the overworld map file into this collection of assets.
        /// Any previous overworld map file will be cleared.
        /// @return True if the overworld map file is loaded; false otherwise.
        bool LoadOverworldMapFile();
        /// Attempts to load the all tile map files in the overworld map file into this collection of assets.
        /// Any previous tile map files will be cleared.
        /// @return True if the all tile map files are loaded; false otherwise.
        bool LoadTileMapFiles();
    };
}