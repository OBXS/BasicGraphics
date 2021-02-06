solution "DirectX9"
    architecture "x64"
    startproject "D3D9"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

DXSDKDir = "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)"

project "D3D9"
    location "D3D9"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        DXSDKDir .. "/Include/d3d9.h",
        DXSDKDir .. "/Include/d3dx9.h"
    }

    defines
    {
        "WIN32_LEAN_AND_MEAN"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        buildoptions "/MDd"

        links
        {
            DXSDKDir .. "/Lib/x64/d3d9.lib",
            DXSDKDir .. "/Lib/x64/d3dx9d.lib",
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        buildoptions "/MD"

        links
        {
            DXSDKDir .. "/Lib/x64/d3d9.lib",
            DXSDKDir .. "/Lib/x64/d3dx9.lib",
        }