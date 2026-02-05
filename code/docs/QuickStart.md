## Usage
A playable build of the game is located in `root/builds`. Instructions on how to play the game
can be found ingame or [here](wiki/game.md).
To compile the source code clone the repository.
**This project has several external dependencies which are embedded as submodules**.
To minimize the risk of compilation issues use:
```bash
git clone --recurse-submodules https://gitlab2.informatik.uni-wuerzburg.de/GE/Teaching/gl3/projects/2025/51-gl3-kalb.git
```


To compile the source code use the [Visual Studio 2022](https://visualstudio.microsoft.com/vs/features/cplusplus/) compiler and [CMake](https://cmake.org/) **version 3.29** or higher. Select the `root/code` folder as the project root folder in your IDE.
> [!CAUTION]
> **MSVC Compiler is required!**
> This project uses an implementation of the SoLoud library, that **requires** the MSVC compiler. Should you encounter any problems, make sure it is properly set up. (In [CLion](https://www.jetbrains.com/clion/) make sure it is the first entry in the list of compilers in
>  `Settings / Build, Execution, Deployment / Toolchains`)

Should you be interested in creating your own game from scratch it is **NECESSARY** to keep the general structure of the project.
Just delete the content of the game folder and adjust the `CMakeFile.txt` in the directory.
Inclusion of all content in the `engine/` and `extern/` subdirectory are mandatory, as is the existence of the `assets/` directory.

Further information on how to create a new game can be found [here](wiki/engine.md).

