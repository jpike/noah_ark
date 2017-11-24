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
            case OBJECTS::AnimalSpecies::ALLIGATOR:
                return "res/sounds/baligator5.wav";
            case OBJECTS::AnimalSpecies::BABOON:
                return "res/sounds/baboon2.wav";
            case OBJECTS::AnimalSpecies::BAT:
                return "res/sounds/BatsVocalizing-SoundBible.com-1162895834.wav";
            case OBJECTS::AnimalSpecies::BEAR:
                return "res/sounds/Bear-SoundBible.com-866204188.wav";
            case OBJECTS::AnimalSpecies::BEE:
                return "res/sounds/Bee-SoundBible.com-914644763.wav";
            case OBJECTS::AnimalSpecies::BOBCAT:
                return "res/sounds/BOB-CAT.wav";
            case OBJECTS::AnimalSpecies::BUTTERFLY:
                return "res/sounds/Butterfly-SoundBible.com-1530156556.wav";
            case OBJECTS::AnimalSpecies::CAMEL:
                return "res/sounds/camel5.wav";
            case OBJECTS::AnimalSpecies::CAT:
                return "res/sounds/CatMeow-SoundBible.com-1453940411.wav";
            case OBJECTS::AnimalSpecies::CATTLE:
                return "res/sounds/SingleCow-SoundBible.com-2051754137.wav";
            case OBJECTS::AnimalSpecies::CHEETAH:
                return "res/sounds/Cheetah5.wav";
            case OBJECTS::AnimalSpecies::CHICKEN:
                return "res/sounds/Chicken-SoundBible.com-744739895.wav";
            case OBJECTS::AnimalSpecies::CHIPMUNK:
                return "res/sounds/AngryChipmunk-SoundBible.com-980210050.wav";
            case OBJECTS::AnimalSpecies::DEER:
                return "res/sounds/Elk-SoundBible.com-734134354.wav";
            case OBJECTS::AnimalSpecies::DOG:
                return "res/sounds/doberman-pincher_daniel-simion.wav";
            case OBJECTS::AnimalSpecies::DOVE:
                return "res/sounds/dove-Mike_Koenig-1208819046.wav";
            case OBJECTS::AnimalSpecies::EAGLE:
                return "res/sounds/BaldEagle-SoundBible.com-2025808531.wav";
            case OBJECTS::AnimalSpecies::FOX:
                return "res/sounds/greyfox-vs-coon.wav";
            case OBJECTS::AnimalSpecies::FROG:
                return "res/sounds/FrogCroaking-SoundBible.com-1053984354.wav";
            case OBJECTS::AnimalSpecies::GOAT:
                return "res/sounds/GoatBah-SoundBible.com-959734934.wav";
            case OBJECTS::AnimalSpecies::GORILLA:
                return "res/sounds/Ape-SoundBible.com-1651128585.wav";
            case OBJECTS::AnimalSpecies::HAWK:
                return "res/sounds/hawk_screeching-Mike_Koenig-1626170357.wav";
            case OBJECTS::AnimalSpecies::HIPPO:
                return "res/sounds/hippo2.wav";
            case OBJECTS::AnimalSpecies::HORSE:
                return "res/sounds/HorseNeigh-SoundBible.com-1740540960.wav";
            case OBJECTS::AnimalSpecies::KOALA:
                return "res/sounds/koala1.wav";
            case OBJECTS::AnimalSpecies::LEOPARD:
                return "res/sounds/leopard3.wav";
            case OBJECTS::AnimalSpecies::LION:
                return "res/sounds/GrowlingLion-SoundBible.com-495747737.wav";
            case OBJECTS::AnimalSpecies::LOCUST:
                return "res/sounds/Grasshopper-SoundBible.com-1592176719.wav";
            case OBJECTS::AnimalSpecies::MONKEY:
                return "res/sounds/gibbon-monkey-daniel_simon.wav";
            case OBJECTS::AnimalSpecies::OSPREY:
                return "res/sounds/OspreyBirdCall-SoundBible.com-525395846.wav";
            case OBJECTS::AnimalSpecies::OWL:
                return "res/sounds/horned_owl-Mike_Koenig-1945374932.wav";
            case OBJECTS::AnimalSpecies::PEACOCK:
                return "res/sounds/PeacockBird-SoundBible.com-98379922.wav";
            case OBJECTS::AnimalSpecies::PELICAN:
                return "res/sounds/PelicanChick-SoundBible.com-549990839.wav";
            case OBJECTS::AnimalSpecies::PIG:
                return "res/sounds/PigOink-SoundBible.com-1424738584.wav";
            case OBJECTS::AnimalSpecies::POLAR_BEAR:
                return "res/sounds/bear_polar.wav";
            case OBJECTS::AnimalSpecies::RACCOON:
                return "res/sounds/Raccoon-J_Dawg-688727657.wav";
            case OBJECTS::AnimalSpecies::RAVEN:
                return "res/sounds/Raven-SoundBible.com-1790882934.wav";
            case OBJECTS::AnimalSpecies::SHEEP:
                return "res/sounds/SheepBleating-SoundBible.com-1373580685.wav";
            case OBJECTS::AnimalSpecies::TURKEY:
                return "res/sounds/Turkey_Call_2-JimBob-26023545.wav";
            case OBJECTS::AnimalSpecies::WOLF:
                return "res/sounds/Howl.wav";
            default:
                // No sound exists.
                return "";
        }
    }
}
