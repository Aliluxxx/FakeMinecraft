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

project "ogg"
	location "."
	kind "StaticLib"
	language "C"
	staticruntime ("" .. sruntime .. "")
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/libs")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/libs")

	includedirs {

		"ogg/include",
		"ogg/src"
	}

	files {

		"ogg/include/**",
		"ogg/src/**"
	}

	filter "system:windows"
		systemversion "latest"
		disablewarnings "4244"

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

project "openal"
	location "."
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime ("" .. sruntime .. "")
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/libs")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/libs")

	includedirs {

		"openal/include",
		"openal/include/AL",
		"openal/common",
		"openal",
		"openal/build",
		"openal/fmt-11.1.1/include"
	}

	files {

		"openal/version.h",
		"openal/hrtf_default.h",

		"openal/common/alassert.cpp",
		"openal/common/filesystem.cpp",
		"openal/common/alcomplex.cpp",
		"openal/common/alsem.cpp",
		"openal/common/alstring.cpp",
		"openal/common/althrd_setname.cpp",
		"openal/common/dynload.cpp",
		"openal/common/polyphase_resampler.cpp",
		"openal/common/ringbuffer.cpp",
		"openal/common/strutils.cpp",
		"openal/common/pffft.cpp",
		"openal/alc/alc.cpp",
		"openal/alc/alconfig.cpp",
		"openal/alc/alu.cpp",
		"openal/core/ambdec.cpp",
		"openal/core/ambidefs.cpp",
		"openal/al/effects/autowah.cpp",
		"openal/alc/effects/autowah.cpp",
		"openal/al/auxeffectslot.cpp",
		"openal/alc/backends/base.cpp",
		"openal/core/bformatdec.cpp",
		"openal/core/filters/biquad.cpp",
		"openal/core/bs2b.cpp",
		"openal/core/bsinc_tables.cpp",
		"openal/core/storage_formats.cpp",
		"openal/al/buffer.cpp",
		"openal/al/debug.cpp",
		"openal/core/buffer_storage.cpp",
		"openal/alc/effects/chorus.cpp",
		"openal/al/effects/chorus.cpp",
		"openal/al/effects/compressor.cpp",
		"openal/alc/effects/compressor.cpp",
		"openal/core/context.cpp",
		"openal/alc/context.cpp",
		"openal/core/converter.cpp",
		"openal/alc/effects/convolution.cpp",
		"openal/al/effects/convolution.cpp",
		"openal/core/cpu_caps.cpp",
		"openal/alc/effects/dedicated.cpp",
		"openal/al/effects/dedicated.cpp",
		"openal/core/cubic_tables.cpp",
		"openal/core/devformat.cpp",
		"openal/core/device.cpp",
		"openal/alc/device.cpp",
		"openal/alc/events.cpp",
		"openal/alc/effects/distortion.cpp",
		"openal/al/effects/distortion.cpp",
		"openal/alc/effects/echo.cpp",
		"openal/al/effects/echo.cpp",
		"openal/al/effect.cpp",
		"openal/al/effects/effects.cpp",
		"openal/core/effectslot.cpp",
		"openal/alc/effects/equalizer.cpp",
		"openal/al/effects/equalizer.cpp",
		"openal/al/error.cpp",
		"openal/al/event.cpp",
		"openal/core/except.cpp",
		"openal/al/extension.cpp",
		"openal/al/filter.cpp",
		"openal/core/fpu_ctrl.cpp",
		"openal/al/effects/fshifter.cpp",
		"openal/alc/effects/fshifter.cpp",
		"openal/core/helpers.cpp",
		"openal/core/hrtf.cpp",
		"openal/al/listener.cpp",
		"openal/core/logging.cpp",
		"openal/alc/backends/loopback.cpp",
		"openal/core/mastering.cpp",
		"openal/core/mixer.cpp",
		"openal/core/mixer/mixer_c.cpp",
		"openal/core/mixer/mixer_sse.cpp",
		"openal/core/mixer/mixer_sse2.cpp",
		"openal/core/mixer/mixer_sse3.cpp",
		"openal/core/mixer/mixer_sse41.cpp",
		"openal/al/effects/modulator.cpp",
		"openal/alc/effects/modulator.cpp",
		"openal/core/filters/nfc.cpp",
		"openal/al/effects/null.cpp",
		"openal/alc/effects/null.cpp",
		"openal/alc/backends/null.cpp",
		"openal/alc/panning.cpp",
		"openal/alc/effects/pshifter.cpp",
		"openal/al/effects/pshifter.cpp",
		"openal/alc/effects/reverb.cpp",
		"openal/al/effects/reverb.cpp",
		"openal/al/source.cpp",
		"openal/core/filters/splitter.cpp",
		"openal/al/state.cpp",
		"openal/core/uhjfilter.cpp",
		"openal/core/uiddefs.cpp",
		"openal/alc/effects/vmorpher.cpp",
		"openal/al/effects/vmorpher.cpp",
		"openal/core/voice.cpp",
		"openal/alc/backends/wave.cpp",
		--"openal/fmt-11.1.1/src/fmt.cc",
		"openal/fmt-11.1.1/src/format.cc",
		"openal/fmt-11.1.1/src/os.cc"
	}

	defines {

		"FMT_UNICODE=0"
	}

	filter "system:windows"
		systemversion "latest"
		conformancemode "On"

		defines {

			"FM_PLATFORM_WINDOWS",
			"AL_LIBTYPE_STATIC",
			"WIN32",
			"_WINDOWS",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"ALC_API=",
			"AL_API=",
			"_WIN32",
			"NOMINMAX",
			"RESTRICT=__restrict",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR",
			"OpenAL_EXPORTS",
			"NDEBUG"
		}

		disablewarnings {

			"4018",
			"4065",
			"4127",
			"4268",
			"4324",
			"5030",
			"5051",
			"4161"
		}

		files {

			"openal/al/eax/api.cpp",
			"openal/al/eax/call.cpp",
			"openal/al/eax/exception.cpp",
			"openal/al/eax/fx_slot_index.cpp",
			"openal/al/eax/fx_slots.cpp",
			"openal/al/eax/utils.cpp",
			"openal/alc/backends/dsound.cpp",
			"openal/alc/backends/wasapi.cpp",
			"openal/alc/backends/winmm.cpp"
		}

	filter {}

	include "config.txt"

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

