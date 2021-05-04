# Running the game

Download the latest release from [https://iotasieve.itch.io/out](https://iotasieve.itch.io/out) if you don't want to build the game

# Building the game

## Linux

The game is cross compilable on linux

To compile to web-assembly make sure you have emscripten installed, then run `make wasm`

To compile to windows make sure you have `x86_64-w64-mingw32-gcc` installed, also, you will need an SDL2.dll file to be able to run this game, it has to be near the binary.

To compile to linux make sure you have SDL2 installed, after that, run `make linux`, convenience function for running exists `make run`, but so far it only works for linux

## Windows

TODO, I'd appreciate if someone would do a pull request on that ;)

## MacOS

TODO, I'd appreciate if someone would do a pull request on that ;)

But to be fair the make file might work on mac-os, you can probably even run `make linux` to build a mac-os version.

# Rights on assets

Use for any purpose
