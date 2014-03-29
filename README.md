# BIBLE GAMES - NOAH'S ARK

This in-development game is based on the Biblical story of Noah's Ark.  The goal is to provide a fun way to become more familiar with and learn more about the story of Noah's Ark.

The game will be a native Windows game, running at least on Windows 7.  Currently, the game is in the earliest stages of development, with little more than an empty window appearing when run.

## Gameplay

Gameplay details are still being fleshed out, but some preliminary details are provided in a PowerPoint presentation in the [documentation/design folder](noah_ark/documentation/design).  Overall, the game will feature a 2D overhead view with a tile map and involve collecting things.

## Programming

The game is being programmed using C++ and [Haaf's Game Engine 1.81](http://hge.relishgames.com) within a Visual Studio 2012 Express environment.  [Catch](https://github.com/philsquared/Catch) is being used for unit testing.

For projects exist within the Visual Studio solution:
* noah_ark_game - The main game.  Uses the library to orchestrate running an actual game.
* noah_ark_library - A library containing the bulk of the code for the game.  This library opens up the possibility for unit testing.
* noah_art_tester - A unit testing application intended for testing the library.
* noah_ark_resources - A project to manage any non-code resources of the project (graphics, tile maps, audio, etc.).

The dependencies described above are not included with this project and must be downloaded externally.  To build the solution, you'll need to have environment variables set to the following directories:

* HGE_DIR - Set to the root directory for Haaf's Game Engine (with include and lib directories and hge.dll).
* CATCH_DIR - Set to the directory with Catch's catch.hpp file.

Haaf's Game Engine is distributed under the [zlib/libpng license](http://hge.relishgames.com/license.html), and Catch is distributed under the [Boost Software license](https://github.com/philsquared/Catch/blob/master/LICENSE_1_0.txt).

Doxygen is used to document the code.

## Graphics

The graphics plan to use a "Game Boy Color" style for the overall shape of sprites, but a wider color palette may be used.

## Audio

Audio hasn't been planned yet.  However, the audio will likely be simple to match the graphical style.

## Bugs/Want to Help?

If you find any bugs (including build issues), let me know.  If you'd like to contribute in some way (programming, art, music, etc.), contact me as well.
