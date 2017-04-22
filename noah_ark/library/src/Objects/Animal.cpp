#include <cassert>
#include "Objects/Animal.h"

namespace OBJECTS
{
    /// Constructor.
    /// @param[in]  species - The species of the animal.
    /// @param[in]  gender - The gender of the animal.
    AnimalType::AnimalType(const AnimalSpecies species, const AnimalGender gender) :
        Species(species),
        Gender(gender),
        MoveSpeedInPixelsPerSecond(GetMoveSpeedInPixelsPerSecond(species))
    {}

    /// Determines whether 2 animal types are equal.
    /// @param[in]  other - The other animal type to compare against.
    /// @return True if the animal types are equal; false otherwise.
    bool AnimalType::operator==(const AnimalType& other) const
    {
        // The move speed is excluded since it should effectively be a constant
        // that is always initialized during construction.
        if (this->Species != other.Species) return false;
        if (this->Gender != other.Gender) return false;

        // All fields are equal.
        return true;
    }

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

    /// Gets the movement speed for a given animal species.
    /// For now, this speed is only intended to vary per species.
    /// @param[in]  species - The species of animal to get the movement speed for.
    /// @return The movement speed to the animal species (in pixels per second).
    float AnimalType::GetMoveSpeedInPixelsPerSecond(const AnimalSpecies species)
    {
        // The movement speeds are somewhat arbitrarily chosen based on
        // what feels best, but the speeds are also somewhat intended
        // to be realistic (relative to each other) based on actual species.
        // However, no real research has been done regarding this.
        switch (species)
        {
            case AnimalSpecies::CAMEL:
                return 32.0f;
            case AnimalSpecies::CATTLE:
                return 16.0f;
            case AnimalSpecies::DOVE:
                return 32.0f;
            case AnimalSpecies::FROG:
                return 16.0f;
            case AnimalSpecies::LOCUST:
                return 16.0f;
            case AnimalSpecies::RAVEN:
                return 32.0f;
            case AnimalSpecies::SHEEP:
                return 16.0f;
            default:
                // Don't let invalid animal species move.
                return 0.0f;
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
