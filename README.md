# Vin

Cross platform OpenGL-based text editor written in C++.

## Building

1. Run `git submodule update --init --recursive` from the project root to download the dependencies.

2. Install FreeType. For Linux and OSX, use your package manager. For Windows, you don't need to do anything.

3. Use CMake to build the library. On Unix-based systems, this would be:
```
mkdir build && cd build
cmake ..
make
```
