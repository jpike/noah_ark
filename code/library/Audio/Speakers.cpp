#include <al.h>
#include "Audio/Speakers.h"

namespace AUDIO
{
    /// Adds a sound to the speakers for playing.
    /// If a sound with the specified ID already exists in the speakers, it will be overwritten.
    /// @param[in]  sound_id - The unique ID for the sound.
    /// @param[in]  audio_samples - The audio samples for the sound.
    void Speakers::AddSound(const RESOURCES::AssetId sound_id, const std::shared_ptr<sf::SoundBuffer>& audio_samples)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return;
        }

        // MAKE SURE THE AUDIO SAMPLES EXIST.
        if (!audio_samples)
        {
            return;
        }

        // STORE THE SOUND.
        AUDIO::SoundEffect sound_effect = AUDIO::SoundEffect(audio_samples);
        Sounds[sound_id] = sound_effect;
    }

    /// Checks if a sound is currently playing in the speakers.
    /// @param[in]  sound_id - The ID of the sound to check.
    /// @return True if the sound is currently playing; false otherwise.
    bool Speakers::SoundIsPlaying(const RESOURCES::AssetId sound_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return false;
        }

        // CHECK IF THE SOUND EXISTS.
        auto sound_effect = Sounds.find(sound_id);
        bool sound_exists = (Sounds.end() != sound_effect);
        if (sound_exists)
        {
            bool is_playing = sound_effect->second.IsPlaying();
            return is_playing;
        }
        else
        {
            return false;
        }
    }

    /// Starts playing the sound with the given ID, assuming it is already stored in the speakers.
    /// This method name has "Effect" appended to it to avoid conflicts with Windows API #defines.
    /// @param[in]  sound_id - The ID of the sound to play.
    void Speakers::PlaySoundEffect(const RESOURCES::AssetId sound_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return;
        }

        // PLAY THE SOUND IF IT EXISTS.
        auto sound_effect = Sounds.find(sound_id);
        bool sound_exists = (Sounds.end() != sound_effect);
        if (sound_exists)
        {
            sound_effect->second.Play();
        }
    }

    /// Attempts to load music from binary data into the speakers.
    /// If music with the specified ID already exists in the speakers, it will be overwritten.
    /// @param[in]  music_id - The unique ID for the music.
    /// @param[in]  music_binary_data - The raw binary data for the music.
    /// @return The music, if successfully loaded; null otherwise.
    std::shared_ptr<sf::Music> Speakers::LoadMusic(const RESOURCES::AssetId music_id, const std::string& music_binary_data)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return nullptr;
        }

        // CREATE A PERMANENT COPY OF THE BINARY DATA.
        std::size_t music_size_in_bytes = music_binary_data.size();
        MusicData[music_id] = std::make_unique<uint8_t[]>(music_size_in_bytes);
        const std::unique_ptr<uint8_t[]>& music_data = MusicData[music_id];
        std::memcpy(music_data.get(), music_binary_data.data(), music_size_in_bytes);

        // LOAD THE MUSIC.
        std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
        bool music_loaded = music->openFromMemory(music_data.get(), music_size_in_bytes);
        if (!music_loaded)
        {
            return nullptr;
        }

        // CHECK TO SEE IF AN OPEN AL ERROR OCCURRED.
        // This could occur if there isn't an audio device for some reason.
        ALenum open_al_error = alGetError();
        bool open_al_error_ocurred = (AL_NO_ERROR != open_al_error);
        if (open_al_error_ocurred)
        {
            return nullptr;
        }

        // ADD THE MUSIC TO THE SPEAKERS.
        AddMusic(music_id, music);
        return music;
    }

    /// Adds music to the speakers for playing.
    /// If music with the specified ID already exists in the speakers, it will be overwritten.
    /// @param[in]  music_id - The unique ID for the music.
    /// @param[in]  music - The music.
    void Speakers::AddMusic(const RESOURCES::AssetId music_id, const std::shared_ptr<sf::Music>& music)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return;
        }

        // MAKE SURE THE MUSIC EXISTS.
        if (!music)
        {
            return;
        }

        // STORE THE MUSIC.
        Music[music_id] = music;
    }

    /// Starts playing the music with the given ID, assuming it is already stored in the speakers.
    /// @param[in]  music_id - The ID of the music to play.
    void Speakers::PlayMusic(const RESOURCES::AssetId music_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return;
        }

        // PLAY THE MUSIC IF IT EXISTS.
        auto music = Music.find(music_id);
        bool music_exists = (Music.end() != music) && (nullptr != music->second);
        if (music_exists)
        {
            music->second->play();
        }
    }

    /// Starts playing the music with the given ID, assuming it is already stored in the speakers
    /// and not already playing.  If it is already playing, then it will just continue playing.
    /// @param[in]  music_id - The ID of the music to play.
    void Speakers::PlayMusicIfNotAlready(const RESOURCES::AssetId music_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return;
        }

        // CHECK IF THE MUSIC EXISTS IN THE SPEAKERS.
        auto music = Music.find(music_id);
        bool music_exists = (Music.end() != music) && (nullptr != music->second);
        if (music_exists)
        {
            // PLAY THE MUSIC ONLY IF IT'S NOT ALREADY PLAYING.
            sf::SoundSource::Status music_status = music->second->getStatus();
            bool music_playing = (sf::SoundSource::Playing == music_status);
            if (!music_playing)
            {
                music->second->play();
            }
        }
    }

    /// Stops playing the music with the given ID, assuming it is already stored in the speakers.
    /// @param[in]  music_id - The ID of the music to stop.
    void Speakers::StopMusic(const RESOURCES::AssetId music_id)
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return;
        }

        // STOP THE MUSIC IF IT EXISTS.
        auto music = Music.find(music_id);
        bool music_exists = (Music.end() != music) && (nullptr != music->second);
        if (music_exists)
        {
            music->second->stop();
        }
    }

    /// Stops all audio that may be playing in the speakers.
    void Speakers::StopAllAudio()
    {
        // PROTECT AGAINST THIS CLASS BEING USED BY MULTIPLE THREADS.
        std::lock_guard<std::recursive_mutex> lock(SpeakerMutex);

        // DON'T DO ANYTHING IF THE SPEAKERS ARE DISABLED.
        // It's not worth spending time on anything if the speakers are disabled.
        if (!Enabled)
        {
            return;
        }

        // STOP ANY PLAYING SOUNDS.
        for (auto& id_and_sound : Sounds)
        {
            id_and_sound.second.Stop();
        }
        
        // STOP ANY PLAYING MUSIC.
        for (auto& id_and_music : Music)
        {
            id_and_music.second->stop();
        }
    }
}
