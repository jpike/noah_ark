#pragma once

#include <memory>
#include "Audio/SoundEffect.h"
#include "Objects/Animal.h"
#include "Resources/Assets.h"

namespace RESOURCES
{
    /// A specialized wrapper around resources containing logic
    /// for obtaining animal sounds.
    class AnimalSounds
    {
    public:
        static std::shared_ptr<AUDIO::SoundEffect> GetSound(
            const OBJECTS::AnimalSpecies animal_species,
            Assets& assets);
    };
}
