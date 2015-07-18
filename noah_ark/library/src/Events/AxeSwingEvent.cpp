#include <stdexcept>
#include "Events/AxeSwingEvent.h"

namespace EVENTS
{
    AxeSwingEvent::AxeSwingEvent(const std::shared_ptr<OBJECTS::Axe>& axe) :
    Axe(axe)
    {
        // MAKE SURE AN AXE WAS PROVIDED.
        bool axe_exists = (nullptr != Axe);
        if (!axe_exists)
        {
            // Provide visibility into the error.
            throw std::invalid_argument("Null axe given to axe swing event.");
        }
    }

    bool AxeSwingEvent::FullySwungOut() const
    {
        bool axe_fully_swung_out = Axe->FullySwungOut();
        return axe_fully_swung_out;
    }

    MATH::FloatRectangle AxeSwingEvent::GetBladeBounds() const
    {
        MATH::FloatRectangle axe_blade_bounds = Axe->GetBladeBounds();
        return axe_blade_bounds;
    }
}
