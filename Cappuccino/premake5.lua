print("Building project \"Cappuccino\"...")
project "Cappuccino"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	staticruntime "on"
	
	targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"
	
	pchheader "CappPCH.h"
	pchsource "src/CappPCH.cpp"
	
	defines { "GLFW_INCLUDE_NONE" }
	
	includedirs {
		"%{wks.location}/Externals/fmod/include",
		"%{wks.location}/Externals/freetype/include",
		"%{wks.location}/Externals/glad/include",
		"%{wks.location}/Externals/glfw3/include",
		"%{wks.location}/Externals/glm/include",
		"%{wks.location}/Externals/imgui/include",
		"%{wks.location}/Externals/imgui/include/imgui",
		"%{wks.location}/Externals/spdlog/include",
		"%{wks.location}/Externals/stb/include",
		
		"include"
	}
	
	links {
		"Externals",
		"opengl32"
	}
	
	files {
		"include/**.h",
		"include/**.hpp",
		"src/**.cpp"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines {
			"CAPP_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines {
			"CAPP_DEBUG=1",
			"CAPP_RELEASE=0",
			"CAPP_DIST=0",
			"CAPP_ENABLE_ASSERTS=1"
		}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines {
			"CAPP_DEBUG=0",
			"CAPP_RELEASE=1",
			"CAPP_DIST=0",
			"CAPP_ENABLE_ASSERTS=1"
		}
		runtime "Release"
		optimize "on"
		
	filter "configurations:Distribution"
		defines {
			"CAPP_DEBUG=0",
			"CAPP_RELEASE=0",
			"CAPP_DIST=1",
			"CAPP_ENABLE_ASSERTS=0"
		}
		runtime "Release"
		optimize "on"