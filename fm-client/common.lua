kind "ConsoleApp"
staticruntime ("" .. sruntime .. "")
language "C++"
cppdialect "C++20"
targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

files {

	"**.lua",
	"src/*",
	"src/**.h", "src/**.cpp", "src/**.inl",
	"%{wks.location}/%{prj.name}/resources/**"
}

includedirs {

	"src",
	"%{wks.location}/fm-core/src",
	"%{IncludeDir.spdlog}"
}

-- Windows
filter "system:windows"
	defines {

		"FM_PLATFORM_WINDOWS",
		"_CRT_SECURE_NO_WARNINGS"
	}

-- Static
filter "configurations:*Static"
	defines "FM_STATIC"

	links {

		"nfd"
	}

-- Shared
filter "configurations:*DLL or *Shared"

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

filter {}

-- Post build commands
postbuildcommands {

	("{MKDIR} ../bin/" .. outputdir .. "/%{prj.name}/resources"),
	("{MKDIR} %{wks.location}/fm-client/resources"),
	("{COPYDIR} %{wks.location}/fm-client/resources ../bin/" .. outputdir .. "/%{prj.name}/resources")
}
