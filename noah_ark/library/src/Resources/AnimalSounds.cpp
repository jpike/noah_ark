#include "Resources/AnimalSounds.h"

namespace RESOURCES
{
    /// Gets the sound effect for the animal species, if one exists.
    /// @param[in]  animal_species - The species for which to get the sound effect.
    /// @param[in,out]  assets - The assets from which to get the sound effect.
    /// @return The sound for the animal, if one exists.
    std::shared_ptr<AUDIO::SoundEffect> AnimalSounds::GetSound(
        const OBJECTS::AnimalSpecies animal_species,
        Assets& assets)
    {
        // GET THE SOUND EFFECT FOR THE SPECIES.
        switch (animal_species)
        {
            case OBJECTS::AnimalSpecies::CAMEL:
                return assets.GetSound("res/sounds/camel5.wav");
                return nullptr;
            case OBJECTS::AnimalSpecies::CATTLE:
                return assets.GetSound("res/sounds/SingleCow-SoundBible.com-2051754137.wav");
            case OBJECTS::AnimalSpecies::DOVE:
                return assets.GetSound("res/sounds/dove-Mike_Koenig-1208819046.wav");
            case OBJECTS::AnimalSpecies::FROG:
                return assets.GetSound("res/sounds/FrogCroaking-SoundBible.com-1053984354.wav");
            case OBJECTS::AnimalSpecies::LOCUST:
                return assets.GetSound("res/sounds/Grasshopper-SoundBible.com-1592176719.wav");
            case OBJECTS::AnimalSpecies::RAVEN:
                return assets.GetSound("res/sounds/Raven-SoundBible.com-1790882934.wav");
            case OBJECTS::AnimalSpecies::SHEEP:
                return assets.GetSound("res/sounds/SheepBleating-SoundBible.com-1373580685.wav");
            default:
                // No sound exists.
                return nullptr;
        }
    }
}
