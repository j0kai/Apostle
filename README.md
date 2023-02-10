# Apostle Engine
The Apostle Engine is an in-development 2D/3D game engine.



# Known Issues
- Currently do not have a way to track imgui premake file, so it needs to be manually created when cloning the repository from Github. See "Imgui Premake" section for basic template.


# Imgui Premake
- This is the base template for the imgui premake file and is enough to get the engine up and running. Create a premake5.lua file in Apostle/vendor/imgui, and paste in the following:

project "imgui"
  kind "StaticLib"
  language "C++"

  files
  {
    "imconfig.h",
    "imgui.cpp",
    "imgui.h",
    "imgui_demo.cpp",
    "imgui_draw.cpp",
    "imgui_internal.h",
    "imgui_tables.cpp",
    "imgui_widgets.cpp",
    "imstb_rectpack.h",
    "imstb_textedit.h",
    "imstb_truetype.h"
  }

  filter "system:windows"
    systemversion "latest"
    staticruntime "on"

  filter {"system:windows", "configurations:Release"}
    runtime "Release"
    optimize "On"


