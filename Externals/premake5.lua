print("Building external dependencies...") 
project "Externals"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	staticruntime "on"
	
	targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"
	
	includedirs {
		"glad/include",
		"glfw3/include",
		"imgui/include/imgui",
		"stb/include/stb"
	}
	
	files {
		"glad/include/KHR/khrplatform.h",
		"glad/include/glad/glad.h",
		"glad/src/glad.c",
		
		"glfw3/include/glfw/glfw3.h",
        "glfw3/include/glfw/glfw3native.h",
        "glfw3/src/glfw_config.h",
        "glfw3/src/context.c",
        "glfw3/src/init.c",
        "glfw3/src/input.c",
        "glfw3/src/monitor.c",
        "glfw3/src/vulkan.c",
        "glfw3/src/window.c",
		
		"imgui/include/imgui/imconfig.h",
		"imgui/include/imgui/imgui.h",
		"imgui/include/imgui/imgui_internal.h",
		"imgui/include/imgui/imstb_rectpack.h",
		"imgui/include/imgui/imstb_textedit.h",
		"imgui/include/imgui/imstb_truetype.h",
		"imgui/include/imgui/imgui.cpp",
		"imgui/include/imgui/imgui_demo.cpp",
		"imgui/include/imgui/imgui_draw.cpp",
		"imgui/include/imgui/imgui_widgets.cpp",
		
		"stb/include/stb/stb_image.h",
        "stb/include/stb/stb_image_write.h",
        "stb/include/stb/stb_impl.h",
        "stb/include/stb/stb_include.h",
        "stb/include/stb/stb_perlin.h",
        "stb/include/stb/stb_rect_pack.h",
        "stb/include/stb/stb_truetype.h",
        "stb/src/stb_impl.cpp"
	}
	
	links {
		"fmod/fmod64",
		"freetype/freetype"
	}
	
	filter "system:windows"		
        systemversion "latest"
        
        files {
            "glfw3/src/win32_init.c",
            "glfw3/src/win32_joystick.c",
            "glfw3/src/win32_monitor.c",
            "glfw3/src/win32_time.c",
            "glfw3/src/win32_thread.c",
            "glfw3/src/win32_window.c",
            "glfw3/src/wgl_context.c",
            "glfw3/src/egl_context.c",
            "glfw3/src/osmesa_context.c"
        }

		defines { 
            "_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Distribution"
		runtime "Release"
		optimize "on"