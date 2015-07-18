#include <iostream>
#include <stdexcept>
#include "Objects/Axe.h"

namespace OBJECTS
{
    const MATH::Vector2f AxeSwingParameters::NO_FLIP = MATH::Vector2f(1.0f, 1.0f);
    const MATH::Vector2f AxeSwingParameters::FLIP_HORIZONTALLY = MATH::Vector2f(-1.0f, 1.0f);
    const MATH::Vector2f AxeSwingParameters::FLIP_VERTICALLY = MATH::Vector2f(1.0f, -1.0f);
    const MATH::Vector2f AxeSwingParameters::FLIP_HORIZONTALLY_AND_VERTICALLY = MATH::Vector2f(-1.0f, -1.0f);

    bool AxeSwingParameters::SwingOutIsPositiveRotation() const
    {
        float swing_out_rotation_in_degrees = (FullySwungOutRotationAngleInDegrees - InitialRotationAngleInDegrees);
        bool swing_out_is_positive_rotation = (swing_out_rotation_in_degrees > 0.0f);
        return swing_out_is_positive_rotation;
    }
    
    bool AxeSwingParameters::SwingBackIsPositiveRotation() const
    {
        float swing_back_rotation_in_degrees = (FinalRotationAngleInDegrees - FullySwungOutRotationAngleInDegrees);
        bool swing_back_is_positive_rotation = (swing_back_rotation_in_degrees > 0.0f);
        return swing_back_is_positive_rotation;
    }

    std::shared_ptr<Axe> Axe::Create(RESOURCES::Assets& assets)
    {
        // GET RESOURCES FOR THE AXE.
        std::shared_ptr<GRAPHICS::Texture> axe_texture = assets.GetTexture(RESOURCES::AXE_TEXTURE_ID);
        bool axe_resources_loaded = (nullptr != axe_texture);
        if (!axe_resources_loaded)
        {
            // The axe cannot be created without its resources.
            return nullptr;
        }

        // CREATE THE AXE SPRITE.
        //const float AXE_SPRITE_X_OFFSET_IN_PIXELS = 3.0f;
        //const float AXE_SPRITE_Y_OFFSET_IN_PIXELS = 66.0f;
        //const float AXE_WIDTH_IN_PIXELS = 9.0f;
        //const float AXE_HEIGHT_IN_PIXELS = 11.0f;
        const float AXE_SPRITE_X_OFFSET_IN_PIXELS = 52.0f;
        const float AXE_SPRITE_Y_OFFSET_IN_PIXELS = 0.0f;
        const float AXE_WIDTH_IN_PIXELS = 11.0f;
        const float AXE_HEIGHT_IN_PIXELS = 14.0f;
        MATH::FloatRectangle texture_sub_rectangle = MATH::FloatRectangle::FromTopLeftAndDimensions(
            AXE_SPRITE_X_OFFSET_IN_PIXELS,
            AXE_SPRITE_Y_OFFSET_IN_PIXELS,
            AXE_WIDTH_IN_PIXELS,
            AXE_HEIGHT_IN_PIXELS);
        std::shared_ptr<GRAPHICS::Sprite> sprite = std::make_shared<GRAPHICS::Sprite>(
            *axe_texture,
            texture_sub_rectangle);

        // CREATE THE AXE.
        std::shared_ptr<Axe> axe = std::make_shared<Axe>(sprite);
        return axe;
    }

    Axe::Axe(const std::shared_ptr<GRAPHICS::Sprite>& sprite) :
    Sprite(sprite),
    SwingingDirection(CORE::Direction::INVALID),
    CurrentlySwinging(false),
    SwingingOut(false),
    SwingingBack(false),
    CurrentRotationAngleInDegrees(0.0f),
    CurrentSwingParameters()
    {
        // MAKE SURE A SPRITE WAS PROVIDED.
        bool sprite_exists = (nullptr != Sprite);
        if (!sprite_exists)
        {
            // REPORT THE UNEXPECTED ERROR.
            throw std::invalid_argument("Null sprite cannot be provided to axe constructor.");
        }
    }

