#include "Resources/AnimalSounds.h"

namespace RESOURCES
{
    /// Gets the sound effect ID for the animal species, if one exists.
    /// @param[in]  animal_species - The species for which to get the sound effect.
    /// @return The sound ID for the animal, if one exists (invalid otherwise).
    RESOURCES::AssetId AnimalSounds::GetSound(const OBJECTS::AnimalSpecies animal_species)
    {
        // GET THE SOUND EFFECT FOR THE SPECIES.
        switch (animal_species)
        {
            case OBJECTS::AnimalSpecies::ALLIGATOR:
                return RESOURCES::AssetId::ALLIGATOR_SOUND;
            case OBJECTS::AnimalSpecies::BABOON:
                return RESOURCES::AssetId::BABOON_SOUND;
            case OBJECTS::AnimalSpecies::BAT:
                return RESOURCES::AssetId::BAT_SOUND;
            case OBJECTS::AnimalSpecies::BEAR:
                return RESOURCES::AssetId::BEAR_SOUND;
            case OBJECTS::AnimalSpecies::BEE:
                return RESOURCES::AssetId::BEE_SOUND;
            case OBJECTS::AnimalSpecies::BOBCAT:
                return RESOURCES::AssetId::BOBCAT_SOUND;
            case OBJECTS::AnimalSpecies::BUTTERFLY:
                return RESOURCES::AssetId::BUTTERFLY_SOUND;
            case OBJECTS::AnimalSpecies::CAMEL:
                return RESOURCES::AssetId::CAMEL_SOUND;
            case OBJECTS::AnimalSpecies::CAT:
                return RESOURCES::AssetId::CAT_SOUND;
            case OBJECTS::AnimalSpecies::CATTLE:
                return RESOURCES::AssetId::CATTLE_SOUND;
            case OBJECTS::AnimalSpecies::CHEETAH:
                return RESOURCES::AssetId::CHEETAH_SOUND;
            case OBJECTS::AnimalSpecies::CHICKEN:
                return RESOURCES::AssetId::CHICKEN_SOUND;
            case OBJECTS::AnimalSpecies::CHIPMUNK:
                return RESOURCES::AssetId::CHIPMUNK_SOUND;
            case OBJECTS::AnimalSpecies::CRICKET:
                return RESOURCES::AssetId::CRICKET_SOUND;
            case OBJECTS::AnimalSpecies::DEER:
                return RESOURCES::AssetId::DEER_SOUND;
            case OBJECTS::AnimalSpecies::DOG:
                return RESOURCES::AssetId::DOG_SOUND;
            case OBJECTS::AnimalSpecies::DOVE:
                return RESOURCES::AssetId::DOVE_SOUND;
            case OBJECTS::AnimalSpecies::DUCK:
                return RESOURCES::AssetId::DUCK_SOUND;
            case OBJECTS::AnimalSpecies::EAGLE:
                return RESOURCES::AssetId::EAGLE_SOUND;
            case OBJECTS::AnimalSpecies::ELEPHANT:
                return RESOURCES::AssetId::ELEPHANT_SOUND;
            case OBJECTS::AnimalSpecies::FOX:
                return RESOURCES::AssetId::FOX_SOUND;
            case OBJECTS::AnimalSpecies::FROG:
                return RESOURCES::AssetId::FROG_SOUND;
            case OBJECTS::AnimalSpecies::GOAT:
                return RESOURCES::AssetId::GOAT_SOUND;
            case OBJECTS::AnimalSpecies::GOOSE:
                return RESOURCES::AssetId::GOOSE_SOUND;
            case OBJECTS::AnimalSpecies::GORILLA:
                return RESOURCES::AssetId::GORILLA_SOUND;
            case OBJECTS::AnimalSpecies::GUINEA_PIG:
                return RESOURCES::AssetId::GUINEA_PIG_SOUND;
            case OBJECTS::AnimalSpecies::HAWK:
                return RESOURCES::AssetId::HAWK_SOUND;
            case OBJECTS::AnimalSpecies::HIPPO:
                return RESOURCES::AssetId::HIPPO_SOUND;
            case OBJECTS::AnimalSpecies::HORSE:
                return RESOURCES::AssetId::HORSE_SOUND;
            case OBJECTS::AnimalSpecies::HYENA:
                return RESOURCES::AssetId::HYENA_SOUND;
            case OBJECTS::AnimalSpecies::KOALA:
                return RESOURCES::AssetId::KOALA_SOUND;
            case OBJECTS::AnimalSpecies::LEOPARD:
                return RESOURCES::AssetId::LEOPARD_SOUND;
            case OBJECTS::AnimalSpecies::LION:
                return RESOURCES::AssetId::LION_SOUND;
            case OBJECTS::AnimalSpecies::LOCUST:
                return RESOURCES::AssetId::LOCUST_SOUND;
            case OBJECTS::AnimalSpecies::MONKEY:
                return RESOURCES::AssetId::MONKEY_SOUND;
            case OBJECTS::AnimalSpecies::OSPREY:
                return RESOURCES::AssetId::OSPREY_SOUND;
            case OBJECTS::AnimalSpecies::OWL:
                return RESOURCES::AssetId::OWL_SOUND;
            case OBJECTS::AnimalSpecies::PEACOCK:
                return RESOURCES::AssetId::PEACOCK_SOUND;
            case OBJECTS::AnimalSpecies::PELICAN:
                return RESOURCES::AssetId::PELICAN_SOUND;
            case OBJECTS::AnimalSpecies::PIG:
                return RESOURCES::AssetId::PIG_SOUND;
            case OBJECTS::AnimalSpecies::POLAR_BEAR:
                return RESOURCES::AssetId::POLAR_BEAR_SOUND;
            case OBJECTS::AnimalSpecies::RACCOON:
                return RESOURCES::AssetId::RACCOON_SOUND;
            case OBJECTS::AnimalSpecies::RAVEN:
                return RESOURCES::AssetId::RAVEN_SOUND;
            case OBJECTS::AnimalSpecies::SHEEP:
                return RESOURCES::AssetId::SHEEP_SOUND;
            case OBJECTS::AnimalSpecies::SNAKE:
                return RESOURCES::AssetId::SNAKE_SOUND;
            case OBJECTS::AnimalSpecies::SQUIRREL:
                return RESOURCES::AssetId::SQUIRREL_SOUND;
            case OBJECTS::AnimalSpecies::TURKEY:
                return RESOURCES::AssetId::TURKEY_SOUND;
            case OBJECTS::AnimalSpecies::WOLF:
                return RESOURCES::AssetId::WOLF_SOUND;
            default:
                // No sound exists.
                return RESOURCES::AssetId::INVALID;
        }
    }
}
