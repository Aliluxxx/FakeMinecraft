# FakeMinecraft
Just another minecraft clone

## Download instructions

To clone the repository use the following git command:
`git clone --recursive https://github.com/Aliluxxx/FakeMinecraft FakeMinecraft`.

If the repository was cloned non-recursively previously, type `git submodule update --init` to clone the necessary submodules.

## Build instructions
To generate project files, you can run the script files inside the `build` folder or execute manually the commands down below. This project uses [premake5](https://premake.github.io/) to generate configuration files.
There are 2 possible configurations that can be generated:

### GUI
The GUI version of the game. It will be used for the client side.

#### Windows
Inside `build\windows`, run the command:
- `premake5.exe --file=build.lua vs2022` or run the script `build-vs2022.bat` to generate Visual Studio 2022 projects
- `premake5.exe --file=build.lua gmake2` or run the script `build-gmake.bat` to generate Makefiles projects

### Headless
The headless verion of the game. It will not create a window, so it will be used for the server side.

#### Windows
Inside `build\windows`, run the command:
- `premake5.exe --file=build-headless.lua vs2022` or run the script `build-headless-vs2022.bat` to generate Visual Studio 2022 projects
- `premake5.exe --file=build-headless.lua gmake2` or run the script `build-headless-gmake.bat` to generate Makefiles projects

## Documentation
All the project documentation can be found in the `docs` folder

## External libraries
- [GLFW](https://github.com/glfw/glfw) for the window handling
- [ENet](https://github.com/zpl-c/enet) for reliable UDP networking
- [GLM](https://github.com/icaven/glm) for the math
- [spdlog](https://github.com/gabime/spdlog) for the logging system
- [Native File Dialog Extended](https://github.com/btzy/nativefiledialog-extended) for the native file dialog (duh)
