#include <algorithm>
#include "Inventory/InventoryAnimalsPage.h"
#include "Resources/AnimalGraphics.h"

namespace INVENTORY
{
    // The exact color is currently arbitrary.
    const GRAPHICS::Color InventoryAnimalsPage::BACKGROUND_COLOR = GRAPHICS::Color::RED;

    /// Updates the animals page.
    /// @param[in]  elapsed_time - The elapsed time since the last frame.
    /// @param[in]  input_controller - The controller on which to check user input.
    void InventoryAnimalsPage::Update(const sf::Time& elapsed_time, const INPUT_CONTROL::InputController& input_controller)
    {
        // CHECK IF A SCROLLING BUTTON WAS PRESSED OR RELEASED.
        // This ensures that even if the press/release occurs really quickly, the user will still
        // see the list scroll to the next row.
        if (input_controller.ButtonWasPressed(sf::Keyboard::Up))
        {
            // MOVE TO THE PREVIOUS ANIMAL SPECIES.
            SelectPreviousAnimalSpecies();

            // RESET THE TIMER TRACKING HOW LONG A SCROLL KEY HAS BEEN DOWN SINCE A COMPLETE KEY PRESS OCCURRED.
            ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals = sf::Time::Zero;

            // Nothing else needs to be done if a full key press was detected.
            return;
        }
        if (input_controller.ButtonWasPressed(sf::Keyboard::Down))
        {
            // MOVE TO THE NEXT ANIMAL SPECIES.
            SelectNextAnimalSpecies();

            // RESET THE TIMER TRACKING HOW LONG A SCROLL KEY HAS BEEN DOWN SINCE A COMPLETE KEY PRESS OCCURRED.
            ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals = sf::Time::Zero;

            // Nothing else needs to be done if a full key press was detected.
            return;
        }

        // TRACK HOW LONG A KEY HAS BEEN PRESSED FOR SCROLLING THROUGH ANIMALS.
        // Having the user press and release a key for scrolling through animals takes too long,
        // particularly when this page doesn't involve as much reading as the Bible verses page.
        // Therefore, to allow easier scrolling, scrolling is allowed if the user holds an
        // appropriate key down long enough.  We can't just always check if the keys are down
        // since that would result in scrolling too fast - potentially preventing the player
        // from being able to land on selecting a particular animal.
        const sf::Time ELAPSED_TIME_FOR_KEY_DOWN_BEFORE_MOVING_TO_ADJACENT_ANIMAL = sf::seconds(0.1f);
        if (input_controller.ButtonDown(sf::Keyboard::Up))
        {
            // UPDATE THE ELAPSED TIME FOR A SCROLLING KEY BEING PRESSED.
            ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals += elapsed_time;

            // MOVE TO THE PREVIOUS ANIMAL IF ENOUGH TIME ELAPSED.
            bool scroll_key_held_down_long_enough = (ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals >= ELAPSED_TIME_FOR_KEY_DOWN_BEFORE_MOVING_TO_ADJACENT_ANIMAL);
            if (scroll_key_held_down_long_enough)
            {
                SelectPreviousAnimalSpecies();

                // Since the previous animal has been switched to, the timer needs to be reset.
                ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals = sf::Time::Zero;
            }
        }
        else if (input_controller.ButtonDown(sf::Keyboard::Down))
        {
            // UPDATE THE ELAPSED TIME FOR A SCROLLING KEY BEING PRESSED.
            ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals += elapsed_time;

            // MOVE TO THE NEXT ANIMAL IF ENOUGH TIME ELAPSED.
            bool scroll_key_held_down_long_enough = (ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals >= ELAPSED_TIME_FOR_KEY_DOWN_BEFORE_MOVING_TO_ADJACENT_ANIMAL);
            if (scroll_key_held_down_long_enough)
            {
                SelectNextAnimalSpecies();

                // Since the previous animal has been switched to, the timer needs to be reset.
                ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals = sf::Time::Zero;
            }
        }
        else
        {
            // RESET THE TIMER TRACKING HOW LONG A SCROLL KEY HAS BEEN DOWN SINCE NO KEY WAS DOWN THIS FRAME.
            ElapsedTimeWithScrollKeyHeldDownBeforeSwitchingAnimals = sf::Time::Zero;
        }
    }

