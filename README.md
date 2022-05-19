# imgui-sdl-glad-template

A template project using [ImGui](https://github.com/ocornut/imgui) (docking branch) with [SDL2](https://www.libsdl.org) and [GLAD](https://glad.dav1d.de/).

Compiles using **CMake** + **CMake Tools** through **Visual Studio Code**. (`Ctrl+Shift+B` to build after running `CMake: Configure` for initial setup)

> Make sure to update the compilerPath setting in `.vscode/c_cpp_properties.json` to a valid path on your system

The output binary and required files will be in `./build/<TARGET>/bin/` (These all need to be manually specified on the CMake build script)
