#include "Audio/SoundEffect.h"
#include "ErrorHandling/NullChecking.h"

namespace AUDIO
{
    /// Attempts to load sound from in-memory data.
    /// @param[in]  binary_data - The binary sound data to try loading from.
    ///     Must be one of the formats supported by SFML
    ///     (https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1InputSoundFile.php#a4e034a8e9e69ca3c33a3f11180250400).
    /// @return The sound, if successfully loaded; null otherwise.
    std::unique_ptr<SoundEffect> SoundEffect::LoadFromMemory(const std::string& binary_data)
    {
        // LOAD THE AUDIO SAMPLES INTO A BUFFER.
        std::shared_ptr<sf::SoundBuffer> sound_buffer = std::make_shared<sf::SoundBuffer>();
        bool audio_samples_loaded = sound_buffer->loadFromMemory(binary_data.data(), binary_data.size());
        if (!audio_samples_loaded)
        {
            return nullptr;
        }

        // RETURN THE SOUND EFFECT.
        auto sound = std::make_unique<SoundEffect>(sound_buffer);
        return sound;
    }

    /// Constructs a sound effect using the provided audio samples.
    /// @param[in]  audio_samples - The audio samples for the sound effect.
    /// @throws std::invalid_argument - Thrown if the audio samples are null.
    SoundEffect::SoundEffect(const std::shared_ptr<sf::SoundBuffer>& audio_samples) :
    AudioSamples(audio_samples),
    Sound()
    {
        // MAKE SURE AUDIO SAMPLES WERE PROVIDED.
        ERROR_HANDLING::ThrowInvalidArgumentExceptionIfNull(AudioSamples, "Audio samples were not provided when constructing a sound effect.");

        // CREATE THE SOUND FOR THE AUDIO SAMPLES.
        Sound.setBuffer(*AudioSamples);
    }

    /// Starts playing the sound effect from the beginning if
    /// it was stopped or currently playing.  If paused,
    /// the sound resumes playing from where it was paused.
    void SoundEffect::Play()
    {
        Sound.play();
    }

    /// Stops playing the sound effect.
    void SoundEffect::Stop()
    {
        Sound.stop();
    }

    /// Checks if the sound effect is currently playing.
    /// @return True if the sound effect is currently playing; false otherwise.
    bool SoundEffect::IsPlaying() const
    {
        sf::SoundSource::Status sound_status = Sound.getStatus();
        bool is_playing = (sf::SoundSource::Playing == sound_status);
        return is_playing;
    }
}
