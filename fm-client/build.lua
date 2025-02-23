project "fm-client"
	location "."

	include "common.lua"

	includedirs {

		"%{wks.location}/fm-core/src/Platform/GUI"
	}

	links {

		"fm-core"
	}

	filter "configurations:*Static"
		links {

			"glfw"
		}
