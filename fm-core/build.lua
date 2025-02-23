project "fm-core"
	location "."

	include "common.lua"

	files {

		"src/Platform/GUI/*",
		"src/Platform/GUI/Core/**",
		"src/Platform/GUI/Backend/GLFW/**"
	}

	includedirs {

		"src/Platform/GUI",
		"%{IncludeDir.glfw}"
	}

-- Post build commands
filter "configurations:*DLL or *Shared"
	postbuildcommands {

		("{MKDIR} ../bin/" .. outputdir .. "/fm-client"),
		("{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/fm-client"),
		("{MKDIR} ../bin/" .. outputdir .. "/fm-server"),
		("{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/fm-server")
	}

	links {

		"glfw"
	}
