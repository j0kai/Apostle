# Apostle Engine
The Apostle Engine is a 2D/3D game engine in the early stages of development. The engine currently doesn't support platforms other than Windows and is exclusively using OpenGL as it's graphics API, as I figured that it would be the easiest to get up-and-running.

# Getting Started

Visual Studio 2022 is recommended (however, VS2017 and onwards ___should___ work); other development environments have not been tested due to me wanting to get Apostle running on a Windows build initially.

Furthermore, due to the way that the project is currently set up, there are a few steps you need to follow to get the project up and running:

1) Open the 'scripts' folder and run ___Win-GenerateProjects.bat___ and then open Apostle.sln

2) And with that you're ready to go! (NOTE: Hit F5 to see the example.)


# Major Features

- Blazingly fast 2D Rendering
- Physically-Based 3D Rendering in the future (focusing on 2D for now so that users can make games with the engine)
- A visual editor
- Support for other platforms (Mac, Linux, Android, and iOS) - for now only Windows is supported
- Support for Vulkan, DirectX, Metal, etc. - only supports OpenGL as of now
- Ability to create scripts and easily integrate them into your project(s)
- An audio system

(NOTE: Most of the above features are not implemented as of yet, but I intend to get most implemented by the end of 2024.)

# Known Issues

- Some key presses are showing up as the wrong keys when printing to the console; generally 'modifier' keys (shift, ctrl, etc.)

# Showcase Images
Below are images showcasing the editor application:

![devenv_XoGHKzVjZT](https://github.com/j0kai/Apostle/assets/105503076/2b600907-c043-4c99-8c2d-6ddd22fb2c9d)
