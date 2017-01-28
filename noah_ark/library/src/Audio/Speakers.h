#pragma once

#include <memory>
#include <vector>
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
        void Play(const std::shared_ptr<AUDIO::SoundEffect>& sound);
        void RemoveCompletedSounds();

    private:
        /// The sound effects currently in the speakers for playing.
        std::vector< std::shared_ptr<AUDIO::SoundEffect> > Sounds = {};
    };
}
