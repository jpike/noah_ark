#include "Objects/ObjectType.h"

namespace OBJECTS
{
    ObjectType::Type ObjectType::FromString(const std::string& object_type_string)
    {
        if ("TREE" == object_type_string)
        {
            return TREE;
        }
        else
        {
            return INVALID;
        }
    }
}
