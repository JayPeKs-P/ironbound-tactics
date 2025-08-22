# Overview

| General Info       |                                                                  |
| ------------------ | ---------------------------------------------------------------- |
| Final Title        | `Ironbound Tactics`                                              |
| Student            | `Julian Kalb`, julian.kalb@stud-mail.uni-wuerzburg.de, `s402363` |
| Target Platform(s) | `Windows`                                                        |
| Start Date         | `29.10.2024`                                                     |
| Study Program      | `Games Engineering B.Sc.`                                        |
| Language           | `C++`                                                            |

## Abstract
As part of the module *GameLab III*, which is one of the core modules of the bachelors degree *Games Engineering* at Julius-Maximilians-University Würzburg, students are tasked to develop a videogame in *C++* without using existing engines or engine-like
libraries.
`Ironbound Tactics` is the game I create in course of this module. It is a turn based endless battler, where the player creates an army by picking their units out of a set pool and then fights the armies of random computer controlled opponents.
***This project is still Work In Progress***

## Project Structure

```
RepositoryRoot/
    ├── README.md           
    │                       
    ├── builds/             // Archive (.zip) of built executable
    │                        
    ├── code/               // project root
    │   ├── assets/         // Shaders, Textures, JSON files, etc.
    │   │                                              
    │   ├── engine/         // Source code of the "engine"
    │   │                                              
    │   ├── extern/         // All external dependencies of the project
    │   │                                              
    │   ├── game/           // Source code of Ironbound Tactics
    │   │                                              
    │   └── CMakeLists.txt  
    │                       
    ├── documentation/      
    │                       
    ├── poster/             
    │                       
    ├── report/             // Forthcoming
    │                       
    └── trailer/            // Forthcoming
```
## Usage
A playable build of the game is located in `root/builds`.  To compile the source code clone the repository. **This project has several external dependencies which are embedded as submodules**. To minimize the risk of compilation issues use:
```bash
git clone --recurse-submodules https://github.com/JayPeKs-P/ironbound-tactics
```


To compile the source code use the [Visual Studio 2022](https://visualstudio.microsoft.com/vs/features/cplusplus/) compiler and [CMake](https://cmake.org/) **version 3.29** or higher. Select the `root/code` folder as the project root folder in your IDE.
> [!CAUTION] 
> **MSVC Compiler is required!** 
> This project uses an implementation of the SoLoud library, that **requires** the MSVC compiler. Should you encounter any problems, make sure it is properly set up. (In [CLion](https://www.jetbrains.com/clion/) make sure it is the first entry in the list of compilers in
>  `Settings / Build, Execution, Deployment / Toolchains`)

## External Dependencies

| **Library**                                                       | **Explanation**                                                                               |
| ----------------------------------------------------------------- | --------------------------------------------------------------------------------------------- |
| [GLAD](https://glad.dav1d.de/)                                    | Version 4.6 - Retrieves OpenGL functions                                                      |
| [GLFW](https://github.com/glfw/glfw)                              | API for creating windows, contexts and surfaces, reading inputs, etc.                         |
| [GLM](https://github.com/g-truc/glm)                              | Header only mathematics library for graphics software based on OpenGL Shading Language (GLSL) |
| [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear)           | Toolkit used for graphical user interface. This project uses the glfw_3 integration           |
| [STB_Image](https://github.com/nothings/stb)                      | Part of the STB library. Used to load .png textures                                           |
| [SoLoud](https://github.com/jarikomppa/soloud?tab=readme-ov-file) | Sound effect and background music integration                                                 |
| [nlohmann_json](https://github.com/nlohmann/json)                 | For .json integration                                                                         |
| [spdlog](https://github.com/gabime/spdlog)                 | For better logging                                                                         |
### Assets
- [Tactical RPG Overworld Pack](https://www.gamedevmarket.net/asset/tactical-rpg-overworld-pack) for units and world textures
- [Fantasy RPG UI pack](https://www.gamedevmarket.net/asset/rpg-ui-pack) for GUI

## Upcoming Features
- [ ] Sound effects
- [ ] Projectile Animations
- [ ] Rework of attack animations
- [ ] Tutorial
- [ ] More unit types
	- [ ] Assault Cover to protect units from incoming range attacks
	- [ ] Cavalry for faster melee attacks
- [ ] Refine enemy AI
- [ ] Refine damage calculation
## Documentation
An explanation on how to play `Ironbound Tactics` is found in the [docs](https://JayPeKs-P.github.io/ironbound-tactics/), 
<del>as well as a step-by-step guide on how to use the engine codebase to create a new game</del>.
