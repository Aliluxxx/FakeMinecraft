project "fm-core-headless"
	location "."

	include "shared.lua"

	files {

		"src/Platform/Headless/*",
		"src/Platform/Headless/Core/**"
	}

	includedirs {

		"src/Platform/Headless"
	}
