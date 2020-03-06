print("Building project \"Sandbox\"...")
print("")
project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	staticruntime "on"
	
	targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"
	
	includedirs {
		"%{wks.location}/Externals/fmod/include",
		"%{wks.location}/Externals/freetype/include",
		"%{wks.location}/Externals/glad/include",
		"%{wks.location}/Externals/glfw3/include",
		"%{wks.location}/Externals/glm/include",
		"%{wks.location}/Externals/imgui/include",
		"%{wks.location}/Externals/spdlog/include",
		"%{wks.location}/Externals/stb/include",
		
		"%{wks.location}/Cappuccino/include",
		
		"include"
	}	
	
	files {
		"include/**.h",
		"include/**.hpp",
		"include/**.inl",
		"src/**.cpp"
	}

	links {
		"Cappuccino"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines {
			"CAPP_PLATFORM_WINDOWS" 
		}
		
		prebuildcommands {
			-- Update any default assets
			"robocopy \"%{wks.location}\\Cappuccino\\Assets\" \"%{prj.location}\\Assets\\Cappuccino\" /mir /xo /ns /np /ndl /njh /njs",

			"exit 0"
		}

		postbuildcommands {
			-- Copy all dll files next to executable
			"robocopy \"%{wks.location}\\Externals\\dll\" \"%{cfg.targetdir}\" /e /xo /xx /ns /np /ndl /njh /njs",
			
			-- Copy assets directory next to executable
			"robocopy \"%{prj.location}\\Assets\" \"%{cfg.targetdir}\\Assets\" /e /xo /xx /ns /np /ndl /njh /njs",
			
			"exit 0"
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
		
	filter { "configurations:Distribution", "system:windows" }
		linkoptions {
			"/subsystem:windows",
			"/entry:mainCRTStartup"
		}