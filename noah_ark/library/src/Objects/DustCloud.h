#pragma once

#include "Graphics/AnimatedSprite.h"

namespace OBJECTS
{
    /// A cloud of dust that appears after a tree has been chopped down and
    /// before it has turned into wooden logs.  The cloud is animated and
    /// only lasts for a short time during this transition.
    class DustCloud
    {
    public:
        // STATUS QUERYING.
        /// Determines if the cloud has disappeared or faded away.
        /// @return True if the cloud has disappeared; false otherwise.
        bool HasDisappeared() const;

        // UPDATING.
        /// Updates the state of the dust cloud based on the elapsed time.
        /// Dust clouds animate and disappear/fade away over time.
        /// @param[in]  elapsed_time_in_seconds - The elapsed time for which
        /// to update the dust cloud.
        void Update(const float elapsed_time_in_seconds);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The graphical sprite for the dust cloud.
        GRAPHICS::AnimatedSprite Sprite = GRAPHICS::AnimatedSprite();
        /// The total time the cloud has been in existence.
        /// Increased during each update call.
        float TotalElapsedTimeInSeconds = 0.0f;
    };
}
