# BIBLE GAMES - NOAH'S ARK

This in-development game is based on the Biblical story of Noah's Ark.  The goal is to provide a fun way to become more familiar with and learn more about the story of Noah's Ark.

The game will be a native Windows game, running at least on Windows 8 (x64).  Currently, the game is still in the early stages of development.

## Gameplay

Gameplay details are still being fleshed out, but some preliminary details are provided in a PowerPoint presentation in the [documentation/design folder](noah_ark/documentation/design).  Overall, the game will feature a 2D overhead view with a tile map and involve collecting things.

## Programming

The game is being programmed using C++ within a Visual Studio 2017 environment.  Doxygen is used to document the code.

Several projects exist within the Visual Studio solution:
* noah_ark_game - The main game.  Uses the library to orchestrate running an actual game.
* noah_ark_library - A library containing the bulk of the code for the game.  This library opens up the possibility for unit testing.
* noah_art_tester - A unit testing application intended for testing the library.
* noah_ark_resources - A project to manage any non-code resources of the project (graphics, tile maps, audio, etc.).
* noah_ark_documentation - A project to hold various documentation files.

Several dependencies are used:
* [SFML 2.4.2](http://sfml-dev.org/) - The main library for windowing, graphics, and input. ([zlib/libpng license](http://sfml-dev.org/license.php)).
* [Catch](https://github.com/philsquared/Catch) - Unit testing ([Boost Software license](https://github.com/philsquared/Catch/blob/master/LICENSE_1_0.txt)).

The dependencies described above are not included with this project and must be downloaded externally.  To build the solution, you'll need to have environment variables set to the following directories:

* SFML_64_BIT_DIR - Set to the root directory for the 64-bit (x64) version of SFML (with include, lib, and bin directories).
** Note that a custom build of the SFML libraries is used for linking.  This is because at the current time, no Visual Studio 2017 pre-built libraries exist.  And even if they did, the pre-built libraries tend to not support full static linking of the Visual C++ runtime, so a custom build was needed.
* CATCH_DIR - Set to the directory with Catch's catch.hpp file.

## Graphics

The graphics plan to use a "Game Boy Color" style for the overall shape of sprites, but a wider color palette may be used.

## Audio

Audio hasn't been planned yet.  However, the audio will likely be simple to match the graphical style.

## Tile Maps

[Tiled](http://www.mapeditor.org/) is used to make tile maps for the game.  This editor was chosen for the following reasons:
* Relatively up-to-date with common features
* Relatively up-to-date parsing libraries
* Output formats that would allow easier creation of a custom parsing library if need

## Bugs/Want to Help?

If you find any bugs (including build issues), let me know.  If you'd like to contribute in some way (programming, art, music, etc.), contact me as well.
