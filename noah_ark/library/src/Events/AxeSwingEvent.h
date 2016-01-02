#pragma once

#include <memory>
#include "Math/Rectangle.h"
#include "Objects/Axe.h"

/// Holds code representing in-game events that may cross subsystem boundaries.
namespace EVENTS
{
    /// An event describing the swinging of an axe, primarily for the
    /// purposes of detecting when an axe being swung collides with
    /// other objects.
    class AxeSwingEvent
    {
    public:
        // METHODS.
        /// Constructor.
        /// @param[in]  axe - The axe that is being swung.
        explicit AxeSwingEvent(const std::shared_ptr<OBJECTS::Axe>& axe);

        /// Determines if the axe is fully swung out.
        /// @return True if the axe is fully swung out; false otherwise.
        bool FullySwungOut() const;

        /// Gets the world boundaries of the axe's blade.
        /// @return The world boundaries of the axe's blade.
        MATH::FloatRectangle GetBladeBounds() const;

        // PUBLIC MEMBER VARIBLES FOR EASY ACCESS.
        /// The axe that was swung for this event.
        std::shared_ptr<OBJECTS::Axe> Axe;
    };
}
