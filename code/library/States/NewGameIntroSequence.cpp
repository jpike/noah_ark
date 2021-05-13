#include "Resources/AssetId.h"
#include "States/NewGameIntroSequence.h"

namespace STATES
{
#if _DEBUG
    // A shorter time is used for debug builds to speed up debugging.
    const sf::Time NewGameIntroSequence::MAX_TIME_PER_FRAME = sf::seconds(3);
#else
    const sf::Time NewGameIntroSequence::MAX_TIME_PER_FRAME = sf::seconds(9);
#endif

    /// Loads the sequence to its initial state.
    /// @param[in,out]  world - The world in which the intro sequence will take place.
    void NewGameIntroSequence::Load(MAPS::World& world)
    {
        // RESET THE CURRENT FRAME TO THE BEGINNING.
        CurrentFrameIndex = 0;
        ElapsedTimeForCurrentFrame = sf::Time::Zero;

        // RESET THE PLAYER'S POSITION.
        world.NoahPlayer->SetWorldPosition(OBJECTS::Noah::DEFAULT_START_WORLD_POSITION);
    }

    /// Update the intro sequence based on an elapsed amount of time.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input and output.
    /// @return The state that the game should be in after the update.
    GameState NewGameIntroSequence::Update(HARDWARE::GamingHardware& gaming_hardware)
    {
        // PLAY THE APPROPRIATE MUSIC IF NOT ALREADY PLAYING.
        gaming_hardware.Speakers->PlayMusicIfNotAlready(RESOURCES::AssetId::NEW_GAME_INTRO_MUSIC);

        // UPDATE THE ELAPSED TIME FOR THE CURRENT FRAME.
        ElapsedTimeForCurrentFrame += gaming_hardware.Clock.ElapsedTimeSinceLastFrame;

        // CHECK IF THE CURRENT FRAME IS FINISHED BEING DISPLAYED.
        bool current_frame_finished_being_displayed = (ElapsedTimeForCurrentFrame >= MAX_TIME_PER_FRAME);
        if (current_frame_finished_being_displayed)
        {
            // MOVE TO THE NEXT FRAME.
            ++CurrentFrameIndex;
            ElapsedTimeForCurrentFrame = sf::Time::Zero;
        }

        // CHECK IF THE INTRO SEQUENCE HAS COMPLETED.
        bool intro_sequence_completed = Completed();
        if (intro_sequence_completed)
        {
            // TRANSITION INTO THE START OF A NEW GAME WHERE INSTRUCTIONS ARE GIVEN.
            return GameState::NEW_GAME_INSTRUCTION_SEQUENCE;
        }
        else
        {
            // REMAIN ON THIS INTRO SEQUENCE UNTIL IT COMPLETES.
            return GameState::NEW_GAME_INTRO_SEQUENCE;
        }
    }

    /// Renders the current frame of the intro sequence.
    /// @param[in,out]  renderer - The renderer to use.
    /// @return The rendered new game intro sequence.
    sf::Sprite NewGameIntroSequence::Render(GRAPHICS::Renderer& renderer) const
    {
        // MAKE SURE THE INTRO SEQUENCE HASN'T COMPLETED.
        bool intro_sequence_completed = Completed();
        if (intro_sequence_completed)
        {
            // Nothing should be rendered if the intro sequence has completed.
            return sf::Sprite();
        }

        // MAKE SURE FRAMES EXIST.
        bool frames_exist = !BibleVerses().empty();
        if (!frames_exist)
        {
            // There's nothing to render.
            return sf::Sprite();
        }

        // RENDER THE CURRENT BIBLE VERSE.
        const BIBLE::BibleVerse& current_bible_verse = BibleVerses().at(CurrentFrameIndex);
        const std::string bible_verse_text = current_bible_verse.ToString();
        // The text color is scaled to fade in and and then out for each frame.
        // A sine wave is used to help scale the color so that it starts dark,
        // peaks halfway between the the frame being displayed, and then
        // gets dark again.
        GRAPHICS::Color text_color = GRAPHICS::Color::WHITE;
        float current_verse_displayed_time_in_seconds = ElapsedTimeForCurrentFrame.asSeconds();
        float max_verse_displayed_time_in_seconds = MAX_TIME_PER_FRAME.asSeconds();
        constexpr float PI = 3.14159f;
        constexpr float RADIANS_FOR_HALF_OF_SINE_WAVE = PI;
        float scale_factor_for_half_sine_wave_to_match_verse_frame_time = RADIANS_FOR_HALF_OF_SINE_WAVE / max_verse_displayed_time_in_seconds;
        float text_scale_factor = std::sin(current_verse_displayed_time_in_seconds * scale_factor_for_half_sine_wave_to_match_verse_frame_time);
        text_color.ScaleRgb(text_scale_factor);
        renderer.RenderCenteredText(
            bible_verse_text,
            RESOURCES::AssetId::SERIF_FONT_TEXTURE,
            renderer.Screen->GetBoundingRectangle<float>(),
            text_color);

        // RETURN THE FINAL RENDERED SCREEN.
        sf::Sprite screen = renderer.RenderFinalScreen();
        return screen;
    }

    /// Gets the Bible verses, in order displayed as part of the intro sequence.
    /// @return Bible verses included in the intro sequence.
    const std::vector<BIBLE::BibleVerse>& NewGameIntroSequence::BibleVerses()
    {
        static std::vector<BIBLE::BibleVerse> bible_verses;

        // Add the Bible verses only if they haven't been found yet.
        if (bible_verses.empty())
        {
            // GET THE BIBLE VERSES FOR THE INTRO SEQUENCE.
            const BIBLE::BibleVerse* intro_verse_1 = BIBLE::FindBibleVerse(
                BIBLE::BibleBook::GENESIS,
                6,
                5);
            bool intro_verse_1_exists = (nullptr != intro_verse_1);
            if (intro_verse_1_exists)
            {
                bible_verses.push_back(*intro_verse_1);
            }

            const BIBLE::BibleVerse* intro_verse_2 = BIBLE::FindBibleVerse(
                BIBLE::BibleBook::GENESIS,
                6,
                6);
            bool intro_verse_2_exists = (nullptr != intro_verse_2);
            if (intro_verse_2_exists)
            {
                bible_verses.push_back(*intro_verse_2);
            }

            const BIBLE::BibleVerse* intro_verse_3 = BIBLE::FindBibleVerse(
                BIBLE::BibleBook::GENESIS,
                6,
                7);
            bool intro_verse_3_exists = (nullptr != intro_verse_3);
            if (intro_verse_3_exists)
            {
                bible_verses.push_back(*intro_verse_3);
            }

            const BIBLE::BibleVerse* intro_verse_4 = BIBLE::FindBibleVerse(
                BIBLE::BibleBook::GENESIS,
                6,
                8);
            bool intro_verse_4_exists = (nullptr != intro_verse_4);
            if (intro_verse_4_exists)
            {
                bible_verses.push_back(*intro_verse_4);
            }
        }

        return bible_verses;
    }

    /// Determines if the intro sequence has completed.
    /// @return True if the intro sequence has completed; false otherwise.
    bool NewGameIntroSequence::Completed() const
    {
        // The intro sequence is only completed if the current frame index
        // is greater than the last frame index as the last frame needs
        // to be displayed on screen for some period of time.
        size_t last_frame_index = BibleVerses().size() - 1;
        bool intro_sequence_completed = (CurrentFrameIndex > last_frame_index);
        return intro_sequence_completed;
    }
}
