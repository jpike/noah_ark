#include <stdexcept>
#include "Audio/SoundEffect.h"

namespace AUDIO
{
    SoundEffect::SoundEffect(const std::shared_ptr<sf::SoundBuffer>& audio_samples) :
    AudioSamples(audio_samples),
    Sound()
    {
        // MAKE SURE AUDIO SAMPLES WERE PROVIDED.
        bool audio_samples_provided = (nullptr != AudioSamples);
        if (!audio_samples_provided)
        {
            throw std::invalid_argument("Audio samples were not provided when constructing a sound effect.");
        }

        // CREATE THE SOUND FOR THE AUDIO SAMPLES.
        Sound.setBuffer(*AudioSamples);
    }

    void SoundEffect::Play()
    {
        Sound.play();
    }

    void SoundEffect::Stop()
    {
        Sound.stop();
    }

    bool SoundEffect::IsPlaying() const
    {
        sf::SoundSource::Status sound_status = Sound.getStatus();
        bool is_playing = (sf::SoundSource::Playing == sound_status);
        return is_playing;
    }
}
