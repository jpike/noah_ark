#pragma once

#include <memory>
#include "Graphics/GraphicsSystem.h"
#include "Resources/Assets.h"
#include "States/IGameState.h"
#include "World/GroundLayer.h"
#include "World/WorldArea.h"

namespace STATES
{
    class WorldAreaTestState : public IGameState
    {
    public:
        /// Constructor.
        explicit WorldAreaTestState(
            std::shared_ptr<GRAPHICS::GraphicsSystem>& graphics_system);
        /// Destructor.
        virtual ~WorldAreaTestState();

        /// @copydoc    IGameState::Update
        virtual bool Update(const float elapsed_time_in_seconds);

    private:
        RESOURCES::Assets graphics_assets;
        std::shared_ptr<GRAPHICS::GraphicsSystem> GraphicsSystem;
        std::shared_ptr<WORLD::GroundLayer> Ground;

        std::shared_ptr<WORLD::WorldArea> WorldArea;
    };
}