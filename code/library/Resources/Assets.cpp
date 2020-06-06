#include <cstdint>
#include <cstring>
#include <future>
#include <vector>
#include "Debugging/DebugConsole.h"
#include "Resources/Assets.h"

namespace RESOURCES
{
    /// Loads assets in the specified asset package into this collection.
    /// @param[in]  asset_package - The package of assets to load.
    /// @return True if all assets in the package are loaded successfully; false otherwise.
    bool Assets::Load(const AssetPackageDefinition& asset_package)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // LOAD EACH ASSET IN THE PACKAGE.
        for (const AssetDefinition& asset_definition : asset_package.Assets)
        {
            // LOAD THE ASSET BASED ON IT'S TYPE.
            bool asset_loaded = false;
            switch (asset_definition.Type)
            {
                case AssetType::FONT:
                    // LOAD THE FONT.
                    asset_loaded = static_cast<bool>(GetFont(asset_definition.Id));
                    break;
                case AssetType::MUSIC:
                    // LOAD THE MUSIC.
                    asset_loaded = static_cast<bool>(GetMusic(asset_definition.Id));
                    break;
                case AssetType::SHADER:
                    // LOAD THE SHADER.
                    asset_loaded = static_cast<bool>(GetShader(asset_definition.Id));
                    break;
                default:
                    // The asset can be loaded if it's type isn't known.
                    asset_loaded = false;
            }

            // ABORT IF THE ASSET FAILED TO BE LOADED.
            if (!asset_loaded)
            {
                return false;
            }
        }