    /// Renders the inventory GUI animals page to the provided screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryAnimalsPage::Render(const STATES::SavedGameData& current_game_data, GRAPHICS::Renderer& renderer) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        const float SCREEN_LEFT_POSITION_IN_PIXELS = 0.0f;
        const float BACKGROUND_HEIGHT_IN_PIXELS = renderer.Screen->HeightInPixels<float>() - TOP_SCREEN_OFFSET_IN_PIXELS;
        MATH::FloatRectangle background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            SCREEN_LEFT_POSITION_IN_PIXELS,
            TOP_SCREEN_OFFSET_IN_PIXELS,
            renderer.Screen->WidthInPixels<float>(),
            BACKGROUND_HEIGHT_IN_PIXELS);

        renderer.RenderScreenRectangle(
            background_rectangle,
            BACKGROUND_COLOR);

        // RENDER A DARKER BACKGROUND FOR ANIMALS TABLE.
        // The table is slightly indented from all sides of this inventory page.
        constexpr float ANIMALS_TABLE_SINGLE_SIDE_PADDING_IN_PIXELS = 4.0f;
        float animals_table_left_screen_position = background_rectangle.LeftTop.X + ANIMALS_TABLE_SINGLE_SIDE_PADDING_IN_PIXELS;
        float animals_table_top_screen_position = background_rectangle.LeftTop.Y + ANIMALS_TABLE_SINGLE_SIDE_PADDING_IN_PIXELS;
        // To account for padding on both sides (left/right and top/bottom), the dimensions must be shrunk by twice as much.
        constexpr float ANIMALS_TABLE_BOTH_SIDES_PADDING_IN_PIXELS = 2.0f * ANIMALS_TABLE_SINGLE_SIDE_PADDING_IN_PIXELS;
        float animals_table_screen_width = background_rectangle.Width() - ANIMALS_TABLE_BOTH_SIDES_PADDING_IN_PIXELS;
        float animals_table_screen_height = background_rectangle.Height() - ANIMALS_TABLE_BOTH_SIDES_PADDING_IN_PIXELS;
        MATH::FloatRectangle animals_table_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
            animals_table_left_screen_position,
            animals_table_top_screen_position,
            animals_table_screen_width,
            animals_table_screen_height);

        // The table is slightly darker to make it easier to see.
        const uint8_t ARBITRARY_BACKGROUND_COLOR_SCALE_FACTOR = 2;
        GRAPHICS::Color animal_box_background_color = BACKGROUND_COLOR;
        animal_box_background_color.ScaleRgbDown(ARBITRARY_BACKGROUND_COLOR_SCALE_FACTOR);
        renderer.RenderScreenRectangle(animals_table_rectangle, animal_box_background_color);

        // RENDER COLUMN HEADERS FOR THE ANIMALS TABLE.
        // A small offset is added before the first column's text to avoid having the text
        // right next to border lines in the table.  Since the later columns' positions
        // are based on this original column, they won't need this additional offset.
        constexpr float COLUMN_HEADER_TEXT_LEFT_SCREEN_OFFSET = 4.0f;
        GRAPHICS::GUI::Text species_column_header_text =
        {
            // An extra space is added before the male/female label to have that label more line up
            // with the positions of actual icons.
            .String = "SPECIES (M,F)",
            .LeftTopPosition = MATH::Vector2f(animals_table_rectangle.LeftTop.X + COLUMN_HEADER_TEXT_LEFT_SCREEN_OFFSET, animals_table_rectangle.LeftTop.Y)
        };
        renderer.Render(species_column_header_text);

        // Since there's plenty of space for each column, the width is evenly distributed across each column.
        float column_screen_width = animals_table_rectangle.Width() / 3.0f;
        float count_following_column_header_left_position = species_column_header_text.LeftTopPosition.X + column_screen_width;
        GRAPHICS::GUI::Text count_following_column_header_text =
        {
            .String = "FOLLOWING",
            .LeftTopPosition = MATH::Vector2f(count_following_column_header_left_position, animals_table_rectangle.LeftTop.Y)
        };
        renderer.Render(count_following_column_header_text);

        float count_in_ark_column_header_left_position = count_following_column_header_text.LeftTopPosition.X + column_screen_width;
        GRAPHICS::GUI::Text count_in_ark_column_header_text =
        {
            .String = "ARK",
            .LeftTopPosition = MATH::Vector2f(count_in_ark_column_header_left_position, animals_table_rectangle.LeftTop.Y)
        };
        renderer.Render(count_in_ark_column_header_text);

        // DETERMINE THE LIST OF ANIMAL SPECIES TO RENDER.
        // The list should generally have the currently selected species in the middle and
        // show about an even number of species on each side to fill the remaining space in the box.
        float column_screen_height = species_column_header_text.ScaleFactor * static_cast<float>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);
        std::size_t animal_species_list_visible_box_height_in_pixels = static_cast<std::size_t>(animals_table_rectangle.Height() - column_screen_height);

        std::size_t row_screen_height = static_cast<std::size_t>(GRAPHICS::GUI::Glyph::DEFAULT_HEIGHT_IN_PIXELS);

        constexpr std::size_t ONE_LESS_SPECIES_TO_AVOID_EXCEEDING_BOX_BOUNDS = 1;
        std::size_t species_to_render_count = (animal_species_list_visible_box_height_in_pixels / row_screen_height) - ONE_LESS_SPECIES_TO_AVOID_EXCEEDING_BOX_BOUNDS;
        std::size_t species_to_render_half_count = species_to_render_count / 2;

        // If there aren't enough species before the selected species, just start rendering
        // with the first possible species.  This effectively prevents "scrolling" within
        // the list box from occurring until it is becomes more necessary to start showing
        // more verses.
        std::size_t first_species_to_render_index = 0;
        bool enough_previous_species = (SelectedAnimalSpeciesIndex > species_to_render_half_count);
        if (enough_previous_species)
        {
            // Start rendering species prior to the selected one so that the selected species
            // appears in the middle, allowing players to more easily see what surrounding
            // species might exist.
            first_species_to_render_index = SelectedAnimalSpeciesIndex - species_to_render_half_count;
        }

        constexpr std::size_t SPECIES_COUNT = static_cast<std::size_t>(OBJECTS::AnimalSpecies::COUNT);
        std::size_t last_species_to_render_index = first_species_to_render_index + species_to_render_count;
        std::size_t last_valid_species_index = SPECIES_COUNT - 1;
        last_species_to_render_index = std::min(last_species_to_render_index, last_valid_species_index);

        // RENDER ROWS FOR EACH ANIMAL SPECIES IN VIEW.
        float current_animal_species_row_top_y_position = species_column_header_text.BottomPosition();
        for (std::size_t species_id = first_species_to_render_index; species_id <= last_species_to_render_index; ++species_id)
        {
            // CHECK IF THE CURRENT SPECIES IS THE SELECTED SPECIES.
            bool current_species_is_selected = (SelectedAnimalSpeciesIndex == species_id);
            if (current_species_is_selected)
            {
                // RENDER A DARKER BOX FOR THE SELECTED SPECIES.
                constexpr uint8_t ARBITRARY_SELECTED_COLOR_SCALE_FACTOR = 2;
                GRAPHICS::Color selected_species_background_color = animal_box_background_color;
                selected_species_background_color.ScaleRgbDown(ARBITRARY_SELECTED_COLOR_SCALE_FACTOR);
                MATH::FloatRectangle selected_species_background_rectangle = MATH::FloatRectangle::FromLeftTopAndDimensions(
                    animals_table_rectangle.LeftTop.X,
                    current_animal_species_row_top_y_position,
                    animals_table_rectangle.Width(),
                    static_cast<float>(row_screen_height));
                renderer.RenderScreenRectangle(
                    selected_species_background_rectangle,
                    selected_species_background_color);
            }

            // CHECK IF THE SPECIES HAS BEEN COLLECTED.
            OBJECTS::AnimalSpecies::Value species = static_cast<OBJECTS::AnimalSpecies::Value>(species_id);
            AnimalCollectionStatistics species_collection_statistics = current_game_data.GetAnimalCollectionStatistics(species);
            bool species_collected = species_collection_statistics.Collected();
            if (species_collected)
            {
                // RENDER THE NAME OF THE CURRENT SPECIES.
                GRAPHICS::GUI::Text collected_species_text
                {
                    .String = OBJECTS::AnimalSpecies::NAMES[species_id],
                    .LeftTopPosition = MATH::Vector2f(species_column_header_text.LeftTopPosition.X, current_animal_species_row_top_y_position)
                };
                renderer.Render(collected_species_text);

                // RENDER AN ICON FOR THE MALE ANIMAL (IF COLLECTED).
                // It should be placed toward the right end of the species column, leaving room for the female icon.
                constexpr float COUNT_BOTH_MALE_AND_FEMALE_ICONS = 2.0f;
                constexpr float SPACING_BEFORE_START_OF_NEXT_COLUMN_HEADER = 16.0f;
                float male_animal_icon_left_screen_position = (
                    count_following_column_header_left_position - 
                    SPACING_BEFORE_START_OF_NEXT_COLUMN_HEADER -
                    (COUNT_BOTH_MALE_AND_FEMALE_ICONS * RESOURCES::AnimalGraphics::SPRITE_WIDTH_IN_PIXELS));
                MATH::Vector2f male_animal_icon_left_top_screen_position = MATH::Vector2f(
                    male_animal_icon_left_screen_position,
                    collected_species_text.LeftTopPosition.Y);
                bool male_of_species_collected = species_collection_statistics.MaleCollected();
                if (male_of_species_collected)
                {
                    OBJECTS::AnimalType male_animal_type(species, OBJECTS::AnimalGender::MALE);
                    std::shared_ptr<GRAPHICS::AnimatedSprite> male_animal_sprite = RESOURCES::AnimalGraphics::GetSprite(male_animal_type);
                    if (male_animal_sprite)
                    {
                        renderer.RenderGuiIcon(male_animal_sprite->CurrentFrameSprite, male_animal_icon_left_top_screen_position);
                    }
                }

                // RENDER AN ICON FOR THE FEMALE ANIMAL (IF COLLECTED).
                bool female_of_species_collected = species_collection_statistics.FemaleCollected();
                if (female_of_species_collected)
                {
                    OBJECTS::AnimalType female_animal_type(species, OBJECTS::AnimalGender::FEMALE);
                    std::shared_ptr<GRAPHICS::AnimatedSprite> female_animal_sprite = RESOURCES::AnimalGraphics::GetSprite(female_animal_type);
                    if (female_animal_sprite)
                    {
                        // It should be placed at the right end of this column, after the male icon.
                        // A little bit of extra padding is added to space out the icons.
                        constexpr float SPACING_IN_PIXELS_BETWEEN_ANIMAL_ICONS = 8.0f;
                        MATH::Vector2f female_animal_icon_left_top_screen_position = male_animal_icon_left_top_screen_position;
                        female_animal_icon_left_top_screen_position.X += RESOURCES::AnimalGraphics::SPRITE_WIDTH_IN_PIXELS + SPACING_IN_PIXELS_BETWEEN_ANIMAL_ICONS;
                        renderer.RenderGuiIcon(female_animal_sprite->CurrentFrameSprite, female_animal_icon_left_top_screen_position);
                    }
                }

                // RENDER TEXT INDICATING HOW MANY OF THE SPECIES ARE FOLLOWING THE PLAYER.
                GRAPHICS::GUI::Text species_following_player_text
                {
                    .String = "M: " + std::to_string(species_collection_statistics.MaleFollowingPlayerCount) + " F: " + std::to_string(species_collection_statistics.FemaleFollowingPlayerCount),
                    .LeftTopPosition = MATH::Vector2f(count_following_column_header_text.LeftTopPosition.X, current_animal_species_row_top_y_position)
                };
                renderer.Render(species_following_player_text);

                // RENDER TEXT INDICATING HOW MANY OF THE SPECIES ARE IN THE ARK.
                GRAPHICS::GUI::Text species_in_ark_text
                {
                    .String = "M: " + std::to_string(species_collection_statistics.MaleInArkCount) + " F: " + std::to_string(species_collection_statistics.FemaleInArkCount),
                    .LeftTopPosition = MATH::Vector2f(count_in_ark_column_header_text.LeftTopPosition.X, current_animal_species_row_top_y_position)
                };
                renderer.Render(species_in_ark_text);
            }
            else
            {
                // RENDER QUESTION MARKS IF THE CURRENT SPECIES HASN'T BEEN COLLECTED AT ALL.
                // This will leave some mystery as to what species remain for collection.
                GRAPHICS::GUI::Text uncollected_species_text
                {
                    .String = "????????",
                    .LeftTopPosition = MATH::Vector2f(species_column_header_text.LeftTopPosition.X, current_animal_species_row_top_y_position)
                };
                renderer.Render(uncollected_species_text);
            }

            // CALCULATE THE Y POSITION FOR THE NEW ROW.
            current_animal_species_row_top_y_position += static_cast<float>(row_screen_height);
        }

        // RENDER LINES SEPARATING MAJOR COMPONENTS OF THE TABLE.
        // These are rendered last so that they appear on top of any shaded table rows for the current selection.
        const GRAPHICS::Color ANIMALS_TABLE_BORDER_COLOR = GRAPHICS::Color::BLACK;

        // One line is drawn to separate column headers from the table body.
        float table_header_body_separator_line_y_position = species_column_header_text.BottomPosition();
        MATH::Vector2f table_header_body_separator_line_start_position(
            animals_table_rectangle.LeftTop.X,
            table_header_body_separator_line_y_position);
        MATH::Vector2f table_header_body_separator_line_end_position(
            animals_table_rectangle.RightBottom.X,
            table_header_body_separator_line_y_position);
        renderer.RenderLine(table_header_body_separator_line_start_position, table_header_body_separator_line_end_position, ANIMALS_TABLE_BORDER_COLOR);

        // One line is drawn to separate each column.
        float species_following_column_separator_line_x_position = animals_table_rectangle.LeftTop.X + column_screen_width;
        MATH::Vector2f species_following_column_separator_line_start_position(
            species_following_column_separator_line_x_position,
            animals_table_rectangle.LeftTop.Y);
        MATH::Vector2f species_following_column_separator_line_end_position(
            species_following_column_separator_line_x_position,
            animals_table_rectangle.RightBottom.Y);
        renderer.RenderLine(species_following_column_separator_line_start_position, species_following_column_separator_line_end_position, ANIMALS_TABLE_BORDER_COLOR);

        constexpr float AFTER_SECOND_COLUMN = 2.0f;
        float following_ark_column_separator_line_x_position = animals_table_rectangle.LeftTop.X + (AFTER_SECOND_COLUMN * column_screen_width);
        MATH::Vector2f following_ark_column_separator_line_start_position(
            following_ark_column_separator_line_x_position,
            animals_table_rectangle.LeftTop.Y);
        MATH::Vector2f following_ark_column_separator_line_end_position(
            following_ark_column_separator_line_x_position,
            animals_table_rectangle.RightBottom.Y);
        renderer.RenderLine(following_ark_column_separator_line_start_position, following_ark_column_separator_line_end_position, ANIMALS_TABLE_BORDER_COLOR);
    }

    /// Selects the previous animal species in the list, if one exists.
    /// If a previous species does not exist, the currently selected species remains.
    void InventoryAnimalsPage::SelectPreviousAnimalSpecies()
    {
        // If subtracting 1 causes the species index to go negative,
        // the number will actually be a larger positive number
        // (due to being an unsigned integer), so the min function
        // call will work to ensure the selected verse doesn't
        // go below the verse at index 0.
        std::size_t previous_species_index = SelectedAnimalSpeciesIndex - 1;
        SelectedAnimalSpeciesIndex = std::min(SelectedAnimalSpeciesIndex, previous_species_index);
    }

    /// Selects the next animal species in the list, if one exists.
    /// If a next species does not exist, the currently selected species remains.
    void InventoryAnimalsPage::SelectNextAnimalSpecies()
    {
        std::size_t next_species_index = SelectedAnimalSpeciesIndex + 1;
        std::size_t last_valid_species_index = static_cast<std::size_t>(OBJECTS::AnimalSpecies::COUNT) - 1;
        SelectedAnimalSpeciesIndex = std::min(next_species_index, last_valid_species_index);
    }
}
