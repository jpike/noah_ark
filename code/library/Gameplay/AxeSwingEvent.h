#pragma once

#include <memory>
#include "Math/Rectangle.h"
#include "Objects/Axe.h"

namespace GAMEPLAY
{
    /// An event describing the swinging of an axe, primarily for the
    /// purposes of detecting when an axe being swung collides with
    /// other objects.
    class AxeSwingEvent
    {
    public:
        // CONSTRUCTION.
        explicit AxeSwingEvent(const std::shared_ptr<OBJECTS::Axe>& axe);

        // OTHER METHODS.
        bool FullySwungOut() const;
        MATH::FloatRectangle GetBladeBounds() const;

        // PUBLIC MEMBER VARIBLES FOR EASY ACCESS.
        /// The axe that was swung for this event.
        std::shared_ptr<OBJECTS::Axe> Axe;
    };
}
