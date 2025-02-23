project "fm-core-headless"
	location "."

	include "common.lua"

	files {

		"src/Platform/Headless/*",
		"src/Platform/Headless/Core/**",
		"src/Platform/Headless/Backend/*"
	}

	includedirs {

		"src/Platform/Headless"
	}

-- Post build commands
filter "configurations:*DLL or *Shared"
	postbuildcommands {

		("{MKDIR} ../bin/" .. outputdir .. "/fm-client-headless"),
		("{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/fm-client-headless"),
		("{MKDIR} ../bin/" .. outputdir .. "/fm-server-headless"),
		("{COPYDIR} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/fm-server-headless")
	}