    void Axe::SwingUp()
    {
        // STORE THE DIRECTION OF THE SWING.
        SwingingDirection = CORE::Direction::UP;

        // DEFINE THE PARAMETERS FOR SWINGING THE AXE UP.
        // The origin should be at the bottom of the axe handle
        // so that the axe appears to rotate about this point.
        const float LEFT_BOUNDARY_IN_PIXELS = 0.0f;
        float axe_sprite_height_in_pixels = Sprite->GetHeightInPixels();
        CurrentSwingParameters.SpriteOriginInPixels = MATH::Vector2f(
            LEFT_BOUNDARY_IN_PIXELS,
            axe_sprite_height_in_pixels);

        // No flipping is needed for swinging up.
        CurrentSwingParameters.SpriteScale = AxeSwingParameters::NO_FLIP;

        // The axe will swing from its initial position left
        // by 90 degrees and then swing back.
        const float INITIAL_AXE_ROTATION_IN_DEGREES = 0.0f;
        const float FULLY_SWUNG_OUT_AXE_ROTATION_IN_DEGREES = -90.0f;
        CurrentSwingParameters.InitialRotationAngleInDegrees = INITIAL_AXE_ROTATION_IN_DEGREES;
        CurrentSwingParameters.FullySwungOutRotationAngleInDegrees = FULLY_SWUNG_OUT_AXE_ROTATION_IN_DEGREES;
        CurrentSwingParameters.FinalRotationAngleInDegrees = INITIAL_AXE_ROTATION_IN_DEGREES;

        // START THE SWING.
        CurrentlySwinging = true;
        SwingingOut = true;
        CurrentRotationAngleInDegrees = CurrentSwingParameters.InitialRotationAngleInDegrees;

        // MAKE THE AXE VISIBLE.
        Sprite->SetVisible(true);

        // SET THE SPRITE'S TRANSFORMATIONS.
        Sprite->SetOrigin(CurrentSwingParameters.SpriteOriginInPixels);
        Sprite->SetRotation(CurrentRotationAngleInDegrees);
        Sprite->SetScale(CurrentSwingParameters.SpriteScale);
    }
    
    void Axe::SwingDown()
    {
        // STORE THE DIRECTION OF THE SWING.
        SwingingDirection = CORE::Direction::DOWN;

        // DEFINE THE PARAMETERS FOR SWINGING THE AXE DOWN.
        // The origin should be at the bottom of the axe handle
        // so that the axe appears to rotate about this point.
        const float TOP_BOUNDARY_IN_PIXELS = 0.0f;
        float axe_sprite_width_in_pixels = Sprite->GetWidthInPixels();
        CurrentSwingParameters.SpriteOriginInPixels = MATH::Vector2f(
            0.0f, /// @todo - Document
            Sprite->GetHeightInPixels());

        // The axe sprite needs to be flipped both horizontally and
        // vertically to have it facing down for the swing.
        CurrentSwingParameters.SpriteScale = AxeSwingParameters::FLIP_HORIZONTALLY_AND_VERTICALLY;

        // The axe will swing from its initial position right
        // by 90 degrees and then swing back.
        const float INITIAL_AXE_ROTATION_IN_DEGREES = 0.0f;
        const float FULLY_SWUNG_OUT_AXE_ROTATION_IN_DEGREES = -90.0f;
        CurrentSwingParameters.InitialRotationAngleInDegrees = INITIAL_AXE_ROTATION_IN_DEGREES;
        CurrentSwingParameters.FullySwungOutRotationAngleInDegrees = FULLY_SWUNG_OUT_AXE_ROTATION_IN_DEGREES;
        CurrentSwingParameters.FinalRotationAngleInDegrees = INITIAL_AXE_ROTATION_IN_DEGREES;

        // START THE SWING.
        CurrentlySwinging = true;
        SwingingOut = true;
        CurrentRotationAngleInDegrees = CurrentSwingParameters.InitialRotationAngleInDegrees;

        // MAKE THE AXE VISIBLE.
        Sprite->SetVisible(true);

        // SET THE SPRITE'S TRANSFORMATIONS.
        Sprite->SetOrigin(CurrentSwingParameters.SpriteOriginInPixels);
        Sprite->SetRotation(CurrentRotationAngleInDegrees);
        Sprite->SetScale(CurrentSwingParameters.SpriteScale);
    }
    