project "vorbis"
	location "."
	kind "StaticLib"
	staticruntime ("" .. sruntime .. "")
	language "C"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/libs")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/libs")

	disablewarnings {

		"4244",
		"4267",
		"4305"
	}

	includedirs {

		"vorbis/include",
		"vorbis/lib",
		"ogg/include"
	}

	files {

		"vorbis/lib/analysis.c",
		"vorbis/lib/bitrate.c",
		"vorbis/lib/block.c",
		"vorbis/lib/codebook.c",
		"vorbis/lib/envelope.c",
		"vorbis/lib/envelope.c",
		"vorbis/lib/floor0.c",
		"vorbis/lib/floor1.c",
		"vorbis/lib/floor1.c",
		"vorbis/lib/info.c",
		"vorbis/lib/lookup.c",
		"vorbis/lib/lpc.c",
		"vorbis/lib/lsp.c",
		"vorbis/lib/mapping0.c",
		"vorbis/lib/mdct.c",
		"vorbis/lib/psy.c",
		"vorbis/lib/registry.c",
		"vorbis/lib/res0.c",
		"vorbis/lib/sharedbook.c",
		"vorbis/lib/smallft.c",
		"vorbis/lib/synthesis.c",
		"vorbis/lib/vorbisenc.c",
		"vorbis/lib/vorbisfile.c",
		"vorbis/lib/window.c"
	}

	filter "system:windows"
		systemversion "latest"
		defines "_CRT_SECURE_NO_WARNINGS"

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
