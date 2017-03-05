#include <iostream>
#include "Objects/RandomAnimalGenerationAlgorithm.h"

namespace OBJECTS
{
    /// Attempts to generate an animal that can be placed in the provided tile map.
    /// The randomly generated animal will be one that hasn't been fully collected
    /// (both genders, appropriate number of) in the inventory.
    /// @param[in]  inventory - The inventory indicating which animals have been
    ///     collected so far.
    /// @param[in]  tile_map - The tile map in which the animal needs to be generated.
    /// @param[in,out]  random_number_generator - The random number generator to use
    ///     for the algorithm.
    /// @param[in,out]  assets - The assets from which to get assets necessary for an animal.
    /// @return An animal, if one was successfully generated; null otherwise.
    std::shared_ptr<Animal> RandomAnimalGenerationAlgorithm::GenerateAnimal(
        const INVENTORY::Inventory& inventory,
        const MAPS::TileMap& tile_map,
        std::random_device& random_number_generator,
        RESOURCES::Assets& assets)
    {
        /// @todo   More advanced logic so that multiple attempts are made to try
        /// and find an animal/position that can be generated (as opposed to giving up early)?

        // DETERMINE THE SPECIES OF ANIMAL TO GENERATE.
        const unsigned int ANIMAL_SPECIES_COUNT = static_cast<unsigned int>(AnimalSpecies::COUNT);
        unsigned int random_number_for_species = random_number_generator();
        AnimalSpecies random_species = static_cast<AnimalSpecies>(random_number_for_species % ANIMAL_SPECIES_COUNT);
        std::cout << "Random species: " << static_cast<unsigned int>(random_species) << std::endl;

        // DETERMINE THE GENDER OF ANIMAL TO GENERATE.
        const unsigned int GENDER_COUNT = static_cast<unsigned int>(AnimalGender::COUNT);
        unsigned int random_number_for_gender = random_number_generator();
        AnimalGender random_gender = static_cast<AnimalGender>(random_number_for_gender % GENDER_COUNT);
        std::cout << "Random gender: " << static_cast<unsigned int>(random_gender) << std::endl;

        // CHECK IF THE ANIMAL TYPE HAS BEEN FULLY COLLECTED.
        AnimalType animal_type(random_species, random_gender);
        bool animal_type_fully_collected = inventory.AnimalTypeFullyCollected(animal_type);
        if (animal_type_fully_collected)
        {
            std::cout << "Animal type fully collected." << std::endl;
            // DON'T GENERATE A RANDOM ANIMAL.
            // Since the randomly chosen animal type has already been fully collected,
            // there's no need to generate another instance of it.
            return nullptr;
        }

        // DETERMINE THE LOCATION OF THE TILE MAP AT WHICH THE ANIMAL SHOULD BE PLACED.
        MATH::FloatRectangle tile_map_world_boundaries = tile_map.GetWorldBoundingBox();
        /// @todo   Create more encapsulated random number generator functionality
        /// so that we can generate random numbers within a range.
        unsigned int tile_map_width = static_cast<unsigned int>(tile_map_world_boundaries.GetWidth());
        unsigned int tile_map_left_x_position = static_cast<unsigned int>(tile_map_world_boundaries.GetLeftXPosition());
        unsigned int tile_map_height = static_cast<unsigned int>(tile_map_world_boundaries.GetHeight());
        unsigned int tile_map_top_y_position = static_cast<unsigned int>(tile_map_world_boundaries.GetTopYPosition());
        unsigned int random_number_for_x_position = random_number_generator();
        unsigned int random_number_for_y_position = random_number_generator();
        unsigned int random_x_position = (random_number_for_x_position % tile_map_width) + tile_map_left_x_position;
        unsigned int random_y_position = (random_number_for_y_position % tile_map_height) + tile_map_top_y_position;
        std::shared_ptr<MAPS::Tile> tile_at_animal_generation_point = tile_map.GetTileAtWorldPosition(
            static_cast<float>(random_x_position), 
            static_cast<float>(random_y_position));
        assert(tile_at_animal_generation_point);
        std::cout << "Animal at: " << random_x_position << ", " << random_y_position << std::endl;

        /// @todo   Account for Noah's position?  We shouldn't generate animals directly on top of Noah.

        // Flying animals can be generated on any tile.
        bool animal_can_fly = animal_type.CanFly();
        if (!animal_can_fly)
        {
            /// @todo   We might need a more robust check of tile types here.
            bool tile_is_walkable = tile_at_animal_generation_point->IsWalkable();
            if (!tile_is_walkable)
            {
                // Non-flying animals can't be generated on unwalkable tiles.
                std::cout << "Tile isn't walkable." << std::endl;
                return nullptr;
            }
        }

        // CREATE THE ANIMAL DETERMINED BY THE ALGORITHM.
        std::shared_ptr<Animal> animal = MakeAnimal(
            animal_type,
            static_cast<float>(random_x_position),
            static_cast<float>(random_y_position),
            assets);
        return animal;
    }

