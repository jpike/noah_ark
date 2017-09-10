#pragma once

#include <string>
#include "Objects/Animal.h"

namespace RESOURCES
{
    /// A specialized wrapper around resources containing logic
    /// for obtaining animal sounds.
    class AnimalSounds
    {
    public:
        static std::string GetSound(const OBJECTS::AnimalSpecies animal_species);
    };
}
