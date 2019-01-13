#pragma once

#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include "Core/String.h"
#include "Graphics/Gui/TextBox.h"

/// A namespace for testing the TextBox class.
namespace TEST_TEXT_BOX
{
    using namespace GRAPHICS;
    using namespace GRAPHICS::GUI;

    TEST_CASE("A multipage Bible verse message can be properly rendered.", "[TextBox]")
    {
        // DEFINE THE TIME NEEDED TO DISPLAY A SINGLE CHARACTER.
        // Due to numerical precision issues, a small amount of buffer padding is added
        // by multiplying the elapsed time to be somewhere between the time for 1 and 2
        // characters.  Note that this time isn't entirely stable, so this "fix" hasn't
        // been propagated to every test case.  Each test case will only be modified as
        // needed.
        float elapsed_time_in_seconds_for_next_character = 1.5f * TextPage::ELAPSED_TIME_BETWEEN_CHARACTERS_IN_SECONDS;

        // CREATE A TEXT BOX.
        const unsigned int WIDTH_IN_PIXELS = 512;
        const unsigned int HEIGHT_IN_PIXELS = 32;
        TextBox text_box(WIDTH_IN_PIXELS, HEIGHT_IN_PIXELS);

        // DEFINE THE MESSAGE TO START DISPLAYING.
        // The Bible verse message was chosen based on a realistic scenario where this was failing.
        const std::string TEST_MESSAGE =
            "You got a Bible verse!\n"
            "1 7:7 - And Noah went in, and his sons, and his wife, and his sons' wives with him, into the ark, because of the waters of the flood.";
        text_box.StartDisplayingText(TEST_MESSAGE);

        // VERIFY THAT THE FIRST PAGE OF TEXT CAN BE PROPERLY DISPLAYED.
        const std::string EXPECTED_FIRST_PAGE_TEXT =
            "You got a Bible verse!\n";
        float elapsed_time_in_seconds_for_first_page = elapsed_time_in_seconds_for_next_character * EXPECTED_FIRST_PAGE_TEXT.length();
        text_box.Update(sf::seconds(elapsed_time_in_seconds_for_first_page));
        std::stringstream first_page_output;
        text_box.Render(first_page_output);
        std::string rendered_first_page_text = first_page_output.str();
        REQUIRE(EXPECTED_FIRST_PAGE_TEXT == rendered_first_page_text);

        // VERIFY THAT THE FIRST PAGE IS FINISHED BEING DISPLAYED.
        bool first_page_finished_being_displayed = text_box.CurrentPageOfTextFinishedBeingDisplayed();
        REQUIRE(first_page_finished_being_displayed);

        // MOVE THE TEXT BOX TO THE NEXT PAGE OF TEXT.
        text_box.MoveToNextPage();

        // VERIFY THAT THE SECOND PAGE OF TEXT CAN BE PROPERLY DISPLAYED.
        const std::string EXPECTED_SECOND_PAGE_TEXT =
            "1 7:7 - And Noah went in, and his sons,\n"
            "and his wife, and his sons' wives with\n";
        float elapsed_time_in_seconds_for_second_page = elapsed_time_in_seconds_for_next_character * EXPECTED_SECOND_PAGE_TEXT.length();
        text_box.Update(sf::seconds(elapsed_time_in_seconds_for_second_page));
        std::stringstream second_page_output;
        text_box.Render(second_page_output);
        std::string rendered_second_page_text = second_page_output.str();
        REQUIRE(EXPECTED_SECOND_PAGE_TEXT == rendered_second_page_text);

        // VERIFY THAT THE SECOND PAGE IS FINISHED BEING DISPLAYED.
        bool second_page_finished_being_displayed = text_box.CurrentPageOfTextFinishedBeingDisplayed();
        REQUIRE(second_page_finished_being_displayed);

        // MOVE THE TEXT BOX TO THE NEXT PAGE OF TEXT.
        text_box.MoveToNextPage();

        // VERIFY THAT THE THIRD PAGE OF TEXT CAN BE PROPERLY DISPLAYED.
        const std::string EXPECTED_THIRD_PAGE_TEXT =
            "him, into the ark, because of the waters\n"
            "of the flood.\n";
        float elapsed_time_in_seconds_for_third_page = elapsed_time_in_seconds_for_next_character * EXPECTED_THIRD_PAGE_TEXT.length();
        text_box.Update(sf::seconds(elapsed_time_in_seconds_for_third_page));
        std::stringstream third_page_output;
        text_box.Render(third_page_output);
        std::string rendered_third_page_text = third_page_output.str();
        REQUIRE(EXPECTED_THIRD_PAGE_TEXT == rendered_third_page_text);

        // VERIFY THAT THE THIRD PAGE IS FINISHED BEING DISPLAYED.
        bool third_page_finished_being_displayed = text_box.CurrentPageOfTextFinishedBeingDisplayed();
        REQUIRE(third_page_finished_being_displayed);

        // MOVE THE TEXT BOX TO THE NEXT PAGE OF TEXT.
        text_box.MoveToNextPage();
    }
}
