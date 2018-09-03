#include "Core/NullChecking.h"
#include "Inventory/InventoryAnimalsPage.h"
#include "Resources/AnimalGraphics.h"

namespace INVENTORY
{
    // The exact color is currently arbitrary.
    const GRAPHICS::Color InventoryAnimalsPage::BACKGROUND_COLOR = GRAPHICS::Color::RED;

    /// Constructor.
    /// @param[in]  inventory - The inventory to display in the GUI.
    /// @param[in]  assets - The assets to use for the page.
    /// @throws std::exception - Thrown if a parameter is null.
    InventoryAnimalsPage::InventoryAnimalsPage(
        const std::shared_ptr<const INVENTORY::Inventory>& inventory,
        const std::shared_ptr<RESOURCES::Assets>& assets) :
    Inventory(inventory),
    Assets(assets)
    {
        // MAKE SURE THE REQUIRED RESOURCES WERE PROVIDED.
        CORE::ThrowInvalidArgumentExceptionIfNull(Inventory, "Null inventory provided to inventory animals page.");
        CORE::ThrowInvalidArgumentExceptionIfNull(Assets, "Null assets provided to inventory animals page.");
    }

    /// Renders the inventory GUI animals page to the provided screen.
    /// @param[in,out]  renderer - The renderer to use for rendering.
    void InventoryAnimalsPage::Render(GRAPHICS::Renderer& renderer) const
    {
        // RENDER A RECTANGLE FOR THE PAGE'S BACKGROUND.
        // It is offset from the top of the screen by the amount of the
        // GUI stuff that should always be displayed above it.  Otherwise,
        // it should cover the remainder of the screen.
        const float TOP_SCREEN_OFFSET_IN_PIXELS = static_cast<float>(2 * GRAPHICS::GUI::Glyph::HEIGHT_IN_PIXELS);
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

        // CALCULATE HOW MANY ANIMAL BOXES CAN APPEAR PER ROW AND COLUMN.
        // Some padding is added around the boxes in order to space them out.
        /// @todo   I'm not sure I like having the boxes spaced out like this (as opposed to how
        /// all of the animal icons were nice and compact together before).  This may be just because
        /// the boxes are primitive (no borders), but it is worthwhile to revisit this
        /// approach later.
        const float BOX_DIMENSION_IN_PIXELS = 48.0f;
        const float PADDING_BETWEEN_BOXES_IN_PIXELS = 16.0f;
        const float BOX_WITH_PADDING_DIMENSION_IN_PIXELS = BOX_DIMENSION_IN_PIXELS + PADDING_BETWEEN_BOXES_IN_PIXELS;
        float page_width_in_pixels = background_rectangle.GetWidth();
        unsigned int boxes_per_row = static_cast<unsigned int>(page_width_in_pixels / BOX_WITH_PADDING_DIMENSION_IN_PIXELS);

        // RENDER BOXES FOR EACH COLLECTED ANIMAL.
        const float BOX_HALF_DIMENSION_IN_PIXELS = BOX_DIMENSION_IN_PIXELS / 2.0f;
        float starting_box_center_x_position = background_rectangle.GetLeftXPosition() + PADDING_BETWEEN_BOXES_IN_PIXELS + BOX_HALF_DIMENSION_IN_PIXELS;
        float starting_box_center_y_position = background_rectangle.GetTopYPosition() + PADDING_BETWEEN_BOXES_IN_PIXELS + BOX_HALF_DIMENSION_IN_PIXELS;
        for (unsigned int species_id = 0; species_id < static_cast<unsigned int>(OBJECTS::AnimalSpecies::COUNT); ++species_id)
        {
            // GET THE COLLECTED COUNT INFORMATION FOR EACH GENDER OF THE SPECIES.
            OBJECTS::AnimalSpecies species = static_cast<OBJECTS::AnimalSpecies>(species_id);
            unsigned int species_male_animal_collected_count = 0;
            unsigned int species_female_animal_collected_count = 0;
            Inventory->GetAnimalCollectedCount(species, species_male_animal_collected_count, species_female_animal_collected_count);

            // CALCULATE THE BOX FOR THIS ANIMAL SPECIES.
            /// @todo Support scrolling through multiples "pages" within this inventory page
            /// as not all animals will be able to fit on a single screen.
            unsigned int box_row_index = species_id / boxes_per_row;
            unsigned int box_column_index = species_id % boxes_per_row;

            float current_box_x_offset = static_cast<float>(box_column_index * BOX_WITH_PADDING_DIMENSION_IN_PIXELS);
            float current_box_center_x_position = starting_box_center_x_position + current_box_x_offset;

            float current_box_y_offset = static_cast<float>(box_row_index * BOX_WITH_PADDING_DIMENSION_IN_PIXELS);
            float current_box_center_y_position = starting_box_center_y_position + current_box_y_offset;

            MATH::FloatRectangle box_screen_rectangle = MATH::FloatRectangle::FromCenterAndDimensions(
                current_box_center_x_position,
                current_box_center_y_position,
                BOX_DIMENSION_IN_PIXELS,
                BOX_DIMENSION_IN_PIXELS);

            // RENDER A BOX FOR ANIMAL SPECIES.
            RenderAnimalBox(
                species, 
                species_male_animal_collected_count, 
                species_female_animal_collected_count,
                box_screen_rectangle,
                renderer);
        }
    }

