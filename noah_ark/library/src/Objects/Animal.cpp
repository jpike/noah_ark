#include <cassert>
#include "Objects/Animal.h"

namespace OBJECTS
{
    /// Constructor.
    /// @param[in]  species - The species of the animal.
    /// @param[in]  gender - The gender of the animal.
    AnimalType::AnimalType(const AnimalSpecies species, const AnimalGender gender) :
        Species(species),
        Gender(gender)
    {}

    /// Determines if this type of animal is considered clean by Biblical standards.
    /// @return True if this type of animal is clean; false otherwise.
    bool AnimalType::Clean() const
    {
        // DETERMINE IF THIS SPECIES IS CLEAN.
        switch (Species)
        {
            // RETURN TRUE FOR ALL CLEAN ANIMALS.
            case AnimalSpecies::CATTLE: // Deuteronomy 14:4
            case AnimalSpecies::DOVE: // Genesis 15:9
            case AnimalSpecies::LOCUST: // Leviticus 11:22
            case AnimalSpecies::SHEEP: // Deuteronomy 14:4
                return true;
            // RETURN FALSE FOR ALL UNCLEAN ANIMALS.
            case AnimalSpecies::CAMEL: // Leviticus 11:4
            case AnimalSpecies::FROG: // Leviticus 11:9-10
            case AnimalSpecies::RAVEN: // Leviticus 11:13-15
                return false;
            default:
                assert(false);
                // Assume it isn't clean.
                return false;
        }
    }

    /// Determines if the specific animal type can fly.
    /// @return True if this type of animal can fly; false otherwise.
    bool AnimalType::CanFly() const
    {
        // DETERMINE IF THIS SPECIES CAN FLY.
        switch (Species)
        {
            // RETURN TRUE FOR ALL FLYING ANIMALS.
            case AnimalSpecies::DOVE:
            case AnimalSpecies::LOCUST:
            case AnimalSpecies::RAVEN:
                return true;
            // RETURN FALSE FOR ALL NON-FLYING ANIMALS.
            case AnimalSpecies::CAMEL:
            case AnimalSpecies::CATTLE:
            case AnimalSpecies::FROG:
            case AnimalSpecies::SHEEP:
            default:
                return false;
        }
    }

    const std::string Animal::ANIMATION_NAME = "AnimalAnimation";

    /// Constructor.
    /// @param[in]  type - The type of the animal.
    /// @param[in]  sprite - The graphical sprite for the animal.
    Animal::Animal(const AnimalType& type, const GRAPHICS::AnimatedSprite& sprite) :
        Type(type),
        Sprite(sprite)
    {}
}
