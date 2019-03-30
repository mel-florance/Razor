workspace "Razor"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Razor/vendor/GLFW/include"

include "Razor/vendor/GLFW"

project "Razor"
	location "Razor"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rzpch.h"
	pchsource "Razor/src/rzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
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
		systemversion "10.0.17763.0"

		defines
		{
			"RZ_PLATFORM_WINDOWS",
			"RZ_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		buildoptions "/MDd"
		symbols "On"
		
	filter "configurations:Release"
		defines "RZ_RELEASE"
		buildoptions "/MD"
		optimize "On"
		
	filter "configurations:Dist"
		defines "RZ_DIST"
		buildoptions "/MD"
		optimize "On"


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
		"Razor/vendor/spdlog/include",
		"Razor/src"
	}

	links
	{
		"Razor"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.17763.0"

		defines
		{
			"RZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "RZ_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "RZ_DIST"
		optimize "On"