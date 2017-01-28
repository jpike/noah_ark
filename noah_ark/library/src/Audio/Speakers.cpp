#include <algorithm>
#include "Audio/Speakers.h"

namespace AUDIO
{
    /// Starts playing the sound out of the speakers.
    /// @param[in]  sound - The sound to play.
    void Speakers::Play(const std::shared_ptr<AUDIO::SoundEffect>& sound)
    {
        // MAKE SURE THE SOUND EXISTS.
        bool sound_exists = (nullptr != sound);
        if (!sound_exists)
        {
            // The sound can't be played if it doesn't exist.
            return;
        }

        // START PLAYING THE SOUND.
        sound->Play();

        // STORE THE SOUND SO THAT IT CAN FINISH BEING PLAYED.
        Sounds.push_back(sound);
    }

    /// Removes any sounds from the speakers that have finished playing.
    void Speakers::RemoveCompletedSounds()
    {
        // MOVE ALL COMPLETED SOUNDS TO THE END OF THE CONTAINER.
        auto sound_completed = [](const std::shared_ptr<AUDIO::SoundEffect>& sound) { return !sound->IsPlaying(); };
        auto first_completed_sound = std::remove_if(
            Sounds.begin(),
            Sounds.end(),
            sound_completed);

        // REMOVE THE COMPLETED SOUNDS.
        Sounds.erase(first_completed_sound, Sounds.end());
    }
}
