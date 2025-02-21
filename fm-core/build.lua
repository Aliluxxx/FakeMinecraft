project "fm-core"
	location "."

	include "shared.lua"

	files {

		"src/Platform/GUI/*",
		"src/Platform/GUI/Core/**"
	}

	includedirs {

		"src/Platform/GUI"
	}
