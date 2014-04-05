#include "States/StateManager.h"

using namespace STATES;

StateManager::StateManager() :
    m_currentState()
{
    // Nothing else to do.
}

StateManager::~StateManager()
{
    // Memory for all members will be automatically freed.
}

std::shared_ptr<IGameState> StateManager::GetCurrentState() const
{
    return m_currentState;
}

void StateManager::SetCurrentState(const std::shared_ptr<IGameState>& newState)
{
    m_currentState = newState;
}

bool StateManager::Update()
{
    return m_currentState->Update();
}