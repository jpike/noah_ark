#include "States/GameState.h"

namespace std
{
    /// An overload of std::to_string to work for game state enum values.
    /// @return The string value corresponding to the provided game state.
    string to_string(const STATES::GameState game_state)
    {
        switch (game_state)
        {
            case STATES::GameState::INTRO_SEQUENCE:
                return "INTRO_SEQUENCE";
            case STATES::GameState::TITLE_SCREEN:
                return "TITLE_SCREEN";
            case STATES::GameState::CREDITS_SCREEN:
                return "CREDITS_SCREEN";
            case STATES::GameState::GAME_SELECTION_SCREEN:
                return "GAME_SELECTION_SCREEN";
            case STATES::GameState::NEW_GAME_INTRO_SEQUENCE:
                return "NEW_GAME_INTRO_SEQUENCE";
            case STATES::GameState::NEW_GAME_INSTRUCTION_SEQUENCE:
                return "NEW_GAME_INSTRUCTION_SEQUENCE";
            case STATES::GameState::PRE_FLOOD_GAMEPLAY:
                return "PRE_FLOOD_GAMEPLAY";
            case STATES::GameState::FLOOD_CUTSCENE:
                return "FLOOD_CUTSCENE";
            case STATES::GameState::DURING_FLOOD_GAMEPLAY:
                return "DURING_FLOOD_GAMEPLAY";
            case STATES::GameState::POST_FLOOD_GAMEPLAY:
                return "POST_FLOOD_GAMEPLAY";
            case STATES::GameState::ENDING_CREDITS_SCREEN:
                return "ENDING_CREDITS_SCREEN";
            default:
                return "GameState::INVALID (" + std::to_string(static_cast<int>(game_state)) + ")";
        }
    }
}
