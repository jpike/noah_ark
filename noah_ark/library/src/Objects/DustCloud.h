#pragma once

#include <memory>
#include <SFML/System.hpp>
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Texture.h"

namespace OBJECTS
{
    /// A cloud of dust that appears after a tree has been chopped down and
    /// before it has turned into wooden logs.  The cloud is animated and
    /// only lasts for a short time during this transition.
    class DustCloud
    {
    public:
        // CONSTRUCTION.
        explicit DustCloud(const std::shared_ptr<GRAPHICS::Texture>& texture);

        // STATUS QUERYING.
        bool HasDisappeared() const;

        // UPDATING.
        void Update(const sf::Time& elapsed_time);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the dust cloud.
        GRAPHICS::AnimatedSprite Sprite;
        /// The total time the cloud has been in existence.
        /// Increased during each update call.
        float TotalElapsedTimeInSeconds;
    };
}
