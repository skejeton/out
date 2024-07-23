# Running the game

Download the latest release from [https://skejeton.itch.io/out](https://skejeton.itch.io/out) if you don't want to build the game

# Building the game

## On Linux

The game is cross compilable on linux

**To compile to web-assembly** make sure you have emscripten installed, then run `make wasm`

**To compile to windows** make sure you have `x86_64-w64-mingw32-gcc` installed, also, you will need an SDL2.dll file to be able to run this game, it has to be near the binary.

**To compile to linux** make sure you have SDL2 installed, after that, run `make linux`, convenience function for running exists `make run`, but so far it only works for linux

## On Windows

Install MSVC, use `build.bat` to compile. You need `%LibSDL2MSVC%` environment variable defined, pointing to the SDL2 directory you downloaded from latest SDL2 release. Note that in `%LibSDL2MSVC%\include` you need to move all `.h` files into another `SDL2` directory, so that the include path is `%LibSDL2MSVC%\include\SDL2`. The build script will automatically generate a `compile_flags.txt` file for your intellisense.

## On MacOS

TODO, I'd appreciate if someone would do a pull request on that ;)

But to be fair the make file might work on mac-os, you can probably even run `make linux` to build a mac-os version.

# Rights on assets

Use for any purpose
