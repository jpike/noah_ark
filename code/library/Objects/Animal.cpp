#include "Objects/Animal.h"

namespace OBJECTS
{
    std::array<std::string, AnimalSpecies::COUNT> AnimalSpecies::NAMES
    {
        "Alligator",
        "Baboon",
        "Bat",
        "Bear",
        "Bee",
        "Bobcat",
        "Butterfly",
        "Camel",
        "Cat",
        "Cattle",
        "Cheetah",
        "Chicken",
        "Chipmunk",
        "Cricket",
        "Deer",
        "Dog",
        "Dove",
        "Duck",
        "Eagle",
        "Elephant",
        "Fox",
        "Frog",
        "Goat",
        "Goose",
        "Gorilla",
        "Guinea Pig",
        "Hawk",
        "Hippo",
        "Horse",
        "Hyena",
        "Koala",
        "Leopard",
        "Lion",
        "Locust",
        "Monkey",
        "Osprey",
        "Owl",
        "Peacock",
        "Pelican",
        "Pig",
        "Polar Bear",
        "Raccoon",
        "Raven",
        "Sheep",
        "Snake",
        "Squirrel",
        "Turkey",
        "Wolf"
    };

    /// Gets the movement speed for a given animal species.
    /// For now, this speed is only intended to vary per species.
    /// @param[in]  species - The species of animal to get the movement speed for.
    /// @return The movement speed to the animal species (in pixels per second).
    float AnimalType::GetMoveSpeedInPixelsPerSecond(const AnimalSpecies::Value species)
    {
        // The movement speeds are somewhat arbitrarily chosen based on
        // what feels best, but the speeds are also somewhat intended
        // to be realistic (relative to each other) based on actual species.
        // However, no real research has been done regarding this.
        switch (species)
        {
            case AnimalSpecies::ALLIGATOR:
                return 32.0f;
            case AnimalSpecies::BABOON:
                return 16.0f;
            case AnimalSpecies::BAT:
                return 32.0f;
            case AnimalSpecies::BEAR:
                return 16.0f;
            case AnimalSpecies::BEE:
                return 16.0f;
            case AnimalSpecies::BOBCAT:
                return 32.0f;
            case AnimalSpecies::BUTTERFLY:
                return 16.0f;
            case AnimalSpecies::CAMEL:
                return 32.0f;
            case AnimalSpecies::CAT:
                return 16.0f;
            case AnimalSpecies::CATTLE:
                return 16.0f;
            case AnimalSpecies::CHEETAH:
                return 32.0f;
            case AnimalSpecies::CHICKEN:
                return 16.0f;
            case AnimalSpecies::CHIPMUNK:
                return 16.0f;
            case AnimalSpecies::CRICKET:
                return 16.0f;
            case AnimalSpecies::DEER:
                return 32.0f;
            case AnimalSpecies::DOG:
                return 16.0f;
            case AnimalSpecies::DOVE:
                return 32.0f;
            case AnimalSpecies::DUCK:
                return 16.0f;
            case AnimalSpecies::EAGLE:
                return 32.0f;
            case AnimalSpecies::ELEPHANT:
                return 32.0f;
            case AnimalSpecies::FOX:
                return 16.0f;
            case AnimalSpecies::FROG:
                return 16.0f;
            case AnimalSpecies::GOAT:
                return 16.0f;
            case AnimalSpecies::GOOSE:
                return 16.0f;
            case AnimalSpecies::GORILLA:
                return 16.0f;
            case AnimalSpecies::GUINEA_PIG:
                return 16.0f;
            case AnimalSpecies::HAWK:
                return 32.0f;
            case AnimalSpecies::HIPPO:
                return 16.0f;
            case AnimalSpecies::HORSE:
                return 32.0f;
            case AnimalSpecies::HYENA:
                return 16.0f;
            case AnimalSpecies::KOALA:
                return 16.0f;
            case AnimalSpecies::LEOPARD:
                return 32.0f;
            case AnimalSpecies::LION:
                return 32.0f;
            case AnimalSpecies::LOCUST:
                return 16.0f;
            case AnimalSpecies::MONKEY:
                return 16.0f;
            case AnimalSpecies::OSPREY:
                return 32.0f;
            case AnimalSpecies::OWL:
                return 32.0f;
            case AnimalSpecies::PEACOCK:
                return 16.0f;
            case AnimalSpecies::PELICAN:
                return 32.0f;
            case AnimalSpecies::PIG:
                return 16.0f;
            case AnimalSpecies::POLAR_BEAR:
                return 16.0f;
            case AnimalSpecies::RACCOON:
                return 16.0f;
            case AnimalSpecies::RAVEN:
                return 32.0f;
            case AnimalSpecies::SHEEP:
                return 16.0f;
            case AnimalSpecies::SNAKE:
                return 16.0f;
            case AnimalSpecies::SQUIRREL:
                return 16.0f;
            case AnimalSpecies::TURKEY:
                return 16.0f;
            case AnimalSpecies::WOLF:
                return 16.0f;
            default:
                // Don't let invalid animal species move.
                return 0.0f;
        }
    }

