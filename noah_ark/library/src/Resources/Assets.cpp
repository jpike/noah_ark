#include <cassert>
#include <cstdint>
#include <future>
#include <vector>
#include "Resources/Assets.h"

namespace RESOURCES
{
    // ASSET IDs.
    // For assets that require accessing the filesystem, IDs are currently set to filepaths
    // (relative to the working directory) to simplify loading.
    const std::string AXE_TEXTURE_ID = "res/images/noah_sprite1.png";
    const std::string DUST_CLOUD_TEXTURE_ID = "res/images/tree_sprite1.png";
    const std::string FONT_TEXTURE_ID = "res/images/main_font1.png";
    const std::string GROUND_TILESET_TEXTURE_ID = "res/images/ground_tile_set.png";
    const std::string NOAH_TEXTURE_ID = "res/images/noah_sprite1.png";
    const std::string TREE_TEXTURE_ID = "res/images/tree_sprite1.png";
    const std::string WOOD_LOG_TEXTURE_ID = "res/images/tree_sprite1.png";
    const std::string AXE_HIT_SOUND_ID = "res/sounds/axe_tree_hit1.wav";
    const std::string COLLECT_BIBLE_VERSE_SOUND_ID = "res/sounds/collect_bible_verse1.wav";
    const std::string TREE_SHAKE_SOUND_ID = "res/sounds/tree_shake4.wav";
    const std::string OVERWORLD_BACKGROUND_MUSIC_ID = "res/sounds/overworld_music.wav";

    /// Attempts to retrieve the texture identified by the specified ID.
    /// @param[in]  texture_id - The ID of the texture to load.
    /// @return The requested texture, if successfully loaded; null otherwise.
    std::shared_ptr<GRAPHICS::Texture> Assets::GetTexture(const std::string& texture_id)
    {
        // RETURN THE TEXTURE IF IT HAS ALREADY BEEN LOADED.
        auto id_with_texture = Textures.find(texture_id);
        bool texture_already_loaded = (Textures.cend() != id_with_texture);
        if (texture_already_loaded)
        {
            return id_with_texture->second;
        }

        // TRY LOADING THE TEXTURE.
        // Texture IDs map directly to filepaths.
        const auto& texture_filepath = texture_id;
        std::shared_ptr<GRAPHICS::Texture> texture = GRAPHICS::Texture::Load(texture_filepath);
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
    std::shared_ptr<GRAPHICS::GUI::Font> Assets::GetFont(const std::string& font_texture_id)
    {
        // RETURN THE FONT IF IT HAS ALREADY BEEN LOADED.
        auto id_with_font = Fonts.find(font_texture_id);
        bool font_already_loaded = (Fonts.cend() != id_with_font);
        if (font_already_loaded)
        {
            return id_with_font->second;
        }

        // GET THE FONT'S TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> font_texture = GetTexture(font_texture_id);
        bool font_texture_loaded = (nullptr != font_texture);
        if (font_texture_loaded)
        {
            // CACHE THE FONT IN THIS COLLECTION BEFORE RETURNING.
            std::shared_ptr<GRAPHICS::GUI::Font> font = std::make_shared<GRAPHICS::GUI::Font>(font_texture);
            Fonts[font_texture_id] = font;
            return font;
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
    std::shared_ptr<sf::Shader> Assets::GetShader(const ShaderId shader_id) const
    {
        // RETURN THE SHADER IF IT HAS ALREADY BEEN LOADED.
        auto id_with_shader = Shaders.find(shader_id);
        bool shader_already_loaded = (Shaders.cend() != id_with_shader);
        if (shader_already_loaded)
        {
            return id_with_shader->second;
        }

        // TRY LOADING THE SHADER.
        bool shader_loaded = false;
        std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
        switch (shader_id)
        {
            case ShaderId::COLORED_TEXTURE:
            {
                // LOAD THE SHADER FROM ITS CODE.
                const std::string COLORED_TEXTURE_FRAGMENT_SHADER_CODE = R"(
                    uniform vec4 color; // Current color to use to tint texture.
                    uniform sampler2D texture;  // Current texture being rendered.

                    void main()
                    {
                        // GET THE CURRENT PIXEL FROM THE TEXTURE.
                        vec4 current_pixel_color = texture2D(texture, gl_TexCoord[0].xy);

                        // APPLY THE COLOR TO THE PIXEL.
                        gl_FragColor = color * current_pixel_color;
                    }
                )";
                shader_loaded = shader->loadFromMemory(COLORED_TEXTURE_FRAGMENT_SHADER_CODE, sf::Shader::Fragment);
                break;
            }
        }

        // RETURN THE SHADER ONLY IF SUCCESSFULLY LOADED.
        if (shader_loaded)
        {
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
    std::shared_ptr<AUDIO::SoundEffect> Assets::GetSound(const std::string& sound_id)
    {
        // CHECK IF THE AUDIO SAMPLES HAVE ALREADY BEEN LOADED.
        auto id_with_audio_samples = AudioSamples.find(sound_id);
        bool audio_samples_already_loaded = (AudioSamples.cend() != id_with_audio_samples);
        if (audio_samples_already_loaded)
        {
            // RETURN THE SOUND EFFECT USING THE PREVIOUSLY LOADED AUDIO SAMPLES.
            std::shared_ptr<AUDIO::SoundEffect> sound_effect = std::make_shared<AUDIO::SoundEffect>(id_with_audio_samples->second);
            return sound_effect;
        }

        // LOAD THE AUDIO SAMPLES INTO A BUFFER.
        // The sound ID maps directly to a filepath.
        std::shared_ptr<sf::SoundBuffer> sound_buffer = std::make_shared<sf::SoundBuffer>();
        bool audio_samples_loaded = sound_buffer->loadFromFile(sound_id);
        if (!audio_samples_loaded)
        {
            return nullptr;
        }

        // SAVE THE SOUND TO AVOID TAKING TIME TO LOAD IT IN THE FUTURE.
        AudioSamples[sound_id] = sound_buffer;

        // RETURN THE SOUND EFFECT.
        std::shared_ptr<AUDIO::SoundEffect> sound_effect = std::make_shared<AUDIO::SoundEffect>(sound_buffer);
        return sound_effect;
    }

    /// Attempts to retrieve the music identified by the specified ID.
    /// The music will be set to loop.  If the music has previously
    /// been loaded, the previously loaded instance will be returned.
    /// @param[in]  music_id - The ID of the music to load.
    /// @return The requested music, if successfully loaded; null otherwise.
    std::shared_ptr<sf::Music> Assets::GetMusic(const std::string& music_id)
    {
        // CHECK IF THE MUSIC HAS ALREADY BEEN LOADED.
        auto id_with_music = Music.find(music_id);
        bool music_already_loaded = (Music.cend() != id_with_music);
        if (music_already_loaded)
        {
            // RETURN THE PREVIOUSLY LOADED MUSIC.
            return id_with_music->second;
        }

        // LOAD THE MUSIC.
        // The muisc ID maps directly to a filepath.
        std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
        bool music_loaded = music->openFromFile(music_id);
        if (!music_loaded)
        {
            return nullptr;
        }

        // SAVE THE MUSIC TO AVOID TAKING TIME TO LOAD IT IN THE FUTURE.
        Music[music_id] = music;

        // RETURN THE MUSIC AS LOOPING.
        // So far, there isn't a need for music that doesn't loop.
        music->setLoop(true);
        return music;
    }
}