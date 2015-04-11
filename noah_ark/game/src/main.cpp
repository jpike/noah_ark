#include <iostream>
#include "NoahArkGame.h"

/// @brief      The main entry point for the game.
///             Runs the Noah's Ark game until the user
///             chooses to exit or an error occurs.
/// @param[in]  argumentCount - The number of command line arguments.
/// @param[in]  arguments - The command line arguments.
/// @return     One of the exit codes defined in NoahArkGame.
int main(int argumentCount, char* arguments[])
{
    // RUN THE NOAH'S ARK GAME.
    int exitCode = NoahArkGame::RunGame();
    
    return exitCode;
}