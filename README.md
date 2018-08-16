# Vin

Cross platform OpenGL-based text editor written in C++.

## Building

1. Run `git submodule update --init --recursive` to download GLFW.

2. Install FreeType (TODO).

3. Use CMake to build the library. On Unix-based systems, this would be:
```
mkdir build && cd build
cmake ..
make
```
