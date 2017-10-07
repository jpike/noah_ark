#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include "Audio/SoundEffect.h"

namespace AUDIO
{
    /// The speakers of the computer out of which audio is played.
    /// Before a sound can be played, the sound must be added with a unique ID
    /// to the speakers.  This is because a limited number of sounds can be
    /// loaded by the underlying sound library at a given time, so this design
    /// is intended to help capture this idea by avoiding creation of an arbitrary
    /// number of sound effects as gameplay goes on.
    class Speakers
    {
    public:
        // SOUND EFFECTS.
        void AddSound(const std::string& sound_id, const std::shared_ptr<sf::SoundBuffer>& audio_samples);
        bool SoundIsPlaying(const std::string& sound_id);
        void PlaySound(const std::string& sound_id);

        // MUSIC.
        void AddMusic(const std::string& music_id, const std::shared_ptr<sf::Music>& music);
        void PlayMusic(const std::string& music_id);
        void StopMusic(const std::string& music_id);

    private:
        /// The sound effects currently loaded into the speakers.
        /// Mapped by sound ID.
        std::unordered_map<std::string, AUDIO::SoundEffect> Sounds = {};
        /// THe music currently loaded into the speakers.
        /// Mapped by music ID.
        std::unordered_map< std::string, std::shared_ptr<sf::Music> > Music = {};
    };
}