    /// Attempts to make the specified animal.
    /// @param[in]  animal_type - The type of animal to make.
    /// @param[in]  world_x_position - The world x position of the center of the animal.
    /// @param[in]  world_y_position - The world y position of the center of the animal.
    /// @param[in,out]  assets - The assets from which to get assets necessary for the animal.
    /// @return The specified animal, if successfully created; null otherwise.
    std::shared_ptr<Animal> RandomAnimalGenerationAlgorithm::MakeAnimal(
        const AnimalType& animal_type,
        const float world_x_position,
        const float world_y_position,
        RESOURCES::Assets& assets)
    {
        // GET THE ANIMAL TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> animal_texture = assets.GetTexture(RESOURCES::ANIMAL_TEXTURE_ID);
        if (!animal_texture)
        {
            std::cout << "Failed to get animal texture." << std::endl;
            return nullptr;
        }

        // GET THE ANIMAL'S ANIMATION SEQUENCE.
        std::shared_ptr<GRAPHICS::AnimationSequence> animation_sequence = GetAnimalAnimation(animal_type);
        if (!animation_sequence)
        {
            std::cout << "Failed to get animal animation." << std::endl;
            return nullptr;
        }

        // CREATE THE ANIMAL'S SPRITE.
        GRAPHICS::AnimatedSprite animal_sprite(animal_texture, animation_sequence);
        animal_sprite.SetWorldPosition(world_x_position, world_y_position);

        // CREATE THE ANIMAL.
        /// @todo   Add sound effects for animals.
        std::shared_ptr<Animal> animal = std::make_shared<Animal>(animal_type, animal_sprite);
        return animal;
    }

    /// Attempts to get the animation sequence for a given type of animal.
    /// @param[in]  animal_type - The type of animal whose animation sequence to get.
    /// @return The animal's animation sequence, if successfully retrieved; null otherwise.
    std::shared_ptr<GRAPHICS::AnimationSequence> RandomAnimalGenerationAlgorithm::GetAnimalAnimation(const AnimalType& animal_type)
    {
        // DETERMINE THE GENDER OF THE ANIMAL.
        bool is_male = (AnimalGender::MALE == animal_type.Gender);
        bool is_female = (AnimalGender::FEMALE == animal_type.Gender);
        assert(is_male || is_female);

        // GET THE ANIMATION SEQUENCE FOR THE APPROPRIATE SPECIES.
        //const std::vector<MATH::IntRectangle>& frames;
        //const std::string OBJECTS::Animal::ANIMATION_NAME;
        const bool LOOPING = true;
        const sf::Time ANIMATION_DURATION = sf::seconds(1.0f);
        std::shared_ptr<GRAPHICS::AnimationSequence> animal_animation;
        switch (animal_type.Species)
        {
            case AnimalSpecies::CAMEL:
            {
                // FORM THE ANIMATION FOR A CAMEL.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 0 : 16;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(128, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(144, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case AnimalSpecies::CATTLE:
            {
                // FORM THE ANIMATION FOR CATTLE.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 0 : 16;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(96, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(112, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case AnimalSpecies::DOVE:
            {
                // FORM THE ANIMATION FOR A DOVE.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 0 : 16;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(32, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(48, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case AnimalSpecies::FROG:
            {
                // FORM THE ANIMATION FOR A FROG.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 0 : 16;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(160, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(176, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case AnimalSpecies::LOCUST:
            {
                // FORM THE ANIMATION FOR A LOCUST.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 0 : 16;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(192, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(208, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case AnimalSpecies::RAVEN:
            {
                // FORM THE ANIMATION FOR A RAVEN.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 0 : 16;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(0, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(16, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case AnimalSpecies::SHEEP:
            {
                // FORM THE ANIMATION FOR A SHEEP.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 0 : 16;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(64, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(80, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
        }

        return animal_animation;
    }
}
