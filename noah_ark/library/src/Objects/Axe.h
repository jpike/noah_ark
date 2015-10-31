#pragma once

#include "Core/Direction.h"
#include "Graphics/Sprite.h"
#include "Math/Rectangle.h"
#include "Math/Vector2.h"
#include "Resources/Assets.h"

namespace OBJECTS
{
    /// Parameters for describing different swings of an axe.
    struct AxeSwingParameters
    {
        // CONSTANTS.
        /// A pre-defined scale value to indicate that no flipping
        /// is needed for the axe sprite during rendering.
        static const MATH::Vector2f NO_FLIP;
        /// A pre-defined scale value to horizontally flip an axe sprite
        /// across the vertical axis for rendering.
        static const MATH::Vector2f FLIP_HORIZONTALLY;
        /// A pre-defined scale value to vertically flip an axe sprite
        /// across the horizontal axis for rendering.
        static const MATH::Vector2f FLIP_VERTICALLY;
        /// A pre-defined scale value to flip an axe sprite both horizontally
        /// and vertically for rendering.
        static const MATH::Vector2f FLIP_HORIZONTALLY_AND_VERTICALLY;
        
        // MEMBER VARIABLES.
        /// The origin/center in pixels (relative to the top-left corner)
        /// of the sprite to use for rotations for axe swings.
        MATH::Vector2f SpriteOriginInPixels;
        /// Scaling to apply to the sprite.  Allows flipping
        /// the sprite horizontally or vertically.  See
        /// constants defined in this struct for pre-defined values.
        MATH::Vector2f SpriteScale;
        /// The initial rotation angle to set for the sprite
        /// when beginning to swing the axe.  The axe will swing
        /// from this initial rotation angle to the rotation angle
        /// defining when it has fully swung out.
        float InitialRotationAngleInDegrees;
        /// The rotation angle defining when the axe sprite
        /// has fully swung out.  Once an axe sprite has reached
        /// this rotation angle during a swing, it should start
        /// swinging back to the final rotation angle.
        float FullySwungOutRotationAngleInDegrees;
        /// The final rotation angle at which the axe sprite
        /// will swing back to after reaching its fully swung
        /// out rotation angle.  When the axe sprite has reached
        /// this final rotation angle, an axe swing is complete.
        float FinalRotationAngleInDegrees;

        // METHODS.
        /// Determines if the rotation performed by swinging out is
        /// positive or not.
        /// @return True if the swing out rotation is positive; false otherwise.
        bool SwingOutIsPositiveRotation() const;
        /// Determines if the rotation performed by swinging back is
        /// positive or not.
        /// @return True if the swing back rotation is positive; false otherwise.
        bool SwingBackIsPositiveRotation() const;
    };

    /// An axe that can be swung to hit objects in the world.
    /// The axe swinging is visualized via rotation defined by axe swing parameters.
    class Axe
    {
    public:
        // SWINGING METHODS.
        /// Swings the axe up.
        void SwingUp();
        /// Swings the axe down.
        void SwingDown();
        /// Swings the axe left.
        void SwingLeft();
        /// Swings the axe up.
        void SwingRight();

        /// Determines if the axe is currently being swung.
        /// @return True if the axe is being swung; false otherwise.
        bool IsSwinging() const;
        /// Determines if the axe is fully swung out.
        /// @return True if the axe is fully swung out; false otherwise.
        bool FullySwungOut() const;

        // OTHER METHODS.
        /// Updates the axe for a frame based on the elapsed amount of time.
        /// @param[in]  elapsed_time_in_seconds - The amount of time to update by.
        void Update(const float elapsed_time_in_seconds);

        /// Gets the world boundaries of the axe's blade.
        /// @return The world boundaries of the axe's blade.
        MATH::FloatRectangle GetBladeBounds() const;

        /// Sets the world position of the axe.
        /// @param[in]  world_position - The world position to set.
        void SetWorldPosition(const MATH::Vector2f& world_position);

        // PUBLIC MEMBER VARIABLES FOR EASY ACCESS.
        /// The sprite for the axe.
        GRAPHICS::Sprite Sprite = GRAPHICS::Sprite();

    private:
        // MEMBER VARIABLES.
        /// The direction the axe is currently being swung.
        CORE::Direction SwingingDirection = CORE::Direction::INVALID;
        /// True if the axe is currently swinging; false otherwise.
        bool CurrentlySwinging = false;
        /// True if the axe is currently swinging out; false otherwise.
        bool SwingingOut = false;
        /// True if the axe is currently swinging back in from its out position; false otherwise.
        bool SwingingBack = false;
        /// The current rotation angle of the axe in its swing.
        float CurrentRotationAngleInDegrees = 0.0f;

        /// Parameters defining the axe swing currently in-progress,
        /// if a swing is occurring.  Undefined if the axe is not being swung.
        AxeSwingParameters CurrentSwingParameters = AxeSwingParameters();
    };
}