        // INDICATE THAT ALL ASSETS WERE LOADED SUCCESSFULLY.
        return true;
    }

    /// Populates this collection of assets with more specific types
    /// of data as specified in the generic collection of assets.
    /// @param[in]  assets - The generic asset data to populate from.
    /// @return True if all assets are successfully populated; false otherwise.
    bool Assets::Populate(const std::vector<Asset>& assets)
    {
        for (const auto& asset : assets)
        {
            switch (asset.Type)
            {
                case RESOURCES::AssetType::MUSIC:
                {
                    std::shared_ptr<sf::Music> music = GetMusic(asset.Id, asset.BinaryData);
                    if (!music)
                    {
                        return false;
                    }
                    break;
                }
                case RESOURCES::AssetType::SHADER:
                {
                    std::shared_ptr<sf::Shader> shader = GetShader(asset.Id, asset.BinaryData);
                    if (!shader)
                    {
                        return false;
                    }
                    break;
                }
                case RESOURCES::AssetType::SOUND_EFFECT:
                {
                    std::shared_ptr<sf::SoundBuffer> sound = GetSound(asset.Id, asset.BinaryData);
                    if (!sound)
                    {
                        return false;
                    }
                    break;
                }
                case RESOURCES::AssetType::TEXTURE:
                {
                    std::shared_ptr<GRAPHICS::Texture> texture = GetTexture(asset.Id, asset.BinaryData);
                    if (!texture)
                    {
                        return false;
                    }
                    break;
                }
            }
        }

        return true;
    }

    /// Attempts to retrieve the texture identified by the specified ID.
    /// @param[in]  texture_id - The ID of the texture to load.
    /// @return The requested texture, if successfully loaded; null otherwise.
    std::shared_ptr<GRAPHICS::Texture> Assets::GetTexture(const AssetId texture_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // RETURN THE TEXTURE IF IT HAS ALREADY BEEN LOADED.
        auto id_with_texture = Textures.find(texture_id);
        bool texture_already_loaded = (Textures.cend() != id_with_texture);
        if (texture_already_loaded)
        {
            return id_with_texture->second;
        }

        // GET THE TEXTURE FILEPATH.
        const auto texture_id_with_filepath = ASSET_FILEPATHS_BY_ID.find(texture_id);
        bool texture_filepath_found = (ASSET_FILEPATHS_BY_ID.cend() != texture_id_with_filepath);
        if (!texture_filepath_found)
        {
            // The filepath is needed to load the texture.
            return nullptr;
        }

        // TRY LOADING THE TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> texture = GRAPHICS::Texture::Load(texture_id_with_filepath->second);
        bool texture_loaded = (nullptr != texture);
        if (texture_loaded)
        {
            // CACHE THE TEXTURE IN THIS COLLECTION BEFORE RETURNING.
            Textures[texture_id] = texture;
            return texture;
        }
        else
        {
            // The texture couldn't be retrieved.
            return nullptr;
        }
    }

    /// Attempts to retrieve the texture identified by the specified ID.
    /// @param[in]  texture_id - The ID of the texture to load.
    /// @param[in]  binary_data - The binary texture data.
    /// @return The requested texture, if successfully loaded; null otherwise.
    std::shared_ptr<GRAPHICS::Texture> Assets::GetTexture(const AssetId texture_id, const std::string& binary_data)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // RETURN THE TEXTURE IF IT HAS ALREADY BEEN LOADED.
        auto id_with_texture = Textures.find(texture_id);
        bool texture_already_loaded = (Textures.cend() != id_with_texture);
        if (texture_already_loaded)
        {
            return id_with_texture->second;
        }

        // TRY LOADING THE TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> texture = GRAPHICS::Texture::LoadFromMemory(binary_data);
        bool texture_loaded = (nullptr != texture);
        if (texture_loaded)
        {
            // CACHE THE TEXTURE IN THIS COLLECTION BEFORE RETURNING.
            Textures[texture_id] = texture;
            return texture;
        }
        else
        {
            // The texture couldn't be retrieved.
            return nullptr;
        }
    }

    /// Attempts to retrieve the font identified by the specified texture ID.
    /// @param[in]  font_texture_id - The ID of the texture associated with the font.
    /// @return The requested font, if successfully loaded; null otherwise.
    std::shared_ptr<GRAPHICS::GUI::Font> Assets::GetFont(const AssetId font_texture_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // RETURN THE FONT IF IT HAS ALREADY BEEN LOADED.
        auto id_with_font = Fonts.find(font_texture_id);
        bool font_already_loaded = (Fonts.cend() != id_with_font);
        if (font_already_loaded)
        {
            return id_with_font->second;
        }
        else
        {
            // INDICATE THAT THE FONT COULD NOT BE LOADED.
            return nullptr;
        }
    }

    /// Attempts to get the specified shader.
    /// @param[in]  shader_id - The ID of the shader to get.
    /// @return The specified shader, if successfully loaded; null otherwise.
    std::shared_ptr<sf::Shader> Assets::GetShader(const AssetId shader_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // RETURN THE SHADER IF IT HAS ALREADY BEEN LOADED.
        auto id_with_shader = Shaders.find(shader_id);
        bool shader_already_loaded = (Shaders.cend() != id_with_shader);
        if (shader_already_loaded)
        {
            return id_with_shader->second;
        }
        else
        {
            return nullptr;
        }
    }

    /// Attempts to get the specified shader.  Assumed to always be a fragment shader.
    /// @param[in]  shader_id - The ID of the shader to get.
    /// @param[in]  binary_data - The binary data of the shader.
    /// @return The specified shader, if successfully loaded; null otherwise.
    std::shared_ptr<sf::Shader> Assets::GetShader(const AssetId shader_id, const std::string& binary_data)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // RETURN THE SHADER IF IT HAS ALREADY BEEN LOADED.
        auto id_with_shader = Shaders.find(shader_id);
        bool shader_already_loaded = (Shaders.cend() != id_with_shader);
        if (shader_already_loaded)
        {
            return id_with_shader->second;
        }

        // TRY LOADING THE SHADER.
        std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
        bool shader_loaded = shader->loadFromMemory(binary_data, sf::Shader::Fragment);
        
        // RETURN THE SHADER ONLY IF SUCCESSFULLY LOADED.
        if (shader_loaded)
        {
            Shaders[shader_id] = shader;
            return shader;
        }
        else
        {
            return nullptr;
        }
    }

    /// Attempts to retrieve the sound effect identified by the specified ID.
    /// The returned sound effect will be a new instance of a sound effect,
    /// but it may share the same buffer of audio samples as another instance.
    /// @param[in]  sound_id - The ID of the sound to load.
    /// @return The requested sound effect, if successfully loaded; null otherwise.
    std::shared_ptr<sf::SoundBuffer> Assets::GetSound(const AssetId sound_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // CHECK IF THE AUDIO SAMPLES HAVE ALREADY BEEN LOADED.
        auto id_with_audio_samples = AudioSamples.find(sound_id);
        bool audio_samples_already_loaded = (AudioSamples.cend() != id_with_audio_samples);
        if (audio_samples_already_loaded)
        {
            // RETURN THE SOUND EFFECT USING THE PREVIOUSLY LOADED AUDIO SAMPLES.
            return id_with_audio_samples->second;
        }

        // GET THE SOUND FILEPATH.
        const auto sound_id_with_filepath = ASSET_FILEPATHS_BY_ID.find(sound_id);
        bool sound_filepath_found = (ASSET_FILEPATHS_BY_ID.cend() != sound_id_with_filepath);
        if (!sound_filepath_found)
        {
            // The filepath is needed to load the sound.
            return nullptr;
        }

        // LOAD THE AUDIO SAMPLES INTO A BUFFER.
        // The sound ID maps directly to a filepath.
        std::shared_ptr<sf::SoundBuffer> sound_buffer = std::make_shared<sf::SoundBuffer>();
        bool audio_samples_loaded = sound_buffer->loadFromFile(sound_id_with_filepath->second);
        if (!audio_samples_loaded)
        {
            return nullptr;
        }

        // SAVE THE SOUND TO AVOID TAKING TIME TO LOAD IT IN THE FUTURE.
        AudioSamples[sound_id] = sound_buffer;
        return sound_buffer;
    }

    /// Attempts to retrieve the sound effect identified by the specified ID.
    /// The returned sound effect will be a new instance of a sound effect,
    /// but it may share the same buffer of audio samples as another instance.
    /// @param[in]  sound_id - The ID of the sound to load.
    /// @param[in]  binary_data - The binary data for the sound.
    /// @return The requested sound effect, if successfully loaded; null otherwise.
    std::shared_ptr<sf::SoundBuffer> Assets::GetSound(const AssetId sound_id, const std::string& binary_data)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // CHECK IF THE AUDIO SAMPLES HAVE ALREADY BEEN LOADED.
        auto id_with_audio_samples = AudioSamples.find(sound_id);
        bool audio_samples_already_loaded = (AudioSamples.cend() != id_with_audio_samples);
        if (audio_samples_already_loaded)
        {
            // RETURN THE SOUND EFFECT USING THE PREVIOUSLY LOADED AUDIO SAMPLES.
            return id_with_audio_samples->second;
        }

        // LOAD THE AUDIO SAMPLES INTO A BUFFER.
        std::shared_ptr<sf::SoundBuffer> sound_buffer = std::make_shared<sf::SoundBuffer>();
        bool audio_samples_loaded = sound_buffer->loadFromMemory(binary_data.data(), binary_data.size());
        if (!audio_samples_loaded)
        {
            return nullptr;
        }

        // SAVE THE SOUND TO AVOID TAKING TIME TO LOAD IT IN THE FUTURE.
        AudioSamples[sound_id] = sound_buffer;
        return sound_buffer;
    }

    /// Attempts to retrieve the music identified by the specified ID.
    /// The music will be set to loop.  If the music has previously
    /// been loaded, the previously loaded instance will be returned.
    /// @param[in]  music_id - The ID of the music to load.
    /// @return The requested music, if successfully loaded; null otherwise.
    std::shared_ptr<sf::Music> Assets::GetMusic(const AssetId music_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // CHECK IF THE MUSIC HAS ALREADY BEEN LOADED.
        auto id_with_music = Music.find(music_id);
        bool music_already_loaded = (Music.cend() != id_with_music);
        if (music_already_loaded)
        {
            // RETURN THE PREVIOUSLY LOADED MUSIC.
            return id_with_music->second;
        }

        // GET THE MUSIC FILEPATH.
        const auto music_id_with_filepath = ASSET_FILEPATHS_BY_ID.find(music_id);
        bool music_filepath_found = (ASSET_FILEPATHS_BY_ID.cend() != music_id_with_filepath);
        if (!music_filepath_found)
        {
            // The filepath is needed to load the music.
            return nullptr;
        }

        // LOAD THE MUSIC.
        // The muisc ID maps directly to a filepath.
        std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
        bool music_loaded = music->openFromFile(music_id_with_filepath->second);
        if (!music_loaded)
        {
            return nullptr;
        }

        // SAVE THE MUSIC TO AVOID TAKING TIME TO LOAD IT IN THE FUTURE.
        Music[music_id] = music;
        return music;
    }

    /// Attempts to retrieve the music identified by the specified ID.
    /// The music will be set to loop.  If the music has previously
    /// been loaded, the previously loaded instance will be returned.
    /// @param[in]  music_id - The ID of the music to load.
    /// @param[in]  binary_data - The binary data of the music.
    /// @return The requested music, if successfully loaded; null otherwise.
    std::shared_ptr<sf::Music> Assets::GetMusic(const AssetId music_id, const std::string& binary_data)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(AssetMutex);

        // CHECK IF THE MUSIC HAS ALREADY BEEN LOADED.
        auto id_with_music = Music.find(music_id);
        bool music_already_loaded = (Music.cend() != id_with_music);
        if (music_already_loaded)
        {
            // RETURN THE PREVIOUSLY LOADED MUSIC.
            return id_with_music->second;
        }

        // CREATE A PERMANENT COPY OF THE BINARY DATA.
        std::size_t music_size_in_bytes = binary_data.size();
        MusicData[music_id] = std::make_unique<uint8_t[]>(music_size_in_bytes);
        const std::unique_ptr<uint8_t[]>& music_data = MusicData[music_id];
        std::memcpy(music_data.get(), binary_data.data(), music_size_in_bytes);

        // LOAD THE MUSIC.
        std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
        bool music_loaded = music->openFromMemory(music_data.get(), music_size_in_bytes);
        if (!music_loaded)
        {
            return nullptr;
        }

        // SAVE THE MUSIC TO AVOID TAKING TIME TO LOAD IT IN THE FUTURE.
        Music[music_id] = music;
        return music;
    }
}