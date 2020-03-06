os.execute("echo Enter project directory:")
local solutionDir = io.read()
if(string.find(solutionDir, "\"", 1)) then
	solutionDir = solutionDir:sub(2)
end
if(string.find(solutionDir, "\"", -1)) then
	solutionDir = solutionDir:sub(1, -2)
end

os.execute("echo.")
os.execute("echo Enter project name:")
local projName = io.read()
os.execute("echo.")

print(string.format("Creating project \"%s\" in directory \"%s\"", projName, solutionDir))

local engineDir = path.getabsolute(_WORKING_DIR .. "/..")

print("Making directories...")
os.execute("mkdir \"" .. solutionDir .. "\\Assets\"")
os.execute("mkdir \"" .. solutionDir .. "\\include\"")
os.execute("mkdir \"" .. solutionDir .. "\\src\"")

print("Copying necessary files...")
-- Copy engine libraries into libs folder
os.execute("robocopy ..\\Externals\\Build\\bin " .. "\"" .. solutionDir .. "\\libs\" *.lib /e /xo /xx /ns /np /ndl /njh /njs")
os.execute("robocopy ..\\Build\\bin "            .. "\"" .. solutionDir .. "\\libs\" *.lib /e /xo /xx /ns /np /ndl /njh /njs")

-- Copy external header, inline, and dll files
os.execute("robocopy ..\\Externals "             .. "\"" .. solutionDir .. "\\Engine\\Externals\" *.dll *.h *.hpp *.inl /s /xo /xx /ns /np /ndl /njh /njs")

-- Copy engine header files
os.execute("robocopy ..\\Cappuccino\\include "   .. "\"" .. solutionDir .. "\\Engine\\Cappuccino\\include\" *.h *.hpp /mir /xo /xx /ns /np /ndl /njh /njs")

os.execute("echo.")
print("Starting premake build...")

print(string.format("Building solution %s...", projName))
workspace (projName)
	location(solutionDir)
	defines "_CRT_SECURE_NO_WARNINGS"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release"
	}

print(string.format("Building project %s...", projName)) 
project (projName)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	staticruntime "on"
	
	targetdir (solutionDir .. "/Build/bin/%{cfg.architecture}/%{cfg.buildcfg}")
	objdir (solutionDir .. "/Build/obj/%{cfg.architecture}/%{cfg.buildcfg}")
	
	includedirs {
		"\"" .. solutionDir .. "/Engine/Externals/fmod/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/freetype/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/glad/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/glfw3/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/glm/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/imgui/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/spdlog/include\"",
		"\"" .. solutionDir .. "/Engine/Externals/stb/include\"",
		
		"\"" .. solutionDir .. "/Engine/Cappuccino/include\"",
		
		"\"" .. solutionDir .. "/include\""
	}
	
	files {
		solutionDir .. "/include/**.h",
		solutionDir .. "/include/**.hpp",
		solutionDir .. "/src/**.cpp",
	}
	
	links {
		"\"" .. solutionDir .. "/libs/%{cfg.architecture}/%{cfg.buildcfg}/Cappuccino\"",
		"\"" .. solutionDir .. "/libs/%{cfg.architecture}/%{cfg.buildcfg}/Externals\""
	}
	
	filter "system:windows"
		systemversion "latest"
		defines { "CAPP_PLATFORM_WINDOWS" }
		
		prebuildcommands {
			-- Update header files from engine
			"robocopy \"" .. engineDir .. "\\Cappuccino\\include\" \"%{prj.location}\\Engine\\Cappuccino\\include\" *.h *.hpp *.inl /mir /xo /ns /np /ndl /njh /njs",
			
			-- Update lib files from engine
			"robocopy \"" .. engineDir .. "\\Externals\\Build\\bin\" \"%{prj.location}\\libs\" *.lib /e /xo /xx /ns /np /ndl /njh /njs",
			"robocopy \"" .. engineDir .. "\\Build\\bin\" \"%{prj.location}\\libs\" *.lib /e /xo /xx /ns /np /ndl /njh /njs",
			
			-- Update any default assets
			"robocopy \"" .. engineDir .. "\\Cappuccino\\Assets\" \"%{prj.location}\\Assets\\Cappuccino\" /s /xo /xx /ns /np /ndl /njh /njs",
			
			"exit 0"
		}
		
		postbuildcommands {
			-- Copy all dll files next to executable
			"robocopy \"%{prj.location}\\Engine\\Externals\\dll\" \"%{cfg.targetdir}\" /e /xo /xx /ns /np /ndl /njh /njs",
			
			-- Copy assets directory next to executable
			"robocopy \"%{prj.location}\\Assets\" \"%{cfg.targetdir}\\Assets\" /mir /xo /xx /ns /np /ndl /njh /njs",
			
			"exit 0"
		}
		
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