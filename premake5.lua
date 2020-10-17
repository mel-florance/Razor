workspace "Razor"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Razor/vendor/GLFW/include"
IncludeDir["Glad"] = "Razor/vendor/Glad/include"
IncludeDir["ImGui"] = "Razor/vendor/imgui"
IncludeDir["glm"] = "Razor/vendor/glm"
IncludeDir["stb"] = "Razor/vendor/stb"
IncludeDir["assimp"] = "Razor/vendor/assimp/include"
IncludeDir["lua"] = "Razor/vendor/lua/include"
IncludeDir["bullet"] = "Razor/vendor/bullet/include"
IncludeDir["openal"] = "Razor/vendor/openal/include"
IncludeDir["opencv"] = "Razor/vendor/opencv/include"

include "Razor/vendor/GLFW"
include "Razor/vendor/Glad"	
include "Razor/vendor/ImGui"

project "Razor"
	location "Razor"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
 
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "rzpch.h"
	pchsource "Razor/src/rzpch.cpp"

	linkoptions { "-IGNORE:4221", "-IGNORE:4006" }

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cc",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/assimp/include/**",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.lua}",
		"%{IncludeDir.bullet}",
		"%{IncludeDir.openal}",
		"%{IncludeDir.opencv}",
		"%{prj.name}/vendor/assimp/include",
		"%{prj.name}/vendor/assimp/build/include"
	}

	libdirs { 
		"%{prj.name}/vendor/assimp/build/code/Release",
		"%{prj.name}/vendor/lua",
		"%{prj.name}/vendor/bullet/lib",
		"%{prj.name}/vendor/openal",
		"%{prj.name}/vendor/opencv",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"assimp",
		"lua53.lib",
		"OpenAL32.lib",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"IMGUI_API=__declspec(dllexport)",
			"RZ_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

		postbuildcommands
		{
			--"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox",
			--"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Server",
			"{COPY} ../Razor/vendor/assimp/build/code/Release/assimp.dll ../bin/Release-windows-x86_64/Sandbox",
			"{COPY} ../Razor/vendor/assimp/build/code/Release/assimp.dll ../bin/Debug-windows-x86_64/Sandbox",
			"{COPY} ../Razor/vendor/lua/lua53.dll ../bin/Release-windows-x86_64/Sandbox",
			"{COPY} ../Razor/vendor/lua/lua53.dll ../bin/Debug-windows-x86_64/Sandbox",
			"{COPY} ../Razor/vendor/openal/OpenAL32.dll ../bin/Release-windows-x86_64/Sandbox",
			"{COPY} ../Razor/vendor/openal/OpenAL32.dll ../bin/Debug-windows-x86_64/Sandbox",

			"{COPY} ../Razor/vendor/assimp/build/code/Release/assimp.dll ../bin/Release-windows-x86_64/Server",
			"{COPY} ../Razor/vendor/assimp/build/code/Release/assimp.dll ../bin/Debug-windows-x86_64/Server",
			"{COPY} ../Razor/vendor/lua/lua53.dll ../bin/Release-windows-x86_64/Server",
			"{COPY} ../Razor/vendor/lua/lua53.dll ../bin/Debug-windows-x86_64/Server",
			"{COPY} ../Razor/vendor/openal/OpenAL32.dll ../bin/Release-windows-x86_64/Server",
			"{COPY} ../Razor/vendor/openal/OpenAL32.dll ../bin/Debug-windows-x86_64/Server",

			"{COPY} ../Razor/vendor/assimp/build/code/Release/assimp.dll ../bin/Release-windows-x86_64/Client",
			"{COPY} ../Razor/vendor/assimp/build/code/Release/assimp.dll ../bin/Debug-windows-x86_64/Client",
			"{COPY} ../Razor/vendor/lua/lua53.dll ../bin/Release-windows-x86_64/Client",
			"{COPY} ../Razor/vendor/lua/lua53.dll ../bin/Debug-windows-x86_64/Client",
			"{COPY} ../Razor/vendor/openal/OpenAL32.dll ../bin/Release-windows-x86_64/Client",
			"{COPY} ../Razor/vendor/openal/OpenAL32.dll ../bin/Debug-windows-x86_64/Client"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		buildoptions {"/MTd"}
		symbols "On"
		links {
			"BulletCollision_Debug.lib",
			"BulletDynamics_Debug.lib",
			"BulletSoftBody_Debug.lib",
			"LinearMath_Debug.lib",
			
			"opencv_core411d.lib",
			"opencv_imgcodecs411d.lib",
			"opencv_highgui411d.lib",
			"opencv_imgproc411d.lib",
			"opencv_videoio411d.lib"
		}
		
	filter "configurations:Release"
		defines "RZ_RELEASE"
		runtime "Release"
		optimize "On"
		links {
			"BulletCollision.lib",
			"BulletDynamics.lib",
			"BulletSoftBody.lib",
			"LinearMath.lib",

			"opencv_core411.lib",
			"opencv_imgcodecs411.lib",
			"opencv_highgui411.lib",
			"opencv_imgproc411.lib",
			"opencv_videoio411.lib"
		}
		
	filter "configurations:Dist"
		defines "RZ_DIST"
		runtime "Release"
		optimize "On"
		links {
			"BulletCollision.lib",
			"BulletDynamics.lib",
			"BulletSoftBody.lib",
			"LinearMath.lib",

			"opencv_core411.lib",
			"opencv_imgcodecs411.lib",
			"opencv_highgui411.lib",
			"opencv_imgproc411.lib",
			"opencv_videoio411.lib"
		}


project "Sandbox"
	location "Sandbox"

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
		"Razor/src",
		"Razor/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.bullet}"
	}

	links
	{
		"Razor"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		kind "ConsoleApp"
		symbols "On"
		
	filter "configurations:Release"
		defines "RZ_RELEASE"
		optimize "On"
		kind "WindowedApp"
		entrypoint "mainCRTStartup" 
		
	filter "configurations:Dist"
		defines "RZ_DIST"
		optimize "On"
		kind "WindowedApp"
		entrypoint "mainCRTStartup" 

project "Server"
	location "Server"
	kind "ConsoleApp"
	language "C++"

	linkoptions { 
		"-IGNORE:4099"
	}

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
		"Razor/src",
		"Razor/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.bullet}"
	}

	links
	{
		"Razor"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"RZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		symbols "On"
		buildoptions {"/MTd"}
		
	filter "configurations:Release"
		defines "RZ_RELEASE"
		optimize "On"
		
	filter "configurations:Dist"
		defines "RZ_DIST"
		optimize "On"
		
project "Client"
	location "Client"
	language "C++"
	kind "WindowedApp"

	linkoptions { 
		"-IGNORE:4099"
	}

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
		"Razor/src",
		"Razor/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.bullet}"
	}

	links
	{
		"Razor"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"RZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RZ_DEBUG"
		symbols "On"
		kind "ConsoleApp"
		buildoptions {"/MTd"}
		
	filter "configurations:Release"
		defines "RZ_RELEASE"
		optimize "On"
		entrypoint "mainCRTStartup" 
		
	filter "configurations:Dist"
		defines "RZ_DIST"
		optimize "On"	
		entrypoint "mainCRTStartup" 