    void Axe::SwingLeft()
    {
        // STORE THE DIRECTION OF THE SWING.
        SwingingDirection = CORE::Direction::LEFT;

        // DEFINE THE PARAMETERS FOR SWINGING THE AXE LEFT.
        // The origin should be at the bottom of the axe handle
        // so that the axe appears to rotate about this point.
        const float LEFT_BOUNDARY_IN_PIXELS = 0.0f;
        float axe_sprite_height_in_pixels = Sprite->GetHeightInPixels();
        CurrentSwingParameters.SpriteOriginInPixels = MATH::Vector2f(
            LEFT_BOUNDARY_IN_PIXELS,
            axe_sprite_height_in_pixels);

        // No flipping is needed for swinging up.
        CurrentSwingParameters.SpriteScale = AxeSwingParameters::NO_FLIP;

        // The axe will swing from its initial position left
        // by 90 degrees and then swing back.
        const float INITIAL_AXE_ROTATION_IN_DEGREES = 0.0f;
        const float FULLY_SWUNG_OUT_AXE_ROTATION_IN_DEGREES = -90.0f;
        CurrentSwingParameters.InitialRotationAngleInDegrees = INITIAL_AXE_ROTATION_IN_DEGREES;
        CurrentSwingParameters.FullySwungOutRotationAngleInDegrees = FULLY_SWUNG_OUT_AXE_ROTATION_IN_DEGREES;
        CurrentSwingParameters.FinalRotationAngleInDegrees = INITIAL_AXE_ROTATION_IN_DEGREES;

        // START THE SWING.
        CurrentlySwinging = true;
        SwingingOut = true;
        CurrentRotationAngleInDegrees = CurrentSwingParameters.InitialRotationAngleInDegrees;

        // MAKE THE AXE VISIBLE.
        Sprite->SetVisible(true);

        // SET THE SPRITE'S TRANSFORMATIONS.
        Sprite->SetOrigin(CurrentSwingParameters.SpriteOriginInPixels);
        Sprite->SetRotation(CurrentRotationAngleInDegrees);
        Sprite->SetScale(CurrentSwingParameters.SpriteScale);
    }

    void Axe::SwingRight()
    {
        // STORE THE DIRECTION OF THE SWING.
        SwingingDirection = CORE::Direction::RIGHT;

        // DEFINE THE PARAMETERS FOR SWINGING THE AXE RIGHT.
        // The origin should be at the bottom of the axe handle
        // so that the axe appears to rotate about this point.
        float axe_sprite_width_in_pixels = Sprite->GetWidthInPixels();
        float axe_sprite_height_in_pixels = Sprite->GetHeightInPixels();
        CurrentSwingParameters.SpriteOriginInPixels = MATH::Vector2f(
            0.0f,//axe_sprite_width_in_pixels,
            axe_sprite_height_in_pixels);

        // Flip the axe so that it is facing right.
        CurrentSwingParameters.SpriteScale = AxeSwingParameters::FLIP_HORIZONTALLY;

        // The axe will swing from its initial position right
        // by 90 degrees and then swing back.
        const float INITIAL_AXE_ROTATION_IN_DEGREES = 0.0f;
        const float FULLY_SWUNG_OUT_AXE_ROTATION_IN_DEGREES = 90.0f;
        CurrentSwingParameters.InitialRotationAngleInDegrees = INITIAL_AXE_ROTATION_IN_DEGREES;
        CurrentSwingParameters.FullySwungOutRotationAngleInDegrees = FULLY_SWUNG_OUT_AXE_ROTATION_IN_DEGREES;
        CurrentSwingParameters.FinalRotationAngleInDegrees = INITIAL_AXE_ROTATION_IN_DEGREES;

        // START THE SWING.
        CurrentlySwinging = true;
        SwingingOut = true;
        CurrentRotationAngleInDegrees = CurrentSwingParameters.InitialRotationAngleInDegrees;

        // MAKE THE AXE VISIBLE.
        Sprite->SetVisible(true);

        // SET THE SPRITE'S TRANSFORMATIONS.
        Sprite->SetOrigin(CurrentSwingParameters.SpriteOriginInPixels);
        Sprite->SetRotation(CurrentRotationAngleInDegrees);
        Sprite->SetScale(CurrentSwingParameters.SpriteScale);
    }

    bool Axe::IsSwinging() const
    {
        return CurrentlySwinging;
    }

