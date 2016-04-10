#pragma once

#include "Objects/Noah.h"

namespace OBJECTS
{
    // The speed is chosen just based on what has felt right so far.
    const float Noah::MOVE_SPEED_IN_PIXELS_PER_SECOND = 64.0f;

    MATH::Vector2f Noah::GetWorldPosition() const
    {
        MATH::Vector2f world_position = Sprite.GetWorldPosition();
        return world_position;
    }

    void Noah::SetWorldPosition(const MATH::Vector2f& world_position)
    {
        Sprite.SetWorldPosition(world_position.X, world_position.Y);
    }

    MATH::FloatRectangle Noah::GetWorldBoundingBox() const
    {
        MATH::FloatRectangle bounding_box = Sprite.GetWorldBoundingBox();
        return bounding_box;
    }

    std::shared_ptr<EVENTS::AxeSwingEvent> Noah::SwingAxe() const
    {
        // CHECK IF NOAH HAS AN AXE.
        bool axe_exists = (nullptr != Inventory.Axe);
        if (!axe_exists)
        {
            // An axe can't be swung since it isn't equipped.
            return nullptr;
        }

        // CHECK IF THE AXE IS ALREADY BEING SWUNG.
        // The prevent the axe from being swung too quickly,
        // it should not be possible to swing it again
        // if it is already being swung.
        bool axe_already_swinging = Inventory.Axe->IsSwinging();
        if (axe_already_swinging)
        {
            // The axe is already being swung,
            // so no new event needs to be created.
            return nullptr;
        }

        // SWING THE AXE IN THE SAME DIRECTION NOAH IS FACING.
        switch (FacingDirection)
        {
            case CORE::Direction::UP:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                float noah_half_width = GetWorldBoundingBox().GetWidth() / 2.0f;
                // The position is slightly tweaked solely based on what looks best.
                axe_position.X += noah_half_width;
                axe_position.X -= 1.5f;
                axe_position.Y += 3;
                Inventory.Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Inventory.Axe->SwingUp();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = std::make_shared<EVENTS::AxeSwingEvent>(Inventory.Axe);
                return axe_swing;
            }
            case CORE::Direction::DOWN:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The offset from Noah's position is calculated
                // to make the axe appear to come from Noah's hand.
                MATH::Vector2f axe_position = GetWorldPosition();
                float noah_half_width = GetWorldBoundingBox().GetWidth() / 2.0f;
                // The position is slightly tweaked solely based on what looks best.
                axe_position.X -= noah_half_width;
                axe_position.X += 2;
                axe_position.Y += 2;
                Inventory.Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Inventory.Axe->SwingDown();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = std::make_shared<EVENTS::AxeSwingEvent>(Inventory.Axe);
                return axe_swing;
            }
            case CORE::Direction::LEFT:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                // The position is slightly tweaked solely based on what looks best.
                axe_position.X += 1;
                axe_position.Y += 6;
                Inventory.Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Inventory.Axe->SwingLeft();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = std::make_shared<EVENTS::AxeSwingEvent>(Inventory.Axe);
                return axe_swing;
            }
            case CORE::Direction::RIGHT:
            {
                // CALCULATE THE POSITION FOR THE AXE.
                // The axe should appear slightly in front of Noah.
                MATH::Vector2f axe_position = GetWorldPosition();
                // The position is slightly tweaked solely based on what looks best.
                axe_position.X -= 1;
                axe_position.Y += 6;
                Inventory.Axe->SetWorldPosition(axe_position);

                // START SWINGING THE AXE.
                Inventory.Axe->SwingRight();

                // CREATE THE AXE SWING EVENT.
                std::shared_ptr<EVENTS::AxeSwingEvent> axe_swing = std::make_shared<EVENTS::AxeSwingEvent>(Inventory.Axe);
                return axe_swing;
            }
            default:
                // Don't swing an axe since a valid direction isn't known.
                return nullptr;
        }
    }
}
