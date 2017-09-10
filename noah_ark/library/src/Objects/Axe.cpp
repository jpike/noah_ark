#include "Core/NullChecking.h"
#include "Objects/Axe.h"

namespace OBJECTS
{
    const MATH::Vector2f AxeSwingParameters::NO_FLIP = MATH::Vector2f(1.0f, 1.0f);
    const MATH::Vector2f AxeSwingParameters::FLIP_HORIZONTALLY = MATH::Vector2f(-1.0f, 1.0f);
    const MATH::Vector2f AxeSwingParameters::FLIP_VERTICALLY = MATH::Vector2f(1.0f, -1.0f);
    const MATH::Vector2f AxeSwingParameters::FLIP_HORIZONTALLY_AND_VERTICALLY = MATH::Vector2f(-1.0f, -1.0f);

    /// Determines if the rotation performed by swinging out is
    /// positive or not.
    /// @return True if the swing out rotation is positive; false otherwise.
    bool AxeSwingParameters::SwingOutIsPositiveRotation() const
    {
        // If the swinging out rotation angle is greater than the initial rotation angle, then a positive rotation occurs.
        float swing_out_rotation_in_degrees = (FullySwungOutRotationAngleInDegrees - InitialRotationAngleInDegrees);
        bool swing_out_is_positive_rotation = (swing_out_rotation_in_degrees > 0.0f);
        return swing_out_is_positive_rotation;
    }
    
    /// Determines if the rotation performed by swinging back is
    /// positive or not.
    /// @return True if the swing back rotation is positive; false otherwise.
    bool AxeSwingParameters::SwingBackIsPositiveRotation() const
    {
        // If the final rotation angle is greater than the fully swung out angle, then a positive rotation occurs.
        float swing_back_rotation_in_degrees = (FinalRotationAngleInDegrees - FullySwungOutRotationAngleInDegrees);
        bool swing_back_is_positive_rotation = (swing_back_rotation_in_degrees > 0.0f);
        return swing_back_is_positive_rotation;
    }

