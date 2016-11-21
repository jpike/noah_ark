#include <algorithm>
#include "Bible/BibleVerses.h"
#include "Core/NullChecking.h"
#include "States/IntroSequence.h"

namespace STATES
{
    const sf::Time IntroSequence::MAX_TIME_PER_FRAME = sf::seconds(10);

    /// Constructor.
    IntroSequence::IntroSequence() :
        IntroBibleVerses(),
        CurrentFrameIndex(0),
        ElapsedTimeForCurrentFrame(sf::Time::Zero)
    {
        // GET THE BIBLE VERSES FOR THE INTRO SEQUENCE.
        const BIBLE::BibleVerse* intro_verse_1 = BIBLE::FindBibleVerse(
            BIBLE::BibleBook::GENESIS,
            BIBLE::ChapterNumber(5),
            BIBLE::VerseNumber(28));
        bool intro_verse_1_exists = (nullptr != intro_verse_1);
        if (intro_verse_1_exists)
        {
            /// @todo   Factor this out so that construction can't fail?
            IntroBibleVerses.push_back(*intro_verse_1);
        }

        const BIBLE::BibleVerse* intro_verse_2 = BIBLE::FindBibleVerse(
            BIBLE::BibleBook::GENESIS,
            BIBLE::ChapterNumber(5),
            BIBLE::VerseNumber(29));
        bool intro_verse_2_exists = (nullptr != intro_verse_2);
        if (intro_verse_2_exists)
        {
            IntroBibleVerses.push_back(*intro_verse_2);
        }
    }

    /// Determines if the intro sequence has completed.
    /// @return True if the intro sequence has completed; false otherwise.
    bool IntroSequence::Completed() const
    {
        // The intro sequence is only completed if the current frame index
        // is greater than the last frame index as the last frame needs
        // to be displayed on screen for some period of time.
        unsigned int last_frame_index = IntroBibleVerses.size() - 1;
        bool intro_sequence_completed = (CurrentFrameIndex > last_frame_index);
        return intro_sequence_completed;
    }

    /// Updates the intro sequence based on the provided amount of time.
    /// @param[in]  elapsed_time - The amount of time elapsed since the last
    ///     update of the intro sequence.
    void IntroSequence::Update(const sf::Time& elapsed_time)
    {
        // UPDATE THE ELAPSED TIME FOR THE CURRENT FRAME.
        ElapsedTimeForCurrentFrame += elapsed_time;

        // CHECK IF THE CURRENT FRAME IS FINISHED BEING DISPLAYED.
        bool current_frame_finished_being_displayed = (ElapsedTimeForCurrentFrame >= MAX_TIME_PER_FRAME);
        if (current_frame_finished_being_displayed)
        {
            // MOVE TO THE NEXT FRAME.
            ++CurrentFrameIndex;
            ElapsedTimeForCurrentFrame = sf::Time::Zero;
        }
    }

    /// Renders the current frame of the intro sequence on screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void IntroSequence::Render(GRAPHICS::Renderer& renderer) const
    {
        // MAKE SURE THE INTRO SEQUENCE HASN'T COMPLETED.
        bool intro_sequence_completed = Completed();
        if (intro_sequence_completed)
        {
            // Nothing should be rendered if the intro sequence has completed.
            return;
        }

        // MAKE SURE FRAMES EXIST.
        bool frames_exist = !IntroBibleVerses.empty();
        if (!frames_exist)
        {
            // There's nothing to render.
            return;
        }

        // RENDER THE CURRENT BIBLE VERSE.
        const BIBLE::BibleVerse& current_bible_verse = IntroBibleVerses.at(CurrentFrameIndex);
        const std::string bible_verse_text = current_bible_verse.ToString();
        GRAPHICS::Color white;
        white.Red = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        white.Green = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        white.Blue = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        white.Alpha = GRAPHICS::Color::MAX_COLOR_COMPONENT;
        renderer.RenderCenteredText(bible_verse_text, renderer.Screen.GetBoundingRectangle<float>(), white);
    }
}
