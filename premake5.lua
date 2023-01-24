workspace "Apostle"
    architecture "x64"
    configurations 
    { 
      "Debug", 
      "Release", 
      "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root
IncludeDir = {}
IncludeDir["GLFW"] = "Apostle/vendor/GLFW/include"

include "Apostle/vendor/GLFW"

project "Apostle"
    location "Apostle"
    kind "SharedLib"
    language "C++"

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
      "%{IncludeDir.GLFW}"
    }

    links
    {
      "GLFW",
      "opengl32.lib"
    }
    
    filter "system:windows"
      cppdialect "C++17"
      staticruntime "On"
      systemversion "latest"

      defines
      {
        "AP_PLATFORM_WINDOWS",
        "AP_BUILD_DLL"
      }
      
      postbuildcommands
      {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir   .. "/Sandbox")
      }

    filter "configurations:Debug"
      defines { "AP_DEBUG" }
      symbols "On"

    filter "configurations:Release"
      defines { "AP_RELEASE" }
      optimize "On"

    filter "configurations:Dist"
      defines { "AP_DIST" }
      optimize "On"

    filter {"system:windows", "configurations:Release"}
      buildoptions "/MT"


--SANDBOX PROJECT--
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
  
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
      staticruntime "On"
      systemversion "latest"
  
      defines
      {
        "AP_PLATFORM_WINDOWS",
      }
  
      filter "configurations:Debug"
        defines { "AP_DEBUG" }
        symbols "On"
  
      filter "configurations:Release"
        defines { "AP_RELEASE" }
        optimize "On"
  
      filter "configurations:Dist"
        defines { "AP_DIST" }
        optimize "On"