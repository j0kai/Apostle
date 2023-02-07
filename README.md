# Apostle
Apostle Engine



# Known Issues
- Currently do not have a way to track imgui premake file, so it needs to be manually created when cloning the repository from Github. See section "imgui Premake" for basic template.





#Imgui Premake
- The following is the base template for the imgui premake file and is enough to get the engine up and running:

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


