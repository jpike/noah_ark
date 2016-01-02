#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

namespace AUDIO
{
    /// A raw clip of audio data.  This provides the simplest
    /// in-memory representation of audio samples used for
    /// sound in a game.
    struct AudioClip
    {
        // MEMBER VARIABLES.
        /// The sample rate for playing the clip.
        unsigned int SampleRateInSamplesPerSecond;
        /// The raw audio sample values.
        std::vector<int16_t> Samples;

        // CONSTRUCTION.
        /// Initializes an audio clip.
        /// @param[in]  sample_rate_in_samples_per_second - The sample rate to use for playing.
        /// @param[in]  samples - The raw audio sample values.
        explicit AudioClip(
            unsigned int sample_rate_in_samples_per_second, 
            std::initializer_list<int16_t> samples) :
        SampleRateInSamplesPerSecond(sample_rate_in_samples_per_second),
        Samples(samples)
        {}
    };

    /// A typedef to assist in uniquely identifying an audio clip.
    /// Since audio clip instances are intended to be constructed
    /// once in constant memory for the game, a pointer to an audio
    /// clip should currently be sufficient to identify audio clips
    /// without needing to store extra data (i.e. a string name).
    typedef const AudioClip* AudioClipId;
}
