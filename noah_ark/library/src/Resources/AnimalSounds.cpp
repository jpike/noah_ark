#include "Resources/AnimalSounds.h"

namespace RESOURCES
{
    /// Gets the sound effect ID for the animal species, if one exists.
    /// @param[in]  animal_species - The species for which to get the sound effect.
    /// @return The sound ID for the animal, if one exists (empty otherwise).
    std::string AnimalSounds::GetSound(const OBJECTS::AnimalSpecies animal_species)
    {
        // GET THE SOUND EFFECT FOR THE SPECIES.
        switch (animal_species)
        {
            case OBJECTS::AnimalSpecies::BAT:
                return "res/sounds/BatsVocalizing-SoundBible.com-1162895834.wav";
            case OBJECTS::AnimalSpecies::CAMEL:
                return "res/sounds/camel5.wav";
            case OBJECTS::AnimalSpecies::CATTLE:
                return "res/sounds/SingleCow-SoundBible.com-2051754137.wav";
            case OBJECTS::AnimalSpecies::DEER:
                return "res/sounds/Elk-SoundBible.com-734134354.wav";
            case OBJECTS::AnimalSpecies::DOVE:
                return "res/sounds/dove-Mike_Koenig-1208819046.wav";
            case OBJECTS::AnimalSpecies::EAGLE:
                return "res/sounds/BaldEagle-SoundBible.com-2025808531.wav";
            case OBJECTS::AnimalSpecies::FROG:
                return "res/sounds/FrogCroaking-SoundBible.com-1053984354.wav";
            case OBJECTS::AnimalSpecies::GOAT:
                return "res/sounds/GoatBah-SoundBible.com-959734934.wav";
            case OBJECTS::AnimalSpecies::HAWK:
                return "res/sounds/hawk_screeching-Mike_Koenig-1626170357.wav";
            case OBJECTS::AnimalSpecies::LOCUST:
                return "res/sounds/Grasshopper-SoundBible.com-1592176719.wav";
            case OBJECTS::AnimalSpecies::OSPREY:
                return "res/sounds/OspreyBirdCall-SoundBible.com-525395846.wav";
            case OBJECTS::AnimalSpecies::OWL:
                return "res/sounds/horned_owl-Mike_Koenig-1945374932.wav";
            case OBJECTS::AnimalSpecies::PELICAN:
                return "res/sounds/PelicanChick-SoundBible.com-549990839.wav";
            case OBJECTS::AnimalSpecies::PIG:
                return "res/sounds/PigOink-SoundBible.com-1424738584.wav";
            case OBJECTS::AnimalSpecies::RAVEN:
                return "res/sounds/Raven-SoundBible.com-1790882934.wav";
            case OBJECTS::AnimalSpecies::SHEEP:
                return "res/sounds/SheepBleating-SoundBible.com-1373580685.wav";
            default:
                // No sound exists.
                return "";
        }
    }
}
