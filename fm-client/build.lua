project "fm-client"
	location "."

	include "shared.lua"

	includedirs {

		"%{wks.location}/fm-core/src/Platform/GUI"
	}

	links {

		"fm-core"
	}
