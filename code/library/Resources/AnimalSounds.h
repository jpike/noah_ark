#pragma once

#include "Objects/Animal.h"
#include "Resources/AssetId.h"

namespace RESOURCES
{
    /// A specialized wrapper around resources containing logic
    /// for obtaining animal sounds.
    class AnimalSounds
    {
    public:
        static RESOURCES::AssetId GetSound(const OBJECTS::AnimalSpecies animal_species);
    };
}
