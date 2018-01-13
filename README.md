# SFXR Qt

![Screenshot](screenshot.png)

This a QtQuick Controls 2 port of [SFXR][]. SFXR is a sound effect generator
created by DrPetter to quickly produce retro-sounding sound effects for games.

This project has the same features as the original SFXR, with a more modern
user interface and, dare I say, a more modular code base.

## Usage

Start the application and click one of the generator buttons. Then play with
the sliders to achieve the sound you want.

For more details, you should read the original [SFXR
readme.txt](readme-sfxr-sdl.txt).

## Building

Building SFXR Qt requires the following:

- Qt 5.7
- SDL 1.2
- Python 3
- CMake 3.7 (might work with older versions if you adjust the minimum version in
  the `CMakeLists.txt` file)

Get the `qpropgen` Git submodule with:

    git submodule update --init

Then create a build directory:

    mkdir build

Finally, build and install:

    cd build
    cmake ..
    ninja
    sudo ninja install

## License

MIT

[SFXR]: http://www.drpetter.se/project_sfxr.html
