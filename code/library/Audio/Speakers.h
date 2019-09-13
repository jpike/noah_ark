#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include "Audio/SoundEffect.h"
#include "Resources/AssetId.h"

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
        void AddSound(const RESOURCES::AssetId sound_id, const std::shared_ptr<sf::SoundBuffer>& audio_samples);
        bool SoundIsPlaying(const RESOURCES::AssetId sound_id);
        void PlaySound(const RESOURCES::AssetId sound_id);

        // MUSIC.
        void AddMusic(const RESOURCES::AssetId music_id, const std::shared_ptr<sf::Music>& music);
        void PlayMusic(const RESOURCES::AssetId music_id);
        void PlayMusicIfNotAlready(const RESOURCES::AssetId music_id);
        void StopMusic(const RESOURCES::AssetId music_id);

    private:
        // MEMBER VARIABLES.
        /// A mutex to provide thread-safety for this class.
        std::recursive_mutex SpeakerMutex = {};
        /// The sound effects currently loaded into the speakers.
        std::unordered_map<RESOURCES::AssetId, AUDIO::SoundEffect> Sounds = {};
        /// THe music currently loaded into the speakers.
        std::unordered_map< RESOURCES::AssetId, std::shared_ptr<sf::Music> > Music = {};
    };
}
