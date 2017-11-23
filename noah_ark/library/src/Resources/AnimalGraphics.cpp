#include <cassert>
#include "Resources/AnimalGraphics.h"

namespace RESOURCES
{
    /// Gets a sprite for the specified animal type.
    /// @param[in]  animal_type - The type of animal to get a sprite for.
    /// @param[in,out]  assets - The assets from which to get the sprite.
    /// @return The animal sprite, if successfully retrieved; null otherwise.
    std::shared_ptr<GRAPHICS::AnimatedSprite> AnimalGraphics::GetSprite(
        const OBJECTS::AnimalType animal_type,
        Assets& assets)
    {
        // GET THE ANIMAL TEXTURE.
        std::shared_ptr<GRAPHICS::Texture> animal_texture = assets.GetTexture(RESOURCES::ANIMAL_TEXTURE_ID);
        if (!animal_texture)
        {
            return nullptr;
        }

        // GET THE ANIMAL'S ANIMATION SEQUENCE.
        std::shared_ptr<GRAPHICS::AnimationSequence> animation_sequence = GetAnimalAnimation(animal_type);
        if (!animation_sequence)
        {
            return nullptr;
        }

        // CREATE THE ANIMAL'S SPRITE.
        auto animal_sprite = std::make_shared<GRAPHICS::AnimatedSprite>(animal_texture, animation_sequence);
        return animal_sprite;
    }

    /// Attempts to get the animation sequence for a given type of animal.
    /// @param[in]  animal_type - The type of animal whose animation sequence to get.
    /// @return The animal's animation sequence, if successfully retrieved; null otherwise.
    std::shared_ptr<GRAPHICS::AnimationSequence> AnimalGraphics::GetAnimalAnimation(const OBJECTS::AnimalType& animal_type)
    {
        // DETERMINE THE GENDER OF THE ANIMAL.
        bool is_male = (OBJECTS::AnimalGender::MALE == animal_type.Gender);
        bool is_female = (OBJECTS::AnimalGender::FEMALE == animal_type.Gender);
        assert(is_male || is_female);

        // GET THE ANIMATION SEQUENCE FOR THE APPROPRIATE SPECIES.
        //const std::vector<MATH::IntRectangle>& frames;
        //const std::string OBJECTS::Animal::ANIMATION_NAME;
        const bool LOOPING = true;
        const sf::Time ANIMATION_DURATION = sf::seconds(1.0f);
        std::shared_ptr<GRAPHICS::AnimationSequence> animal_animation;
        switch (animal_type.Species)
        {
            case OBJECTS::AnimalSpecies::BAT:
            {
                // FORM THE ANIMATION FOR A BAT.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 32 : 48;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(32, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(48, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::CAMEL:
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
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::CATTLE:
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
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::DEER:
            {
                // FORM THE ANIMATION FOR A DEER.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 32 : 48;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(64, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(80, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::DOVE:
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
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::EAGLE:
            {
                // FORM THE ANIMATION FOR EAGLE.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 32 : 48;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(96, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(112, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::FROG:
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
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::GOAT:
            {
                // FORM THE ANIMATION FOR A GOAT.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 0 : 16;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(224, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(240, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::HAWK:
            {
                // FORM THE ANIMATION FOR A HAWK.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 32 : 48;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(128, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(144, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::LOCUST:
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
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::OSPREY:
            {
                // FORM THE ANIMATION FOR A OSPREY.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 32 : 48;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(160, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(176, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::OWL:
            {
                // FORM THE ANIMATION FOR A OWL.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 32 : 48;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(192, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(208, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::PELICAN:
            {
                // FORM THE ANIMATION FOR A PELICAN.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 32 : 48;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(224, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(240, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::PIG:
            {
                // FORM THE ANIMATION FOR A PIG.
                // The frames differ based on gender.
                int top_texture_offset_in_pixels = is_male ? 32 : 48;
                const std::vector<MATH::IntRectangle> FRAMES =
                {
                    // Frames are duplicated some so that things appear to loop a few times.
                    MATH::IntRectangle::FromLeftTopAndDimensions(0, top_texture_offset_in_pixels, 16, 16),
                    MATH::IntRectangle::FromLeftTopAndDimensions(16, top_texture_offset_in_pixels, 16, 16),
                };

                animal_animation = std::make_shared<GRAPHICS::AnimationSequence>(
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::RAVEN:
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
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
            case OBJECTS::AnimalSpecies::SHEEP:
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
                    OBJECTS::Animal::ANIMATION_NAME,
                    LOOPING,
                    ANIMATION_DURATION,
                    FRAMES);
                break;
            }
        }

        return animal_animation;
    }
}
