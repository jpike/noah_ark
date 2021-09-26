#include <algorithm>
#include <iterator>
#include "ErrorHandling/Asserts.h"
#include "Math/Number.h"
#include "Resources/AssetId.h"
#include "States/BibleVerseMiniGame.h"

namespace STATES
{
    /// Populates the mini-game with some initial verses from the provided verses.
    /// @param[in]  available_verses - The verses to potentially select from.
    /// @param[in,out]  random_number_generator - Random number generator to use in verse selection.
    void BibleVerseMiniGame::PopulateVersesFrom(const std::set<BIBLE::BibleVerse>* available_verses, MATH::RandomNumberGenerator& random_number_generator)
    {
        // ENSURE VERSES WERE PROVIDED.
        ASSERT_THEN_IF_NOT(available_verses)
        {
            // The game cannot be opened if no verses are available.
            IsOpen = false;
            return;
        }

        std::size_t available_verse_count = available_verses->size();
        bool enough_verses_available = (available_verse_count > MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME);
        if (!enough_verses_available)
        {
            // The game cannot be opened if enough verses are not available.
            IsOpen = false;
            return;
        }

        // SELECT ENOUGH VERSES FOR THE MINI-GAME.
        AvailableVerses = available_verses;
        CurrentVerses = GetRandomVerses(*AvailableVerses, random_number_generator);
        // The verse orders should be cleared for a new batch of verses.
        CurrentVerseOrders = {};
        // Other data should be reset to the beginning.
        CurrentlySelectedVerseIndex = 0;
        NextOrderNumberThatCanBeEntered = 1;
        CurrentSubstate = Substate::SELECTING_VERSES;
        ResultsMessage = "";
    }

