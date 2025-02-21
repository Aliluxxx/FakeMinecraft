staticruntime ("" .. sruntime .. "")
language "C++"
cppdialect "C++20"
targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

files {

	"**.lua",
	"src/*",
	"src/Core/**",
	"%{wks.location}/%{prj.name}/resources/**"
}

includedirs {

	"src"
}

defines "FM_ENGINE"

-- Windows
filter "system:windows"
	pchheader "fmpch.h"
	pchsource "src/fmpch.cpp"
	defines {

		"FM_PLATFORM_WINDOWS",
		"_CRT_SECURE_NO_WARNINGS"
	}

-- Debug
filter "configurations:Debug*"
	defines "FM_DEBUG"
	runtime "Debug"
	symbols "On"
	optimize "Off"

-- Release
filter "configurations:Release*"
	defines "FM_RELEASE"
	runtime "Release"
	symbols "On"
	optimize "On"

-- Dist
filter "configurations:Dist*"
	defines "FM_DIST"
	runtime "Release"
	symbols "Off"
	optimize "On"

-- Static
filter "configurations:*Static"
	kind "StaticLib"
	defines "FM_STATIC"

-- Shared
filter "configurations:*DLL or *Shared"
	kind "SharedLib"

filter "configurations:*DLL"
	defines "FM_BUILD_DLL"

filter "configurations:*Shared"
	defines "FM_BUILD_SHARED"

filter { "Debug*", "kind:StaticLib" }
	targetsuffix "-s-d"

filter { "Debug*", "kind:SharedLib" }
	targetsuffix "-d"

filter { "Release*", "kind:StaticLib" }
	targetsuffix "-s-r"

filter { "Release*", "kind:SharedLib" }
	targetsuffix "-r"

filter { "Dist*", "kind:StaticLib" }
	targetsuffix "-s"

filter { "Dist*", "kind:SharedLib" }
	targetsuffix ""

filter {}
