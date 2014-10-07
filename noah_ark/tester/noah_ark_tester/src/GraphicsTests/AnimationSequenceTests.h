#pragma once

#include "Graphics/AnimationSequence.h"

/// A namespace for testing the AnimationSequence class.
namespace TEST_ANIMATION_SEQUENCE
{
    TEST_CASE( "Constructor.  Looping.", "[AnimationSequence][Constructor][Looping]" )
    {
        // CONSTRUCT AN ANIMATION SEQUENCE.
        // The values are arbitrary except for the looping status.
        const std::string TEST_ANIMATION_SEQUENCE_NAME = "TestAnimationSequence";
        const bool IS_LOOPING = true;
        const int TEST_DURATION_IN_MICROSECONDS = 24567;
        const sf::Time TEST_DURATION = sf::microseconds(TEST_DURATION_IN_MICROSECONDS);
        const thor::FrameAnimation TEST_FRAMES;

        GRAPHICS::AnimationSequence animationSequence(
            TEST_ANIMATION_SEQUENCE_NAME,
            IS_LOOPING,
            TEST_DURATION,
            TEST_FRAMES);

        // VERIFY THAT THE FIELDS ARE PROPERLY SET.
        // Verify the animation name.
        REQUIRE( TEST_ANIMATION_SEQUENCE_NAME == animationSequence.GetName() );

        // Verify the looping status.
        REQUIRE( IS_LOOPING == animationSequence.IsLooping() );

        // Verify the duration.
        REQUIRE( TEST_DURATION == animationSequence.GetDuration() );

        /// @todo   The frames themselves aren't verified because there is not a way yet to
        ///         get access to the internally frame data.  If we move to a custom implementation
        ///         that doesn't use Thor, then verification should be done here.
    }

    TEST_CASE( "Constructor.  Not looping.", "[AnimationSequence][Constructor][NotLooping]" )
    {
        // CONSTRUCT AN ANIMATION SEQUENCE.
        // The values are arbitrary except for the looping status.
        const std::string TEST_ANIMATION_SEQUENCE_NAME = "TestAnimationSequence";
        const bool IS_NOT_LOOPING = false;
        const int TEST_DURATION_IN_MICROSECONDS = 24567;
        const sf::Time TEST_DURATION = sf::microseconds(TEST_DURATION_IN_MICROSECONDS);
        const thor::FrameAnimation TEST_FRAMES;

        GRAPHICS::AnimationSequence animationSequence(
            TEST_ANIMATION_SEQUENCE_NAME,
            IS_NOT_LOOPING,
            TEST_DURATION,
            TEST_FRAMES);

        // VERIFY THAT THE FIELDS ARE PROPERLY SET.
        // Verify the animation name.
        REQUIRE( TEST_ANIMATION_SEQUENCE_NAME == animationSequence.GetName() );

        // Verify the looping status.
        REQUIRE( IS_NOT_LOOPING == animationSequence.IsLooping() );

        // Verify the duration.
        REQUIRE( TEST_DURATION == animationSequence.GetDuration() );

        /// @todo   The frames themselves aren't verified because there is not a way yet to
        ///         get access to the internally frame data.  If we move to a custom implementation
        ///         that doesn't use Thor, then verification should be done here.
    }
}