    /// Updates the mini-game.
    /// @param[in,out]  gaming_hardware - The gaming hardware supplying input.
    void BibleVerseMiniGame::Update(HARDWARE::GamingHardware& gaming_hardware)
    {
        // CHECK IF THE PLAYER CHOSE TO END THE MINI-GAME.
        // This happens consistently regardess of substate.
        if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Escape))
        {
            // END THE MINI-GAME.
            IsOpen = false;
        }

        // UPDATE THE GAME BASED ON SUB-STATE.
        switch (CurrentSubstate)
        {
            case Substate::SELECTING_VERSES:
            {
                // UPDATE THE GAME BASED ON PLAYER INPUT.
                constexpr std::size_t MIN_VERSE_INDEX = 0;
                constexpr std::size_t MAX_VERSE_INDEX = MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME - 1;
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Up))
                {
                    // MOVE UP TO HAVE THE PREVIOUS VERSE SELECTED.
                    --CurrentlySelectedVerseIndex;
                    CurrentlySelectedVerseIndex = MATH::Number::Clamp<std::size_t>(CurrentlySelectedVerseIndex, MIN_VERSE_INDEX, MAX_VERSE_INDEX);
                }
                else if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Down))
                {
                    // MOVE DOWN TO HAVE THE NEXT VERSE SELECTED.
                    ++CurrentlySelectedVerseIndex;
                    CurrentlySelectedVerseIndex = MATH::Number::Clamp<std::size_t>(CurrentlySelectedVerseIndex, MIN_VERSE_INDEX, MAX_VERSE_INDEX);
                }
                else if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Enter))
                {
                    // SET THE NUMBER FOR THE CURRENT VERSE IF NOT ALREADY ORDERED.
                    std::size_t current_verse_order_number = CurrentVerseOrders[CurrentlySelectedVerseIndex];
                    bool current_verse_is_ordered = (UNORDERED_VERSE_NUMBER != current_verse_order_number);
                    if (!current_verse_is_ordered)
                    {
                        // SET THE VERSE ORDER NUMBER.
                        CurrentVerseOrders[CurrentlySelectedVerseIndex] = NextOrderNumberThatCanBeEntered;

                        // PREPARE FOR THE NEXT VERSE ORDER NUMBER TO BE ENTERED.
                        ++NextOrderNumberThatCanBeEntered;
                    }
                }

                // CHECK IF THE PLAYER ENTERED ORDERS FOR ALL VERSES.
                bool all_verses_ordered = (
                    (UNORDERED_VERSE_NUMBER != CurrentVerseOrders[0]) &&
                    (UNORDERED_VERSE_NUMBER != CurrentVerseOrders[1]) &&
                    (UNORDERED_VERSE_NUMBER != CurrentVerseOrders[2]));
                if (all_verses_ordered)
                {
                    // CHECK IF THE ORDERS ARE CORRECT.
                    const BIBLE::BibleVerse* first_verse = nullptr;
                    const BIBLE::BibleVerse* second_verse = nullptr;
                    const BIBLE::BibleVerse* third_verse = nullptr;
                    switch (CurrentVerseOrders[0])
                    {
                        case 1:
                            first_verse = &CurrentVerses[0];
                            break;
                        case 2:
                            second_verse = &CurrentVerses[0];
                            break;
                        case 3:
                            third_verse = &CurrentVerses[0];
                            break;
                    }

                    switch (CurrentVerseOrders[1])
                    {
                        case 1:
                            first_verse = &CurrentVerses[1];
                            break;
                        case 2:
                            second_verse = &CurrentVerses[1];
                            break;
                        case 3:
                            third_verse = &CurrentVerses[1];
                            break;
                    }

                    switch (CurrentVerseOrders[2])
                    {
                        case 1:
                            first_verse = &CurrentVerses[2];
                            break;
                        case 2:
                            second_verse = &CurrentVerses[2];
                            break;
                        case 3:
                            third_verse = &CurrentVerses[2];
                            break;
                    }

                    bool ordered_verses_found = (first_verse && second_verse && third_verse);
                    ASSERT_THEN_IF(ordered_verses_found)
                    {
                        bool verses_in_correct_order = (
                            ((*first_verse) < (*second_verse)) &&
                            ((*second_verse) < (*third_verse)));
                        if (verses_in_correct_order)
                        {
                            ResultsMessage = "Order correct!";
                        }
                        else
                        {
                            ResultsMessage = "Order incorrect.";
                        }
                    }

                    // MOVE TO THE RESULTS STATE.
                    CurrentSubstate = Substate::VIEWING_RESULTS;
                }

                break;
            }
            case Substate::VIEWING_RESULTS:
            {
                // CHECK IF THE PLAYER WANTED TO PLAY AGAIN.
                if (gaming_hardware.InputController.ButtonWasPressed(sf::Keyboard::Enter))
                {
                    // START THE GAME WITH A NEW SET OF VERSES.
                    PopulateVersesFrom(AvailableVerses, gaming_hardware.RandomNumberGenerator);
                }

                break;
            }
        }
    }

    /// Renders the mini-game.
    /// @param[in]  renderer - The renderer to use.
    void BibleVerseMiniGame::Render(GRAPHICS::Renderer& renderer)
    {
        // RENDER A COLORED BACKGROUND.
        constexpr float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        constexpr float SCREEN_TOP_POSITION_IN_PIXELS = 0.0f;
        float screen_width_in_pixels = renderer.Screen->WidthInPixels<float>();
        float screen_height_in_pixels = renderer.Screen->HeightInPixels<float>();
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            SCREEN_TOP_POSITION_IN_PIXELS,
            screen_width_in_pixels,
            screen_height_in_pixels);

        // The background color is chosen to be similar to that of the inventory Bible page.
        const GRAPHICS::Color BACKGROUND_COLOR = GRAPHICS::Color::ORANGE;
        renderer.RenderScreenRectangle(
            background_rectangle,
            BACKGROUND_COLOR);

        // RENDER INITIAL INSTRUCTIONS BASED ON SUBSTATE.
        const MATH::Vector2f SCREEN_LEFT_TOP_POSITION_IN_PIXELS(SCREEN_LEFT_POSITION_IN_PIXELS, SCREEN_TOP_POSITION_IN_PIXELS);
        switch (CurrentSubstate)
        {
            case Substate::SELECTING_VERSES:
            {
                const std::string INITIAL_INSTRUCTION_TEXT = "Put verses in the correct order.";
                renderer.RenderText(
                    INITIAL_INSTRUCTION_TEXT, 
                    RESOURCES::AssetId::FONT_TEXTURE,
                    SCREEN_LEFT_TOP_POSITION_IN_PIXELS);
                break;
            }
            case Substate::VIEWING_RESULTS:
            {
                renderer.RenderText(
                    ResultsMessage, 
                    RESOURCES::AssetId::FONT_TEXTURE,
                    SCREEN_LEFT_TOP_POSITION_IN_PIXELS);
                break;
            }
        }

        // RENDER THE VERSES.
        // Verse text needs to be rendered in boxes for wrapping.
        // To have the most space available for computing bounding boxes, the maximum number of instructions lines
        // needs to be removed from the total available vertical space.
        constexpr float INSTRUCTION_MAX_LINE_COUNT = 4.0f;
        constexpr float INSTRUCTION_MAX_VERTICAL_HEIGHT_IN_PIXELS = 4.0f * static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        float vertical_space_for_verse_text_in_pixels = screen_height_in_pixels - INSTRUCTION_MAX_VERTICAL_HEIGHT_IN_PIXELS;
        float vertical_space_per_verse_in_pixels = vertical_space_for_verse_text_in_pixels / static_cast<float>(MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME);

        MATH::Vector2f current_verse_line_screen_position_in_pixels = SCREEN_LEFT_TOP_POSITION_IN_PIXELS;
        current_verse_line_screen_position_in_pixels.Y += static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        for (std::size_t verse_index = 0; verse_index < MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME; ++verse_index)
        {
            // DETERMINE THE COLOR FOR THE CURRENT VERSE TEXT.
            // A lighter color is used for the currently selected verse.
            GRAPHICS::Color text_color = GRAPHICS::Color::BLACK;
            bool current_verse_is_selected = (verse_index == CurrentlySelectedVerseIndex);
            if (current_verse_is_selected)
            {
                text_color = GRAPHICS::Color::DARK_GRAY;
            }

            // RENDER THE ORDER FOR THE CURRENT VERSE.
            std::size_t current_verse_order_number = CurrentVerseOrders[verse_index];
            bool verse_ordered = (UNORDERED_VERSE_NUMBER != current_verse_order_number);
            if (verse_ordered)
            {
                // RENDER THE ORDER NUMBER.
                std::string current_verse_order_number_text = std::to_string(current_verse_order_number);
                renderer.RenderText(
                    current_verse_order_number_text, 
                    RESOURCES::AssetId::FONT_TEXTURE,
                    current_verse_line_screen_position_in_pixels,
                    text_color);
            }
            else
            {
                // RENDER A BLANK LINE.
                renderer.RenderText(
                    "_", 
                    RESOURCES::AssetId::FONT_TEXTURE,
                    current_verse_line_screen_position_in_pixels,
                    text_color);
            }

            // RENDER THE VERSE TEXT.
            constexpr float PADDING_BETWEEN_ORDER_AND_VERSE_TEXT = 3.0f * static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_WIDTH_IN_PIXELS);
            current_verse_line_screen_position_in_pixels.X += PADDING_BETWEEN_ORDER_AND_VERSE_TEXT;
            float verse_text_bounding_box_max_width_in_pixels = screen_width_in_pixels - current_verse_line_screen_position_in_pixels.X;
            MATH::FloatRectangle verse_text_bounding_box = MATH::FloatRectangle::FromLeftTopAndDimensions(
                current_verse_line_screen_position_in_pixels.X,
                current_verse_line_screen_position_in_pixels.Y,
                verse_text_bounding_box_max_width_in_pixels,
                vertical_space_per_verse_in_pixels);
            const BIBLE::BibleVerse& current_verse = CurrentVerses[verse_index];
            renderer.RenderText(
                current_verse.Text,
                RESOURCES::AssetId::FONT_TEXTURE,
                verse_text_bounding_box,
                text_color);

            // MOVE THE DRAWING POSITION FOR THE NEXT VERSE.
            current_verse_line_screen_position_in_pixels.X = SCREEN_LEFT_POSITION_IN_PIXELS;
            current_verse_line_screen_position_in_pixels.Y += vertical_space_per_verse_in_pixels;
        }

        // RENDER ADDITIONAL INSTRUCTIONS BASED ON SUB-STATE.
        // These should appear on the last several lines of the screen.
        constexpr float FINAL_INSTRUCTION_LINE_COUNT = 3.0f;
        MATH::Vector2f current_instruction_line_screen_position_in_pixels(SCREEN_LEFT_POSITION_IN_PIXELS, screen_height_in_pixels);
        current_instruction_line_screen_position_in_pixels.Y -= FINAL_INSTRUCTION_LINE_COUNT * static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);

        switch (CurrentSubstate)
        {
            case Substate::SELECTING_VERSES:
            {
                const std::string SELECT_VERSE_INSTRUCTIONS = "Arrow keys to move up/down verses.";
                renderer.RenderText(
                    SELECT_VERSE_INSTRUCTIONS, 
                    RESOURCES::AssetId::FONT_TEXTURE,
                    current_instruction_line_screen_position_in_pixels);

                current_instruction_line_screen_position_in_pixels.Y += static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
                const std::string NUMBER_VERSE_INSTRUCTIONS = "Enter to set current verse to next number.";
                renderer.RenderText(
                    NUMBER_VERSE_INSTRUCTIONS, 
                    RESOURCES::AssetId::FONT_TEXTURE,
                    current_instruction_line_screen_position_in_pixels);

                break;
            }
            case Substate::VIEWING_RESULTS:
            {
                current_instruction_line_screen_position_in_pixels.Y += static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
                const std::string NUMBER_VERSE_INSTRUCTIONS = "Enter to play again.";
                renderer.RenderText(
                    NUMBER_VERSE_INSTRUCTIONS, 
                    RESOURCES::AssetId::FONT_TEXTURE,
                    current_instruction_line_screen_position_in_pixels);

                break;
            }
        }

        current_instruction_line_screen_position_in_pixels.Y += static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        const std::string EXIT_MINI_GAME_INSTRUCTIONS = "ESC to exit mini-game.";
        renderer.RenderText(
            EXIT_MINI_GAME_INSTRUCTIONS, 
            RESOURCES::AssetId::FONT_TEXTURE,
            current_instruction_line_screen_position_in_pixels);
    }

    /// Selects an appropriate number of random verses.
    /// @param[in]  available_verses - The verses to select from.
    /// @param[in,out]  random_number_generator - The random number generator to use for verse selection.
    /// @return The appropriate number of random verses.
    std::array<BIBLE::BibleVerse, BibleVerseMiniGame::MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME> BibleVerseMiniGame::GetRandomVerses(
        const std::set<BIBLE::BibleVerse>& available_verses,
        MATH::RandomNumberGenerator& random_number_generator)
    {
        // RANDOMLY SELECT AN APPROPRIATE NUMBER OF VERSES.
        std::size_t found_verse_count = 0;
        std::array<BIBLE::BibleVerse, MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME> unique_random_verses;
        while (found_verse_count < MIN_REQUIRED_VERSE_COUNT_FOR_MINI_GAME)
        {
            // RANDOMLY SELECT A VERSE.
            std::size_t total_verse_count = available_verses.size();
            std::size_t random_verse_index = random_number_generator.RandomNumberLessThan(total_verse_count);
            auto random_verse = std::cbegin(available_verses);
            std::advance(random_verse, random_verse_index);

            auto previously_found_same_verse = std::find(unique_random_verses.cbegin(), unique_random_verses.cend(), *random_verse);
            bool verse_previously_found = (unique_random_verses.cend() != previously_found_same_verse);
            if (!verse_previously_found)
            {
                unique_random_verses[found_verse_count] = *random_verse;
                ++found_verse_count;
            }
        }

        return unique_random_verses;
    }
}
