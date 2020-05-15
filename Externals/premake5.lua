print("Building external dependencies...") 
project "Externals"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	staticruntime "on"
	
	targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"
	
	warnings "default"

	links {
		"fmod/fmod64"
	}

	includedirs {
		"freetype/include/freetype",
		"freetype/include/freetype/freetype",
		"freetype/include/freetype/freetype/config",
		"freetype/include/freetype/freetype/internal",

		"glad/include",
		"glfw3/include",
		"imgui/include/imgui",
		"stb/include/stb"
	}
	
	files {
		"freetype/src/autofit/autofit.c",
		"freetype/src/base/ftbase.c",
		"freetype/src/base/ftbbox.c",
		"freetype/src/base/ftbdf.c",
		"freetype/src/base/ftbitmap.c",
		"freetype/src/base/ftcid.c",
		"freetype/src/base/ftfstype.c",
		"freetype/src/base/ftgasp.c",
		"freetype/src/base/ftglyph.c",
		"freetype/src/base/ftgxval.c",
		"freetype/src/base/ftinit.c",
		"freetype/src/base/ftmm.c",
		"freetype/src/base/ftotval.c",
		"freetype/src/base/ftpatent.c",
		"freetype/src/base/ftpfr.c",
		"freetype/src/base/ftstroke.c",
		"freetype/src/base/ftsynth.c",
		"freetype/src/base/ftsystem.c",
		"freetype/src/base/fttype1.c",
		"freetype/src/base/ftwinfnt.c",
		"freetype/src/bdf/bdf.c",
		"freetype/src/bzip2/ftbzip2.c",
		"freetype/src/cache/ftcache.c",
		"freetype/src/cff/cff.c",
		"freetype/src/cid/type1cid.c",
		"freetype/src/gzip/ftgzip.c",
		"freetype/src/lzw/ftlzw.c",
		"freetype/src/pcf/pcf.c",
		"freetype/src/pfr/pfr.c",
		"freetype/src/psaux/psaux.c",
		"freetype/src/pshinter/pshinter.c",
		"freetype/src/psnames/psnames.c",
		"freetype/src/raster/raster.c",
		"freetype/src/sfnt/sfnt.c",
		"freetype/src/smooth/smooth.c",
		"freetype/src/truetype/truetype.c",
		"freetype/src/type1/type1.c",
		"freetype/src/type42/type42.c",
		"freetype/src/winfonts/winfnt.c",

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
	
	filter "system:windows"
		systemversion "latest"
			
		files {
			"freetype/builds/windows/ftdebug.c",
			"freetype/src/base/ftver.rc",

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
			"FT2_BUILD_LIBRARY",
			"WIN32",
			"_LIB",
			"_UNICODE",
			"UNICODE",

			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_WARNINGS",
		}
		
	filter "configurations:Debug"
		defines {
			"_DEBUG",
			"FT_DEBUG_LEVEL_ERROR",
			"FT_DEBUG_LEVEL_TRACE"
		}

		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "on"
	
	filter "configurations:Distribution"
		defines { "NDEBUG" }
		runtime "Release"
		optimize "on"