#pragma once

#include <string>
#include <SFML/System.hpp>
#include "Graphics/AnimatedSprite.h"

namespace OBJECTS
{
    /// Enumerates the different genders that animals may have.
    enum class AnimalGender
    {
        /// Indicates a male animal.
        MALE,
        /// Indicates a female animal.
        FEMALE,
        /// A non-gender value that supports determining the number of genders
        /// for algorithms that need this information.
        COUNT
    };

    /// Enumerates the different species of animals in the game.
    /// Species are listed in alphabetical order to make them easier
    /// to find.
    enum class AnimalSpecies
    {
        CAMEL,
        CATTLE,
        DOVE,
        FROG,
        LOCUST,
        RAVEN,
        SHEEP,
        /// A non-species value that supports determining the number of species
        /// for algorithms that need this information.
        COUNT
    };

    /// A single description that describes the "type" of an animal in the game.
    /// Encapsulating both the species and gender of an animal in a single
    /// abstraction simplifies certain logic in the game.
    /// Movement speed is also included since it is intended to vary but
    /// be consistent within a given type of animal.
    class AnimalType
    {
    public:
        /// CONSTRUCTION.
        explicit AnimalType(
            const AnimalSpecies species, 
            const AnimalGender gender);

        /// OTHER PUBLIC METHODS.
        bool Clean() const;
        bool CanFly() const;

        /// PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The species of the animal.
        AnimalSpecies Species;
        /// The gender of the animal.
        AnimalGender Gender;
        /// How fast the animal moves in the world (in pixels per second).
        float MoveSpeedInPixelsPerSecond;

    private:
        /// HELPER METHODS.
        static float GetMoveSpeedInPixelsPerSecond(const AnimalSpecies species);
    };

    /// An animal in the game.  A single animal class exists that is parameterized
    /// in order to support a wide variety of animals without a huge number of classes
    /// (and without a huge overhead to add support for new types of animals).
    /// 
    /// Animals appear in the overworld and move toward the player (Genesis 6:20).
    /// The player collects them to be put on the ark during the flood.
    class Animal
    {
    public:
        /// STATIC CONSTANTS.
        /// The name of the animation used for animals.
        static const std::string ANIMATION_NAME;

        /// CONSTRUCTION.
        explicit Animal(const AnimalType& type, const GRAPHICS::AnimatedSprite& sprite);

        /// PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The type of the animal.
        AnimalType Type;
        /// The graphical sprite for the animal.
        GRAPHICS::AnimatedSprite Sprite;
    };
}
