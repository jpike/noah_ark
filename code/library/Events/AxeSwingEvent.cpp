#include "Core/NullChecking.h"
#include "Events/AxeSwingEvent.h"

namespace EVENTS
{
    /// Constructor.
    /// @param[in]  axe - The axe that is being swung.
    AxeSwingEvent::AxeSwingEvent(const std::shared_ptr<OBJECTS::Axe>& axe) :
    Axe(axe)
    {
        // MAKE SURE AN AXE WAS PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(Axe, "Null axe given to axe swing event.");
    }

    /// Determines if the axe is fully swung out.
    /// @return True if the axe is fully swung out; false otherwise.
    bool AxeSwingEvent::FullySwungOut() const
    {
        bool axe_fully_swung_out = Axe->FullySwungOut();
        return axe_fully_swung_out;
    }

    /// Gets the world boundaries of the axe's blade.
    /// @return The world boundaries of the axe's blade.
    MATH::FloatRectangle AxeSwingEvent::GetBladeBounds() const
    {
        MATH::FloatRectangle axe_blade_bounds = Axe->GetBladeBounds();
        return axe_blade_bounds;
    }
}
