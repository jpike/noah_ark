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
            case AnimalSpecies::DEER: // Deuteronomy 14:5
            case AnimalSpecies::DOVE: // Genesis 15:9
            case AnimalSpecies::GOAT: // Deuteronomy 14:4
            case AnimalSpecies::LOCUST: // Leviticus 11:22
            case AnimalSpecies::SHEEP: // Deuteronomy 14:4
                return true;
            // RETURN FALSE FOR ALL UNCLEAN ANIMALS.
            case AnimalSpecies::BAT: // Leviticus 11:19
            case AnimalSpecies::CAMEL: // Leviticus 11:4
            case AnimalSpecies::EAGLE: // Leviticus 11:13
            case AnimalSpecies::FROG: // Leviticus 11:9-10
            case AnimalSpecies::HAWK: // Leviticus 11:16
            case AnimalSpecies::OSPREY: // Leviticus 11:13
            case AnimalSpecies::OWL: // Leviticus 11:16
            case AnimalSpecies::PELICAN: // Leviticus 11:18
            case AnimalSpecies::PIG: // Leviticus 11:7
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
            case AnimalSpecies::BAT:
            case AnimalSpecies::DOVE:
            case AnimalSpecies::EAGLE:
            case AnimalSpecies::HAWK:
            case AnimalSpecies::LOCUST:
            case AnimalSpecies::OSPREY:
            case AnimalSpecies::OWL:
            case AnimalSpecies::PELICAN:
            case AnimalSpecies::RAVEN:
                return true;
            // RETURN FALSE FOR ALL NON-FLYING ANIMALS.
            case AnimalSpecies::CAMEL:
            case AnimalSpecies::CATTLE:
            case AnimalSpecies::DEER:
            case AnimalSpecies::FROG:
            case AnimalSpecies::GOAT:
            case AnimalSpecies::PIG:
            case AnimalSpecies::SHEEP:
            default:
                return false;
        }
    }

    /// Determines if the specific animal type can swim.
    /// @return True if this type of animal can swim; false otherwise.
    bool AnimalType::CanSwim() const
    {
        // DETERMINE IF THIS SPECIES CAN SWIM.
        switch (Species)
        {
            // RETURN TRUE FOR ALL SWIMMING ANIMALS.
            case AnimalSpecies::FROG:
                return true;
            // RETURN FALSE FOR ALL NON-SWIMMING ANIMALS.
            case AnimalSpecies::BAT:
            case AnimalSpecies::CAMEL:
            case AnimalSpecies::CATTLE:
            case AnimalSpecies::DEER:
            case AnimalSpecies::DOVE:
            case AnimalSpecies::EAGLE:
            case AnimalSpecies::GOAT:
            case AnimalSpecies::HAWK:
            case AnimalSpecies::LOCUST:
            case AnimalSpecies::OSPREY:
            case AnimalSpecies::OWL:
            case AnimalSpecies::PELICAN:
            case AnimalSpecies::PIG:
            case AnimalSpecies::RAVEN:
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
            case AnimalSpecies::BAT:
                return 32.0f;
            case AnimalSpecies::CAMEL:
                return 32.0f;
            case AnimalSpecies::CATTLE:
                return 16.0f;
            case AnimalSpecies::DEER:
                return 32.0f;
            case AnimalSpecies::DOVE:
                return 32.0f;
            case AnimalSpecies::EAGLE:
                return 32.0f;
            case AnimalSpecies::FROG:
                return 16.0f;
            case AnimalSpecies::GOAT:
                return 16.0f;
            case AnimalSpecies::HAWK:
                return 32.0f;
            case AnimalSpecies::LOCUST:
                return 16.0f;
            case AnimalSpecies::OSPREY:
                return 32.0f;
            case AnimalSpecies::OWL:
                return 32.0f;
            case AnimalSpecies::PELICAN:
                return 32.0f;
            case AnimalSpecies::PIG:
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
    /// @param[in]  sound_id - The ID of the sound for the animal (optional).
    Animal::Animal(
        const AnimalType& type, 
        const GRAPHICS::AnimatedSprite& sprite,
        const std::string& sound_id) :
    Type(type),
    Sprite(sprite),
    SoundId(sound_id)
    {}
}
