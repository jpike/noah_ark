# BIBLE GAMES - NOAH'S ARK

This in-development game is based on the Biblical story of Noah's Ark.  The goal is to provide a fun way to become more familiar with and learn more about the story of Noah's Ark.

The game will be a native Windows game, running at least on Windows 8 (x64).  Currently, the game is still in the early stages of development.

## Gameplay

Gameplay details are still being fleshed out, but some preliminary details are provided in a PowerPoint presentation in the [documentation/design folder](noah_ark/documentation/design).  Overall, the game will feature a 2D overhead view with a tile map and involve collecting things.

## Programming

The game is being programmed using C++ within a Visual Studio 2019 environment.  Doxygen is used to document the code.

The game is designed for command line builds using various .bat files and also opening as a folder in Visual Studio.

Several dependencies are used (see ThirdParty folder):
* [SFML 2.5.1](http://sfml-dev.org/) - The main library for windowing, graphics, and input. ([zlib/libpng license](http://sfml-dev.org/license.php)).
* [Catch](https://github.com/philsquared/Catch) - Unit testing ([Boost Software license](https://github.com/philsquared/Catch/blob/master/LICENSE_1_0.txt)).

The dependencies described above are included in the ThirdParty folder.

** Note that a custom build of the SFML libraries is used for linking.  This is because at the current time, no Visual Studio 2019 pre-built libraries exist.  And even if they did, the pre-built libraries tend to not support full static linking of the Visual C++ runtime, so a custom build was needed.

## Graphics

The graphics plan to use a "Game Boy Color" style for the overall shape of sprites, but a wider color palette may be used.

## Bugs/Want to Help?

If you find any bugs (including build issues), let me know.  If you'd like to contribute in some way (programming, art, music, etc.), contact me as well.
