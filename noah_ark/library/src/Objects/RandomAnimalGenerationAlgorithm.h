#pragma once

#include <memory>
#include <random>
#include "Graphics/AnimationSequence.h"
#include "Inventory/Inventory.h"
#include "Maps/TileMap.h"
#include "Objects/Animal.h"
#include "Resources/Assets.h"

namespace OBJECTS
{
    /// An algorithm for generating random animals in the game world.
    /// This class encapsulates the following details:
    /// - Selecting which animal to randomly generate, based on which
    ///     animals haven't yet been collected in a player's inventory.
    ///     Only animals that haven't yet been fully collected by a player
    ///     are generated in order to make it easier for players to
    ///     collect all of the animals.
    /// - Determining where on a tile map to generate the random animal.
    ///     Animals should only be generated on spots on which they can
    ///     move.
    class RandomAnimalGenerationAlgorithm
    {
    public:
        static std::shared_ptr<Animal> GenerateAnimal(
            const INVENTORY::Inventory& inventory,
            const MAPS::TileMap& tile_map,
            std::random_device& random_number_generator,
            RESOURCES::Assets& assets);

    private:
        static std::shared_ptr<Animal> MakeAnimal(
            const AnimalType& animal_type,
            const float world_x_position,
            const float world_y_position,
            RESOURCES::Assets& assets);
        static std::shared_ptr<GRAPHICS::AnimationSequence> GetAnimalAnimation(const AnimalType& animal_type);
    };
}
