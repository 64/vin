# Vin

Cross platform OpenGL-based text editor written in C++.

## Building

1. Run `git submodule update --init --recursive` to download GLFW.

2. Install FreeType. For Linux and OSX, use your package manager. For Windows, download Freetype [here](https://github.com/ubawurinna/freetype-windows-binaries/releases) and in the step below invoke cmake with `cmake -DFREETYPE_DIR="C:\path\to\freetype" ..` instead.

3. Use CMake to build the library. On Unix-based systems, this would be:
```
mkdir build && cd build
cmake ..
make
```
