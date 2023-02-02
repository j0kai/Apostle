workspace "Apostle"
    architecture "x64"
    configurations 
    { 
      "Debug", 
      "Release", 
      "Dist"
    }

    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root
IncludeDir = {}
IncludeDir["GLFW"] = "Apostle/vendor/GLFW/include"
IncludeDir["Glad"] = "Apostle/vendor/Glad/include"
IncludeDir["ImGui"] = "Apostle/vendor/imgui"

group "Dependencies"
    include "Apostle/vendor/GLFW"
    include "Apostle/vendor/Glad"
    include "Apostle/vendor/imgui"

group""

project "Apostle"
    location "Apostle"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "APpch.h"
    pchsource "Apostle/src/APpch.cpp"

    files 
    { 
      "%{prj.name}/src/**.h", 
      "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
      "%{prj.name}/src",
      "%{prj.name}/vendor/spdlog/include",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.Glad}",
      "%{IncludeDir.ImGui}"
    }

    links
    {
      "GLFW",
      "Glad",
      "ImGui",
      "opengl32.lib"
    }
    
    filter "system:windows"
      cppdialect "C++17"
      systemversion "latest"

      defines
      {
        "AP_PLATFORM_WINDOWS",
        "AP_BUILD_DLL",
        "GLFW_INCLUDE_NONE",
        "IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
      }
      
      postbuildcommands
      {
        ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir   .. "/Sandbox/\"")
      }

    filter "configurations:Debug"
      defines { "AP_DEBUG" }
      runtime "Debug"
      symbols "On"

    filter "configurations:Release"
      defines { "AP_RELEASE" }
      runtime "Release"
      optimize "On"

    filter "configurations:Dist"
      defines { "AP_DIST" }
      runtime "Release"
      optimize "On"

    filter {"system:windows", "configurations:Release"}
      buildoptions "/MD"


--SANDBOX PROJECT--
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
  
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
    files 
    { 
      "%{prj.name}/src/**.h", 
      "%{prj.name}/src/**.cpp"
    }
  
    includedirs
    {
      "Apostle/vendor/spdlog/include",
      "Apostle/src"
    }

    links
    {
      "Apostle"
    }
      
    filter "system:windows"
      cppdialect "C++17"
      systemversion "latest"
  
      defines
      {
        "AP_PLATFORM_WINDOWS",
      }
  
      filter "configurations:Debug"
        defines { "AP_DEBUG" }
        runtime "Debug"
        symbols "On"
  
      filter "configurations:Release"
        defines { "AP_RELEASE" }
        runtime "Release"
        optimize "On"
  
      filter "configurations:Dist"
        defines { "AP_DIST" }
        runtime "Release"
        optimize "On"