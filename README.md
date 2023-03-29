# Apostle Engine
The Apostle Engine is an in-development 2D/3D game engine.




# Known Issues
- When cloning the repository from Github, you must add " #include "APpch.h" " to the list of includes in the imgui_impl_glfw.cpp file. This can be found at the following filepath: Apostle/vendor/imgui/backends

- When building for the first time or from a clean build, you need to include 'imgui_impl_opengl3.cpp' into the project. In Visual Studio, I do this through the solution explorer by finding the file, right clicking on it, and pressing 'Include in project'. Fix should be as simple as including it through the premake file, but as it's not a pressing matter I'm leaving it for a later date, or until it becomes relevant.

- Some key presses are showing up as the wrong keys when printing to the console; generally 'modifier' keys. 