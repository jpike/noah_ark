#pragma once

namespace OBJECTS
{
    /// A member of Noah's family that has to be gathered.
    /// An enum is wrapped in a struct to provid named scoping
    /// while also allowing implicit conversion to integers.
    /// 
    struct FamilyMember
    {
        /// The different types of Noah's family members to be gathered.
        enum Type
        {
            NOAH_WIFE,
            SHEM,
            SHEM_WIFE,
            HAM,
            HAM_WIFE,
            JAPHETH,
            JAPHETH_WIFE,
            /// An enum value is reserved to make it easier to determine the number of types.
            COUNT
        };
    };
}
