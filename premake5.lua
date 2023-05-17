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
IncludeDir["glm"] = "Apostle/vendor/glm"
IncludeDir["stb_image"] = "Apostle/vendor/stb_image"

group "Dependencies"
    include "Apostle/vendor/GLFW"
    include "Apostle/vendor/Glad"
    include "Apostle/vendor/imgui"

group""

project "Apostle"
    location "Apostle"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "APpch.h"
    pchsource "Apostle/src/APpch.cpp"

    files 
    { 
      "%{prj.name}/src/**.h", 
      "%{prj.name}/src/**.cpp",
      "%{prj.name}/vendor/imgui/backends/imgui_impl_glfw.h",
      "%{prj.name}/vendor/imgui/backends/imgui_impl_glfw.cpp",
      "%{prj.name}/vendor/stb_image/**.h",
      "%{prj.name}/vendor/stb_image/**.cpp",
      "%{prj.name}/vendor/glm/glm/**.hpp",
      "%{prj.name}/vendor/glm/glm/**.inl",
    }

    includedirs
    {
      "%{prj.name}/src",
      "%{prj.name}/vendor/spdlog/include",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.Glad}",
      "%{IncludeDir.ImGui}",
      "%{IncludeDir.glm}",
      "%{IncludeDir.stb_image}"
    }

    links
    {
      "GLFW",
      "Glad",
      "ImGui",
      "opengl32.lib"
    }

    defines
      {
        "_CRT_SECURE_NO_WARNINGS"
      }
    
    filter "system:windows"
      systemversion "latest"

      defines
      {
        "AP_PLATFORM_WINDOWS",
        "AP_BUILD_DLL",
        "GLFW_INCLUDE_NONE",
        "IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
      }
      
      
    filter "configurations:Debug"
      defines { "AP_DEBUG" }
      runtime "Debug"
      symbols "on"

    filter "configurations:Release"
      defines { "AP_RELEASE" }
      runtime "Release"
      optimize "on"

    filter "configurations:Dist"
      defines { "AP_DIST" }
      runtime "Release"
      optimize "on"

--SANDBOX PROJECT--
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
  
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
      "Apostle/src",
      "%{IncludeDir.ImGui}",
      "%{IncludeDir.glm}"
    }

    links
    {
      "Apostle",
      "ImGui"
    }
      
    filter "system:windows"
      systemversion "latest"
  
      defines
      {
        "AP_PLATFORM_WINDOWS",
      }
  
      filter "configurations:Debug"
        defines { "AP_DEBUG" }
        runtime "Debug"
        symbols "on"
  
      filter "configurations:Release"
        defines { "AP_RELEASE" }
        runtime "Release"
        optimize "on"
  
      filter "configurations:Dist"
        defines { "AP_DIST" }
        runtime "Release"
        optimize "on"
