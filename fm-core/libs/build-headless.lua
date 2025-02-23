project "nfd"
	location "."
	kind "StaticLib"
	staticruntime ("" .. sruntime .. "")
	language "C"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/libs")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/libs")

	files {

		"nfd/src/**.h"
	}

	includedirs {

		"nfd/src/include"
	}

	filter "system:windows"
		defines "_CRT_SECURE_NO_WARNINGS"
		files "nfd/src/nfd_win.cpp"

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