    /// Constructor.
    /// @param[in]  texture - The texture for the axe.
    /// @throws std::exception - Thrown if a parameter is null.
    Axe::Axe(const std::shared_ptr<GRAPHICS::Texture>& texture) :
    Sprite()
    {
        // MAKE SURE REQUIRED PARAMETERS WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(texture, "Texture required for axe.");

        // INITIALIZE THE SPRITE.
        const float AXE_SPRITE_X_OFFSET_IN_PIXELS = 52.0f;
        const float AXE_SPRITE_Y_OFFSET_IN_PIXELS = 0.0f;
        const float AXE_WIDTH_IN_PIXELS = 11.0f;
        const float AXE_HEIGHT_IN_PIXELS = 14.0f;
        MATH::FloatRectangle axe_texture_sub_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            AXE_SPRITE_X_OFFSET_IN_PIXELS,
            AXE_SPRITE_Y_OFFSET_IN_PIXELS,
            AXE_WIDTH_IN_PIXELS,
            AXE_HEIGHT_IN_PIXELS);
        Sprite = GRAPHICS::Sprite(texture, axe_texture_sub_rectangle);
    }

    /// Swings the axe up.
    void Axe::SwingUp()
    {
        // STORE THE DIRECTION OF THE SWING.
        SwingingDirection = CORE::Direction::UP;

        // DEFINE THE PARAMETERS FOR SWINGING THE AXE UP.
        // The origin should be at the bottom of the axe handle
        // so that the axe appears to rotate about this point.
        const float LEFT_BOUNDARY_IN_PIXELS = 0.0f;
        float axe_sprite_height_in_pixels = Sprite.GetHeightInPixels();
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
        Sprite.IsVisible = true;

        // SET THE SPRITE'S TRANSFORMATIONS.
        Sprite.SetOrigin(CurrentSwingParameters.SpriteOriginInPixels);
        Sprite.SetRotation(CurrentRotationAngleInDegrees);
        Sprite.SetScale(CurrentSwingParameters.SpriteScale);
    }
    
    /// Swings the axe down.
    void Axe::SwingDown()
    {
        // STORE THE DIRECTION OF THE SWING.
        SwingingDirection = CORE::Direction::DOWN;

        // DEFINE THE PARAMETERS FOR SWINGING THE AXE DOWN.
        // The origin should be at the bottom of the axe handle
        // so that the axe appears to rotate about this point.
        const float LEFT_BOUNDARY_IN_PIXELS = 0.0f;
        float axe_sprite_height_in_pixels = Sprite.GetHeightInPixels();
        CurrentSwingParameters.SpriteOriginInPixels = MATH::Vector2f(
            LEFT_BOUNDARY_IN_PIXELS,
            axe_sprite_height_in_pixels);

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
        Sprite.IsVisible = true;

        // SET THE SPRITE'S TRANSFORMATIONS.
        Sprite.SetOrigin(CurrentSwingParameters.SpriteOriginInPixels);
        Sprite.SetRotation(CurrentRotationAngleInDegrees);
        Sprite.SetScale(CurrentSwingParameters.SpriteScale);
    }
    
    /// Swings the axe left.
    void Axe::SwingLeft()
    {
        // STORE THE DIRECTION OF THE SWING.
        SwingingDirection = CORE::Direction::LEFT;

        // DEFINE THE PARAMETERS FOR SWINGING THE AXE LEFT.
        // The origin should be at the bottom of the axe handle
        // so that the axe appears to rotate about this point.
        const float LEFT_BOUNDARY_IN_PIXELS = 0.0f;
        float axe_sprite_height_in_pixels = Sprite.GetHeightInPixels();
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
        Sprite.IsVisible = true;

        // SET THE SPRITE'S TRANSFORMATIONS.
        Sprite.SetOrigin(CurrentSwingParameters.SpriteOriginInPixels);
        Sprite.SetRotation(CurrentRotationAngleInDegrees);
        Sprite.SetScale(CurrentSwingParameters.SpriteScale);
    }

    /// Swings the axe up.
    void Axe::SwingRight()
    {
        // STORE THE DIRECTION OF THE SWING.
        SwingingDirection = CORE::Direction::RIGHT;

        // DEFINE THE PARAMETERS FOR SWINGING THE AXE RIGHT.
        // The origin should be at the bottom of the axe handle
        // so that the axe appears to rotate about this point.
        const float LEFT_BOUNDARY_IN_PIXELS = 0.0f;
        float axe_sprite_height_in_pixels = Sprite.GetHeightInPixels();
        CurrentSwingParameters.SpriteOriginInPixels = MATH::Vector2f(
            LEFT_BOUNDARY_IN_PIXELS,
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
        Sprite.IsVisible = true;

        // SET THE SPRITE'S TRANSFORMATIONS.
        Sprite.SetOrigin(CurrentSwingParameters.SpriteOriginInPixels);
        Sprite.SetRotation(CurrentRotationAngleInDegrees);
        Sprite.SetScale(CurrentSwingParameters.SpriteScale);
    }

    /// Determines if the axe is currently being swung.
    /// @return True if the axe is being swung; false otherwise.
    bool Axe::IsSwinging() const
    {
        return CurrentlySwinging;
    }

    /// Determines if the axe is fully swung out.
    /// @return True if the axe is fully swung out; false otherwise.
    bool Axe::FullySwungOut() const
    {
        // The axe isn't fully swung out if it isn't being swung at all.
        if (!CurrentlySwinging)
        {
            return false;
        }

        // DETERMINE THE SWINGING DIRECTION OF THE AXE.
        bool swinging_out_is_positive_rotation = CurrentSwingParameters.SwingOutIsPositiveRotation();
        if (swinging_out_is_positive_rotation)
        {
            // The axe is fully swung out if it's rotation angle is at least its fully swung out angle.
            bool fully_swung_out = (CurrentRotationAngleInDegrees >= CurrentSwingParameters.FullySwungOutRotationAngleInDegrees);
            return fully_swung_out;
        }
        else
        {
            // The axe is fully swung out if it's rotation angle has at least reached its fully swung out angle.
            bool fully_swung_out = (CurrentRotationAngleInDegrees <= CurrentSwingParameters.FullySwungOutRotationAngleInDegrees);
            return fully_swung_out;
        }
    }

    /// Updates the axe for a frame based on the elapsed amount of time.
    /// @param[in]  elapsed_time - The amount of time to update by.
    void Axe::Update(const sf::Time& elapsed_time)
    {
        // CHECK IF THE AXE IS SWINGING.
        bool is_swinging = IsSwinging();
        if (is_swinging)
        {
            // UPDATE THE SWING BASED ON THE ELAPSED TIME.
            const float ROTATION_SPEED_IN_DEGREES_PER_SECOND = 90.0f;
            float elapsed_time_in_seconds = elapsed_time.asSeconds();
            float rotation_amount_for_update_in_degrees = ROTATION_SPEED_IN_DEGREES_PER_SECOND * elapsed_time_in_seconds;
            if (SwingingOut)
            {
                // DETERMINE THE DIRECTION OF THE AXE SWING.
                bool swinging_out_is_positive_rotation = CurrentSwingParameters.SwingOutIsPositiveRotation();
                if (swinging_out_is_positive_rotation)
                {
                    // CONTINUE SWINGING OF THE AXE.
                    // For a positive rotation, the rotation angle needs to be increased.
                    CurrentRotationAngleInDegrees += rotation_amount_for_update_in_degrees;

                    // STOP SWINGING THE AXE OUT IF IT HAS REACHED ITS MAX ROTATION ANGLE.
                    bool fully_swung_out = (CurrentRotationAngleInDegrees >= CurrentSwingParameters.FullySwungOutRotationAngleInDegrees);
                    if (fully_swung_out)
                    {
                        // START SWINGING THE AXE BACK TO ITS ORIGINAL POSITION.
                        SwingingOut = false;
                        SwingingBack = true;
                    }
                }
                else
                {
                    // CONTINUE SWINGING OF THE AXE.
                    // For a negative rotation, the rotation angle needs to be decreased.
                    CurrentRotationAngleInDegrees -= rotation_amount_for_update_in_degrees;

                    // STOP SWINGING THE AXE OUT IF IT HAS REACHED ITS MAX ROTATION ANGLE.
                    bool fully_swung_out = (CurrentRotationAngleInDegrees <= CurrentSwingParameters.FullySwungOutRotationAngleInDegrees);
                    if (fully_swung_out)
                    {
                        // START SWINGING THE AXE BACK TO ITS ORIGINAL POSITION.
                        SwingingOut = false;
                        SwingingBack = true;
                    }
                }
            }
            else if (SwingingBack)
            {
                // DETERMINE THE DIRECTION OF THE AXE SWING.
                bool swinging_back_is_positive_rotation = CurrentSwingParameters.SwingBackIsPositiveRotation();
                if (swinging_back_is_positive_rotation)
                {
                    // CONTINUE SWINGING OF THE AXE.
                    // For a positive rotation, the rotation angle needs to be increased.
                    CurrentRotationAngleInDegrees += rotation_amount_for_update_in_degrees;

                    // STOP SWINGING THE AXE BACK IF IT HAS REACHED ITS MAX ROTATION ANGLE.
                    bool fully_swung_back = (CurrentRotationAngleInDegrees >= CurrentSwingParameters.FinalRotationAngleInDegrees);
                    if (fully_swung_back)
                    {
                        // STOP THE AXE FROM SWINGING SINCE THE SWING HAS COMPLETED.
                        SwingingBack = false;
                        CurrentlySwinging = false;
                    }
                }
                else
                {
                    // CONTINUE SWINGING OF THE AXE.
                    // For a negative rotation, the rotation angle needs to be decreased.
                    CurrentRotationAngleInDegrees -= rotation_amount_for_update_in_degrees;

                    // STOP SWINGING THE AXE BACK IF IT HAS REACHED ITS MAX ROTATION ANGLE.
                    bool fully_swung_back = (CurrentRotationAngleInDegrees <= CurrentSwingParameters.FinalRotationAngleInDegrees);
                    if (fully_swung_back)
                    {
                        // STOP SWINGING THE AXE BACK IF IT HAS REACHED ITS MAX ROTATION ANGLE.
                        SwingingBack = false;
                        CurrentlySwinging = false;
                    }
                }
            }
            
            // UPDATE THE ROTATION OF THE AXE SPRITE>
            Sprite.SetRotation(CurrentRotationAngleInDegrees);
        }
        else
        {
            // HIDE THE AXE TO PREVENT IT FROM BEING DRAWN SINCE IT ISN'T BEING SWUNG.
            Sprite.IsVisible = false;
        }
    }

    /// Gets the world boundaries of the axe's blade.
    /// @return The world boundaries of the axe's blade.
    MATH::FloatRectangle Axe::GetBladeBounds() const
    {
        // CHECK IF THE AXE IS BEING SWUNG.
        bool axe_is_swinging = IsSwinging();
        if (!axe_is_swinging)
        {
            return MATH::FloatRectangle();
        }

        return Sprite.GetWorldBoundingBox();
    }

    /// Sets the world position of the axe.
    /// @param[in]  world_position - The world position to set.
    void Axe::SetWorldPosition(const MATH::Vector2f& world_position)
    {
        Sprite.SetWorldPosition(world_position.X, world_position.Y);
    }
}
