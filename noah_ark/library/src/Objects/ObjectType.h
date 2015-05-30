#pragma once

#include <string>

namespace OBJECTS
{
    /// Defines the different types of objects supported by the game.
    /// The enum is wrapped in a struct to provide scoping and allow
    /// functions to be more closely associated with this type.
    struct ObjectType
    {
        /// Enumerates the different object types.
        enum Type
        {
            INVALID,    ///< An invalid type of game object.
            TREE    ///< A tree object.
        };

        /// Gets the object type from the corresponding string.
        /// @param[in]  object_type_string - The object type as a string.
        /// @return The object type identified by the string.
        static Type FromString(const std::string& object_type_string);
    };
}
