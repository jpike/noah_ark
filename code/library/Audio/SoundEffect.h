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
        explicit SoundEffect() = default;
        explicit SoundEffect(const std::shared_ptr<sf::SoundBuffer>& audio_samples);

        // PLAYING.
        void Play();
        void Stop();

        // STATUS QUERYING.
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
