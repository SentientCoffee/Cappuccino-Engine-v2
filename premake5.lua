print("Starting premake build...")

print("Building solution CappuccinoEngine...")
workspace "CappuccinoEngine"
	defines "_CRT_SECURE_NO_WARNINGS"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release"
	}

	startproject "Cappuccino"

include "Externals"

print("Building project \"Cappuccino\"...")
project "Cappuccino"
	location "%{prj.name}"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	staticruntime "on"
	
	targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"
	
	pchheader "CappPCH.h"
	pchsource "%{prj.name}/src/CappPCH.cpp"
	
	defines { "GLFW_INCLUDE_NONE" }
	
	includedirs {
		"Externals/fmod/include",
		"Externals/freetype/include",
		"Externals/glad/include",
		"Externals/glfw3/include",
		"Externals/glm/include",
		"Externals/imgui/include",
		"Externals/imgui/include/imgui",
		"Externals/spdlog/include",
		"Externals/stb/include",
		
		"%{prj.name}/include"
	}
	
	links {
		"Externals",
		"opengl32",
		"Externals/fmod/fmod64",
		"Externals/freetype/freetype"
	}
	
	files {
		"%{prj.name}/include/**.h",
		"%{prj.name}/include/**.hpp",
		"%{prj.name}/src/**.cpp"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines { "CAPP_PLATFORM_WINDOWS" }
		
	filter "configurations:Debug"
		defines {
			"CAPP_DEBUG=1",
			"CAPP_RELEASE=0",
			"CAPP_ENABLE_ASSERTS=1"
		}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines {
			"CAPP_DEBUG=0",
			"CAPP_RELEASE=1",
			"CAPP_ENABLE_ASSERTS=0"
		}
		runtime "Release"
		optimize "on"
