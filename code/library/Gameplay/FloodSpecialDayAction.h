#pragma once

namespace GAMEPLAY
{
    /// Special actions that can be performed on specific days of the flood.
    enum class FloodSpecialDayAction
    {
        /// Indicates no special action should take place.
        NONE = 0,
        /// Similar to "none" but indicates one of the other actions has been completed.
        /// This is a special "signaling" enum value, which is sort of a bit of a hack.
        CURRENT_DAY_ACTION_COMPLETED, 
        /// The raven should be sent out the first time.
        SEND_OUT_RAVEN_FIRST_TIME,
        /// The dove should be sent out the first time.
        SEND_OUT_DOVE_FIRST_TIME,
        /// The dove should be sent out the second time.
        SEND_OUT_DOVE_SECOND_TIME,
        /// The dove should be sent out the final (third) time.
        SEND_OUT_DOVE_FINAL_TIME
    };
}