    /// Renders a box for an animal on the page.
    /// @param[in]  species - The species of animal to render.
    /// @param[in]  species_male_animal_collected_count - The count of male animals collected of the species.
    /// @param[in]  species_female_animal_collected_count - The count of female animals collected of the species.
    /// @param[in]  box_screen_rectangle - The placement/dimensions of the box to render, in screen coordinates.
    /// @param[in,out]  renderer - The renderer to use.
    void InventoryAnimalsPage::RenderAnimalBox(
        const OBJECTS::AnimalSpecies species,
        const unsigned int species_male_animal_collected_count,
        const unsigned int species_female_animal_collected_count,
        const MATH::FloatRectangle& box_screen_rectangle,
        GRAPHICS::Renderer& renderer) const
    {
        // RENDER A SLIGHTLY DARKER RECTANGLE FOR THE BACKGROUND.
        const uint8_t ARBITRARY_BACKGROUND_COLOR_SCALE_FACTOR = 2;
        GRAPHICS::Color animal_box_background_color = BACKGROUND_COLOR;
        animal_box_background_color.ScaleRgbDown(ARBITRARY_BACKGROUND_COLOR_SCALE_FACTOR);
        renderer.RenderScreenRectangle(box_screen_rectangle, animal_box_background_color);

        // RENDER INFORMATION ABOUT ANY COLLECTED MALE ANIMALS.
        bool male_animal_collected = (species_male_animal_collected_count > 0);
        if (male_animal_collected)
        {
            // RENDER AN ICON FOR THE MALE ANIMAL.
            // It should appear in the left-top corner of the box.
            OBJECTS::AnimalType male_animal_type(species, OBJECTS::AnimalGender::MALE);
            std::shared_ptr<GRAPHICS::AnimatedSprite> male_animal_sprite = RESOURCES::AnimalGraphics::GetSprite(male_animal_type, *Assets);
            if (male_animal_sprite)
            {
                MATH::Vector2f male_animal_icon_left_top_screen_position = box_screen_rectangle.GetLeftXTopYPosition();
                renderer.RenderGuiIcon(male_animal_sprite->Sprite, male_animal_icon_left_top_screen_position);

                // RENDER THE COLLECTED COUNT FOR THE MALE ANIMAL.
                // It should appear just to the right of the male animal icon.
                std::string male_collected_count_text = "x" + std::to_string(species_male_animal_collected_count);
                MATH::Vector2f male_collected_count_left_top_screen_position = male_animal_icon_left_top_screen_position;
                male_collected_count_left_top_screen_position.X += male_animal_sprite->Sprite.GetWidthInPixels();
                renderer.RenderText(male_collected_count_text, male_collected_count_left_top_screen_position);
            }
        }

        // RENDER INFORMATION ABOUT ANY COLLECTED FEMALE ANIMALS.
        bool female_animal_collected = (species_female_animal_collected_count > 0);
        if (female_animal_collected)
        {
            // RENDER AN ICON FOR THE FEMALE ANIMAL.
            // It should appear in the left-bottom corner of the box.
            OBJECTS::AnimalType female_animal_type(species, OBJECTS::AnimalGender::FEMALE);
            std::shared_ptr<GRAPHICS::AnimatedSprite> female_animal_sprite = RESOURCES::AnimalGraphics::GetSprite(female_animal_type, *Assets);
            if (female_animal_sprite)
            {
                MATH::Vector2f female_animal_icon_left_top_screen_position = box_screen_rectangle.GetLeftXBottomYPosition();
                female_animal_icon_left_top_screen_position.Y -= female_animal_sprite->Sprite.GetHeightInPixels();
                renderer.RenderGuiIcon(female_animal_sprite->Sprite, female_animal_icon_left_top_screen_position);

                // RENDER THE COLLECTED COUNT FOR THE FEMALE ANIMAL.
                // It should appear just to the right of the female animal icon.
                std::string female_collected_count_text = "x" + std::to_string(species_female_animal_collected_count);
                MATH::Vector2f female_collected_count_left_top_screen_position = female_animal_icon_left_top_screen_position;
                female_collected_count_left_top_screen_position.X += female_animal_sprite->Sprite.GetWidthInPixels();
                renderer.RenderText(female_collected_count_text, female_collected_count_left_top_screen_position);
            }
        }
    }
}
