#pragma once

#include <memory>
#include <vector>
#include <SFML/System.hpp>
#include "Graphics/Renderer.h"
#include "Math/RandomNumberGenerator.h"
#include "Math/Vector2.h"
#include "Objects/Animal.h"

// Forward declarations.
namespace OBJECTS
{
    class Noah;
}

namespace GAMEPLAY
{
    /// A group of animals that are currently following Noah
    /// (as opposed to being in the ark).
    class FollowingAnimalGroup
    {
    public:
        /// The dimension (width and height) of the group, in pixels.
        static constexpr float DIMENSION_IN_PIXELS = 32.0f;

        void Add(const std::shared_ptr<OBJECTS::Animal>& animal);

        void Update(const sf::Time& elapsed_time);

        /// The instance of Noah the animals are following.
        OBJECTS::Noah* Noah = nullptr;
        /// The current center position of the group within the world.
        MATH::Vector2f CurrentCenterWorldPosition = MATH::Vector2f();
        /// The animals currently in the group.
        /// Stored as shared pointers to allow for updating the animals
        /// while also sharing ownership with other code.
        std::vector< std::shared_ptr<OBJECTS::Animal> > Animals = {};

    private:
        /// A random number generator used for giving animals random positions in the group.
        MATH::RandomNumberGenerator RandomNumberGenerator = {};
        /// The total elapsed time for the group.  Used for some jumping animations.
        sf::Time ElapsedTime = sf::Time::Zero;
        /// The current destination world position for the animal group.  This is tracked
        /// separately from the current center world position to allow for smoother
        /// transitions when the player changes facing directions.
        MATH::Vector2f CurrentDestinationCenterWorldPosition = MATH::Vector2f();
    };
}