    bool Axe::FullySwungOut() const
    {
        if (!CurrentlySwinging)
        {
            return false;
        }

        bool swinging_out_is_positive_rotation = CurrentSwingParameters.SwingOutIsPositiveRotation();
        if (swinging_out_is_positive_rotation)
        {
            bool fully_swung_out = (CurrentRotationAngleInDegrees >= CurrentSwingParameters.FullySwungOutRotationAngleInDegrees);
            return fully_swung_out;
        }
        else
        {
            bool fully_swung_out = (CurrentRotationAngleInDegrees <= CurrentSwingParameters.FullySwungOutRotationAngleInDegrees);
            return fully_swung_out;
        }
    }

    void Axe::Update(const float elapsed_time_in_seconds)
    {
        // CHECK IF THE AXE IS SWINGING.
        bool is_swinging = IsSwinging();
        if (is_swinging)
        {
            // UPDATE THE SWING BASED ON THE ELAPSED TIME.
            const float ROTATION_SPEED_IN_DEGREES_PER_SECOND = 90.0f;
            float rotation_amount_for_update_in_degrees = ROTATION_SPEED_IN_DEGREES_PER_SECOND * elapsed_time_in_seconds;
            if (SwingingOut)
            {
                bool swinging_out_is_positive_rotation = CurrentSwingParameters.SwingOutIsPositiveRotation();
                if (swinging_out_is_positive_rotation)
                {
                    CurrentRotationAngleInDegrees += rotation_amount_for_update_in_degrees;

                    bool fully_swung_out = (CurrentRotationAngleInDegrees >= CurrentSwingParameters.FullySwungOutRotationAngleInDegrees);
                    if (fully_swung_out)
                    {
                        /// @todo   Clamp to the max rotation angle?
                        SwingingOut = false;
                        SwingingBack = true;
                    }
                }
                else
                {
                    CurrentRotationAngleInDegrees -= rotation_amount_for_update_in_degrees;

                    bool fully_swung_out = (CurrentRotationAngleInDegrees <= CurrentSwingParameters.FullySwungOutRotationAngleInDegrees);
                    if (fully_swung_out)
                    {
                        /// @todo   Clamp to the max rotation angle?
                        SwingingOut = false;
                        SwingingBack = true;
                    }
                }
            }
            else if (SwingingBack)
            {
                bool swinging_back_is_positive_rotation = CurrentSwingParameters.SwingBackIsPositiveRotation();
                if (swinging_back_is_positive_rotation)
                {
                    CurrentRotationAngleInDegrees += rotation_amount_for_update_in_degrees;

                    bool fully_swung_back = (CurrentRotationAngleInDegrees >= CurrentSwingParameters.FinalRotationAngleInDegrees);
                    if (fully_swung_back)
                    {
                        /// @todo   Clamp to the max rotation angle?
                        SwingingBack = false;
                        CurrentlySwinging = false;
                    }
                }
                else
                {
                    CurrentRotationAngleInDegrees -= rotation_amount_for_update_in_degrees;

                    bool fully_swung_back = (CurrentRotationAngleInDegrees <= CurrentSwingParameters.FinalRotationAngleInDegrees);
                    if (fully_swung_back)
                    {
                        /// @todo   Clamp to the max rotation angle?
                        SwingingBack = false;
                        CurrentlySwinging = false;
                    }
                }
            }
            
            Sprite->SetRotation(CurrentRotationAngleInDegrees);
        }
        else
        {
            // HIDE THE AXE TO PREVENT IT FROM BEING DRAWN.
            Sprite->SetVisible(false);
        }
    }

    MATH::FloatRectangle Axe::GetBladeBounds() const
    {
        // CHECK IF THE AXE IS BEING SWUNG.
        bool axe_is_swinging = IsSwinging();
        if (!axe_is_swinging)
        {
            /// @todo   Rethink returning an invalid rectangle here.
            return MATH::FloatRectangle();
        }

        /// @todo   Think of more advanced collision.
        /// Blade is 7x7.
        return Sprite->GetBoundingBox();
    }

    void Axe::SetWorldPosition(const MATH::Vector2f& world_position)
    {
        Sprite->SetWorldPosition(world_position.X, world_position.Y);
    }

    std::shared_ptr<GRAPHICS::Sprite> Axe::GetSprite() const
    {
        return Sprite;
    }
}
