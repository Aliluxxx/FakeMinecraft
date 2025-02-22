include "build-headless.lua"

project "glfw"
	location "."
	kind "StaticLib"
	staticruntime ("" .. sruntime .. "")
	language "C"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/libs")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/libs")

	files {

		"glfw/src/context.c",
		"glfw/src/egl_context.c",
		"glfw/src/glx_context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/monitor.c",
		"glfw/src/null_init.c",
		"glfw/src/null_joystick.c",
		"glfw/src/null_monitor.c",
		"glfw/src/null_window.c",
		"glfw/src/platform.c",
		"glfw/src/window.c",
		"glfw/src/vulkan.c",
		"glfw/src/osmesa_context.c"
	}

	includedirs {

		"glfw/src/include"
	}

	filter "system:windows"
		defines { "_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS" }

		files {

			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_module.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c"
		}

	filter {}

	-- Debug
	filter "configurations:Debug*"
		runtime "Debug"
		symbols "On"
		optimize "Off"
		targetsuffix "-s-d"

	-- Release
	filter "configurations:Release*"
		runtime "Release"
		symbols "On"
		optimize "On"
		targetsuffix "-s-r"

	-- Dist
	filter "configurations:Dist*"
		runtime "Release"
		symbols "Off"
		optimize "On"
		targetsuffix "-s"

	filter {}
