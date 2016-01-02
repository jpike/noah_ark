#pragma once

#include <memory>
#include <SFML/Audio.hpp>

/// Holds code related to audio (sound effects, music) for the game.
namespace AUDIO
{
    /// A short sound effect.
    class SoundEffect
    {
    public:
        // CONSTRUCTION.
        /// Constructs a sound effect using the provided audio samples.
        /// @param[in]  audio_samples - The audio samples for the sound effect.
        /// @throws std::invalid_argument - Thrown if the audio samples are null.
        explicit SoundEffect(const std::shared_ptr<sf::SoundBuffer>& audio_samples);

        // PLAYING.
        /// Starts playing the sound effect from the beginning if
        /// it was stopped or currently playing.  If paused,
        /// the sound resumes playing from where it was paused.
        void Play();
        /// Stops playing the sound effect.
        void Stop();

        // STATUS QUERYING.
        /// Checks if the sound effect is currently playing.
        /// @return True if the sound effect is currently playing; false otherwise.
        bool IsPlaying() const;

        // MEMBER VARIABLES.
        /// The buffer of audio samples for the sound.
        /// Stored here to ensure that the sound buffer remains in memory
        /// as long as the sound is in memory.
        std::shared_ptr<sf::SoundBuffer> AudioSamples = nullptr;
        /// The SFML sound for the effect.
        sf::Sound Sound = sf::Sound();
    };
}
