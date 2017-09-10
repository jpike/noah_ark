#include "Audio/Speakers.h"

namespace AUDIO
{
    /// Adds a sound to the speakers for playing.
    /// If a sound with the specified ID already exists in the speakers, it will be overwritten.
    /// @param[in]  sound_id - The unique ID for the sound.
    /// @param[in]  audio_samples - The audio samples for the sound.
    void Speakers::AddSound(const std::string& sound_id, const std::shared_ptr<sf::SoundBuffer>& audio_samples)
    {
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
    bool Speakers::IsPlaying(const std::string& sound_id)
    {
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
    /// @param[in]  sound_id - The ID of the sound to play.
    void Speakers::Play(const std::string& sound_id)
    {
        // PLAY THE SOUND IF IT EXISTS.
        auto sound_effect = Sounds.find(sound_id);
        bool sound_exists = (Sounds.end() != sound_effect);
        if (sound_exists)
        {
            sound_effect->second.Play();
        }
    }
}
