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
        void PlaySoundEffect(const RESOURCES::AssetId sound_id);

        // MUSIC.
        std::shared_ptr<sf::Music> LoadMusic(const RESOURCES::AssetId music_id, const std::string& music_binary_data);
        void AddMusic(const RESOURCES::AssetId music_id, const std::shared_ptr<sf::Music>& music);
        void PlayMusic(const RESOURCES::AssetId music_id);
        void PlayMusicIfNotAlready(const RESOURCES::AssetId music_id);
        void StopMusic(const RESOURCES::AssetId music_id);

        // GENERAL.
        void StopAllAudio();

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// True if the speakers are enabled; false otherwise.
        /// Speakers may need to be disabled to allow the game to be played even if no audio devices are available.
        bool Enabled = true;

    private:
        // MEMBER VARIABLES.
        /// A mutex to provide thread-safety for this class.
        std::recursive_mutex SpeakerMutex = {};
        /// The sound effects currently loaded into the speakers.
        std::unordered_map<RESOURCES::AssetId, AUDIO::SoundEffect> Sounds = {};
        /// THe music currently loaded into the speakers.
        std::unordered_map<RESOURCES::AssetId, std::shared_ptr<sf::Music>> Music = {};
        /// Raw binary music data mapped by asset ID.  Must remain in-memory due to how SFML works
        /// (https://www.sfml-dev.org/documentation/2.5.0/classsf_1_1Music.php#ae93b21bcf28ff0b5fec458039111386e).
        /// This doesn't need to be done for other assets since they don't need to remain in memory.
        std::unordered_map<RESOURCES::AssetId, std::unique_ptr<uint8_t[]>> MusicData = {};
    };
}
