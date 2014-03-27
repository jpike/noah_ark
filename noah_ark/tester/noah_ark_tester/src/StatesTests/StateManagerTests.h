#pragma once
#include <memory>
#include <catch.hpp>
#include "States/StateManager.h"
#include "StatesTests/MockGameState.h"

TEST_CASE( "Set current state, no previous state.", "[StateManager][GetCurrentState][SetCurrentState]" )
{
    // CREATE THE STATE MANAGER.
    STATES::StateManager stateManager;

    // VERIFY THAT NO PREVIOUS STATE IS BEING MANAGED.
    std::shared_ptr<STATES::IGameState> initialState = stateManager.GetCurrentState();
    bool noInitialState = ( NULL == initialState.get() );
    REQUIRE( noInitialState );

    // CREATE THE NEW STATE FOR THE STATE MANAGER.
    std::shared_ptr<STATES::IGameState> newState = std::shared_ptr<STATES::IGameState>(new STATES::MockGameState());
    stateManager.SetCurrentState(newState);

    // VERIFY THAT THE STATE MANAGER HAS CHANGED TO MANAGING THE NEW STATE.
    std::shared_ptr<STATES::IGameState> currentManagedState = stateManager.GetCurrentState();
    bool currentManagedStateMatchesNewlySetState = ( currentManagedState.get() == newState.get() );
    REQUIRE( currentManagedStateMatchesNewlySetState );
}

TEST_CASE( "Change state from previous state.", "[StateManager][GetCurrentState][SetCurrentState]" )
{
    // CREATE THE STATE MANAGER.
    STATES::StateManager stateManager;

    // CREATE AN INITIAL STATE FOR THE STATE MANAGER.
    std::shared_ptr<STATES::IGameState> initialState = std::shared_ptr<STATES::IGameState>(new STATES::MockGameState());
    stateManager.SetCurrentState(initialState);

    // Verify that the initial state is managed.
    std::shared_ptr<STATES::IGameState> initialManagedState = stateManager.GetCurrentState();
    bool initialStateIsManaged = ( initialManagedState.get() == initialState.get() );
    REQUIRE( initialStateIsManaged );

    // CREATE A NEW STATE FOR THE STATE MANAGER.
    std::shared_ptr<STATES::IGameState> newState = std::shared_ptr<STATES::IGameState>(new STATES::MockGameState());
    stateManager.SetCurrentState(newState);

    // VERIFY THAT THE STATE HAS CHANGED.
    std::shared_ptr<STATES::IGameState> currentManagedState = stateManager.GetCurrentState();

    // Verify the initial state is no longer managed.
    bool initialStateNoLongerManaged = ( initialState.get() != currentManagedState.get() );
    REQUIRE( initialStateNoLongerManaged );

    // Verify the new state is managed.
    bool newStateIsManaged = ( newState.get() == currentManagedState.get() );
    REQUIRE( newStateIsManaged );
}