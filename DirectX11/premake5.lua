solution "DirectX11"
    architecture "x64"
    startproject "D3D11"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "D3D11"
    location "D3D11"
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
        "../Shared/Windows/"
    }

    defines
    {
        "WIN32_LEAN_AND_MEAN"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        buildoptions "/MDd"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        buildoptions "/MD"