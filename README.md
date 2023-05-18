# Apostle Engine
The Apostle Engine is a 2D/3D game engine in the early stages of development. The engine currently doesn't support platforms other than Windows and is exclusively using OpenGL as it's graphics API, as I figured that it would be the easiest to get up-and-running.

# Getting Started

Visual Studio 2022 is recommended (however, VS2017 and onwards _should_ work), as other development environments have not been tested due to me wanting to get Apostle running on a Windows build initially.

Furthermore, due to the way that the project is currently set up, there are a few steps you need to follow to get the project up and running:

1) Navigate to Apostle/vendor/imgui/backends and find the following two files:

	  imgui_impl_opengl3.cpp - add __#include "APpch.h"__ and __#include <glad/glad.h>__ to the top of the include list
	  imgui_impl_glfw.cpp - add __#include "APpch.h"__ to the top of the include list

2) Navigate to Apostle/vendor/GLFW/premake5.lua and open it, then change the staticruntime from "off" to "on".

3) Run GenerateProjects.bat in the root folder and open Apostle.sln.

4) And with that you're ready to go! (NOTE: Hit F5 to see the example.)


# Features to come

- Blazingly fast 2D Rendering
- Physically-Based 3D Rendering in the future (focusing on 2D for now so that users can make games with the engine)
- A visual editor
- Support for other platforms (Mac, Linux, Android, and iOS) - for now only Windows is supported
- Support for Vulkan, DirectX, Metal, etc. - only supports OpenGL as of now
- Ability to create scripts and easily integrate them into your project(s)
- An audio system


# Known Issues
- As explained in the Getting Started section, there are issues with the includes in some files that are used. Unsure how I could fix this without making things unnecessarily complicated as of right now.

- Some key presses are showing up as the wrong keys when printing to the console; generally 'modifier' keys (shift, ctrl, etc.)