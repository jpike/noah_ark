#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include "Audio/SoundEffect.h"

namespace AUDIO
{
    /// The speakers of the computer out of which audio is played.
    /// This class primarily assists in ensuring that sounds persist
    /// in memory for the entirety of the time they are being played,
    /// as opposed to potentially dropping out of memory if they go
    /// out of scope.
    class Speakers
    {
    public:
        void AddSound(const std::string& sound_id, const std::shared_ptr<sf::SoundBuffer>& audio_samples);
        bool IsPlaying(const std::string& sound_id);
        void Play(const std::string& sound_id);

    private:
        /// The sound effects currently loaded into the speakers.
        /// Mapped by sound ID.
        std::unordered_map<std::string, AUDIO::SoundEffect> Sounds = {};
    };
}
