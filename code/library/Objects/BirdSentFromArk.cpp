#include "ErrorHandling/Asserts.h"
#include "Objects/BirdSentFromArk.h"
#include "Resources/AnimalGraphics.h"
#include "Resources/AnimalSounds.h"

namespace OBJECTS
{
    /// Constructor.
    /// @param[in]  action_that_sent_out_bird - The action that sent the bird out.
    /// @param[in]  species - The species for the bird (should be raven or dove).
    BirdSentFromArk::BirdSentFromArk(const GAMEPLAY::FloodSpecialDayAction action_that_sent_out_bird, const AnimalSpecies::Value species) :
        MovingOut(true),
        ActionThatSentOutBird(action_that_sent_out_bird),
        Species(species),
        Sprite(),
        SoundId(),
        MoveDirection()
    {
        // GET THE PROPER SPRITE FOR THE ANIMAL.
        // A gender is arbitrarily chosen.
        OBJECTS::AnimalType animal_type(Species, AnimalGender::MALE);
        std::shared_ptr<GRAPHICS::AnimatedSprite> sprite = RESOURCES::AnimalGraphics::GetSprite(animal_type);
        ASSERT_THEN_IF(sprite)
        {
            Sprite = *sprite;
        }

        // GET THE PROPER SOUND FOR THE ANIMAL.
        SoundId = RESOURCES::AnimalSounds::GetSound(Species);

        // INITIALIZE THE MOVE DIRECTION.
        // Birds when first sent out are somewhat arbitrarily sent out to the top-left corner.
        MoveDirection = MATH::Vector2f::Normalize(MATH::Vector2f(-1.0f, -0.5f));
    }

    /// Updates the bird based on the elapsed time.
    /// @param[in]  elapsed_time - The time for which to update the bird.
    void BirdSentFromArk::Update(const sf::Time& elapsed_time)
    {
        // UPDATE THE SPRITE ANIMATION.
        Sprite.Update(elapsed_time);

        // MOVE THE BIRD BASED ON ELAPSED TIME.
        float elapsed_time_in_seconds = elapsed_time.asSeconds();
        float bird_move_speed_in_pixels_per_second = OBJECTS::AnimalType::GetMoveSpeedInPixelsPerSecond(Species);
        float movement_distance = elapsed_time_in_seconds * bird_move_speed_in_pixels_per_second;
        MATH::Vector2f movement = MATH::Vector2f::Scale(movement_distance, MoveDirection);
        Sprite.CurrentFrameSprite.WorldPosition += movement;
    }
}