    /// Constructor.
    /// @param[in]  species - The species of the animal.
    /// @param[in]  gender - The gender of the animal.
    AnimalType::AnimalType(const AnimalSpecies::Value species, const AnimalGender::Value gender) :
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
            case AnimalSpecies::CHICKEN:
            case AnimalSpecies::CRICKET: // Leviticus 11:22
            case AnimalSpecies::DEER: // Deuteronomy 14:5
            case AnimalSpecies::DOVE: // Genesis 15:9
            case AnimalSpecies::DUCK:
            case AnimalSpecies::GOAT: // Deuteronomy 14:4
            case AnimalSpecies::GOOSE:
            case AnimalSpecies::LOCUST: // Leviticus 11:22
            case AnimalSpecies::PEACOCK:
            case AnimalSpecies::SHEEP: // Deuteronomy 14:4
            case AnimalSpecies::TURKEY:
                return true;
            // RETURN FALSE FOR ALL UNCLEAN ANIMALS.
            case AnimalSpecies::ALLIGATOR:
            case AnimalSpecies::BABOON:
            case AnimalSpecies::BAT: // Leviticus 11:19
            case AnimalSpecies::BEAR:
            case AnimalSpecies::BEE:
            case AnimalSpecies::BOBCAT:
            case AnimalSpecies::BUTTERFLY:
            case AnimalSpecies::CAMEL: // Leviticus 11:4
            case AnimalSpecies::CAT:
            case AnimalSpecies::CHEETAH:
            case AnimalSpecies::CHIPMUNK:
            case AnimalSpecies::DOG:
            case AnimalSpecies::EAGLE: // Leviticus 11:13
            case AnimalSpecies::ELEPHANT:
            case AnimalSpecies::FOX:
            case AnimalSpecies::FROG: // Leviticus 11:9-10
            case AnimalSpecies::GORILLA:
            case AnimalSpecies::GUINEA_PIG:
            case AnimalSpecies::HAWK: // Leviticus 11:16
            case AnimalSpecies::HIPPO:
            case AnimalSpecies::HORSE:
            case AnimalSpecies::HYENA:
            case AnimalSpecies::KOALA:
            case AnimalSpecies::LEOPARD:
            case AnimalSpecies::LION:
            case AnimalSpecies::MONKEY:
            case AnimalSpecies::OSPREY: // Leviticus 11:13
            case AnimalSpecies::OWL: // Leviticus 11:16
            case AnimalSpecies::PELICAN: // Leviticus 11:18
            case AnimalSpecies::PIG: // Leviticus 11:7
            case AnimalSpecies::POLAR_BEAR:
            case AnimalSpecies::RACCOON:
            case AnimalSpecies::RAVEN: // Leviticus 11:13-15
            case AnimalSpecies::SNAKE: // Leviticus 11:22
            case AnimalSpecies::SQUIRREL:
            case AnimalSpecies::WOLF:
                return false;
            default:
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
            case AnimalSpecies::BEE:
            case AnimalSpecies::BUTTERFLY:
            case AnimalSpecies::CHICKEN:
            case AnimalSpecies::DOVE:
            case AnimalSpecies::EAGLE:
            case AnimalSpecies::HAWK:
            case AnimalSpecies::LOCUST:
            case AnimalSpecies::OSPREY:
            case AnimalSpecies::OWL:
            case AnimalSpecies::PEACOCK:
            case AnimalSpecies::PELICAN:
            case AnimalSpecies::RAVEN:
            case AnimalSpecies::TURKEY:
                return true;
            // RETURN FALSE FOR ALL NON-FLYING ANIMALS.
            case AnimalSpecies::ALLIGATOR:
            case AnimalSpecies::BABOON:
            case AnimalSpecies::BEAR:
            case AnimalSpecies::BOBCAT:
            case AnimalSpecies::CAMEL:
            case AnimalSpecies::CAT:
            case AnimalSpecies::CATTLE:
            case AnimalSpecies::CHEETAH:
            case AnimalSpecies::CHIPMUNK:
            case AnimalSpecies::DEER:
            case AnimalSpecies::DOG:
            case AnimalSpecies::FOX:
            case AnimalSpecies::FROG:
            case AnimalSpecies::GOAT:
            case AnimalSpecies::GORILLA:
            case AnimalSpecies::HIPPO:
            case AnimalSpecies::HORSE:
            case AnimalSpecies::KOALA:
            case AnimalSpecies::LEOPARD:
            case AnimalSpecies::LION:
            case AnimalSpecies::MONKEY:
            case AnimalSpecies::PIG:
            case AnimalSpecies::POLAR_BEAR:
            case AnimalSpecies::RACCOON:
            case AnimalSpecies::SHEEP:
            case AnimalSpecies::WOLF:
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
            case AnimalSpecies::ALLIGATOR:
            case AnimalSpecies::BEAR:
            case AnimalSpecies::FROG:
            case AnimalSpecies::HIPPO:
            case AnimalSpecies::POLAR_BEAR:
                return true;
            // RETURN FALSE FOR ALL NON-SWIMMING ANIMALS.
            case AnimalSpecies::BAT:
            case AnimalSpecies::BEE:
            case AnimalSpecies::BOBCAT:
            case AnimalSpecies::BUTTERFLY:
            case AnimalSpecies::CAMEL:
            case AnimalSpecies::CAT:
            case AnimalSpecies::CATTLE:
            case AnimalSpecies::CHEETAH:
            case AnimalSpecies::CHICKEN:
            case AnimalSpecies::CHIPMUNK:
            case AnimalSpecies::DEER:
            case AnimalSpecies::DOG:
            case AnimalSpecies::DOVE:
            case AnimalSpecies::EAGLE:
            case AnimalSpecies::FOX:
            case AnimalSpecies::GOAT:
            case AnimalSpecies::GORILLA:
            case AnimalSpecies::HAWK:
            case AnimalSpecies::HORSE:
            case AnimalSpecies::KOALA:
            case AnimalSpecies::LEOPARD:
            case AnimalSpecies::LION:
            case AnimalSpecies::LOCUST:
            case AnimalSpecies::MONKEY:
            case AnimalSpecies::OSPREY:
            case AnimalSpecies::OWL:
            case AnimalSpecies::PEACOCK:
            case AnimalSpecies::PELICAN:
            case AnimalSpecies::PIG:
            case AnimalSpecies::RACCOON:
            case AnimalSpecies::RAVEN:
            case AnimalSpecies::SHEEP:
            case AnimalSpecies::TURKEY:
            case AnimalSpecies::WOLF:
            default:
                return false;
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
        const RESOURCES::AssetId sound_id) :
    Type(type),
    Sprite(sprite),
    SoundId(sound_id)
    {}
}
