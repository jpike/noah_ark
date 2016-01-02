#pragma once

#include "Audio/AudioClip.h"

namespace RESOURCES
{
    /// An audio clip for a square wave with 1000 samples
    /// and a sample rate of 1000 samples/second.
    /// When played in loop, makes a kind of "knocking" sound.
    /// Multiple "knocks" occur within a single play of this clip.
    extern const AUDIO::AudioClip SQUARE_WAVE_1000_SAMPLES_1000_SAMPLE_RATE;
    /// An audio clip for a sawtooth wave with 1000 samples
    /// and a sample rate of 2000 samples/second.
    /// When played in loop, makes a kind of "knocking" sound.
    /// A single "knock" occurs within a single play of this clip.
    extern const AUDIO::AudioClip SAWTOOTH_WAVE_1000_SAMPLES_2000_SAMPLE_RATE;
    /// An audio clip for a triangle wave with 1000 samples
    /// and a sample rate of 8000 samples/second.
    extern const AUDIO::AudioClip TRIANGLE_WAVE_1000_SAMPLES_8000_SAMPLE_RATE;

    /// The audio clip to play when an axe hits an object.
    extern const AUDIO::AudioClip& AXE_HIT_AUDIO_CLIP;
    /// The audio clip to play when a tree is shaking.
    extern const AUDIO::AudioClip& TREE_SHAKE_AUDIO_CLIP;
}
