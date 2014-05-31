#include "States/OverworldState.h"

using namespace STATES;

OverworldState::OverworldState(std::shared_ptr<GRAPHICS::GraphicsSystem>& graphicsSystem) :
    m_graphicsSystem(graphicsSystem),
    m_overworldMap()
{

}

OverworldState::~OverworldState()
{

}

bool OverworldState::Update()
{
    return false;
}