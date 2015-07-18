#include <iostream>
#include "Input/KeyboardInputController.h"
#include "States/AxeTestState.h"

namespace STATES
{
    AxeTestState::AxeTestState(
        std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system) :
    GraphicsAssets(),
    GraphicsSystem(graphics_system),
    Axe()
    {
        Axe = OBJECTS::Axe::Create(GraphicsAssets);

        MATH::Vector2f position;
        position.X = static_cast<float>(GraphicsSystem->GetRenderTarget()->getSize().x) / 2.0f;
        position.Y = static_cast<float>(GraphicsSystem->GetRenderTarget()->getSize().y) / 2.0f;
        Axe->SetWorldPosition(position);

        GraphicsSystem->AddGraphicsComponent(
            GRAPHICS::GraphicsLayer::PLAYER,
            Axe->GetSprite());
    }
    
    AxeTestState::~AxeTestState()
    {}

    bool AxeTestState::Update(const float elapsed_time_in_seconds)
    {
        static bool axe_visible = false;
        static float total_elapsed_time_for_swing = 0;
        static bool first_frame_of_swing = false;
        static unsigned int frame_index = 0;
        
        // USER INPUT.
        INPUT_CONTROL::KeyboardInputController input_controller;
        if (input_controller.UpButtonPressed())
        {
            if (!Axe->IsSwinging())
            {
                Axe->SwingUp();
            }
        }
        else if (input_controller.DownButtonPressed())
        {
            if (!Axe->IsSwinging())
            {
                Axe->SwingDown();
            }
        }
        else if (input_controller.LeftButtonPressed())
        {
            if (!Axe->IsSwinging())
            {
                Axe->SwingLeft();
            }
        }
        else if (input_controller.RightButtonPressed())
        {
            if (!Axe->IsSwinging())
            {
                Axe->SwingRight();
            }
        }
        
        // ANIMATION.
        Axe->Update(elapsed_time_in_seconds);
        GraphicsSystem->Update(elapsed_time_in_seconds);

        // RENDER.
        /*if (Axe->IsSwinging())
        {
            auto render_target = GraphicsSystem->GetRenderTarget();
            //render_target->draw(*AxeSprite);
            Axe->GetSprite()->Render(*render_target);
        }*/

        return false;
    }
}