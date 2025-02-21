defines {

	"FM_HEADLESS"
}

flags { "MultiProcessorCompile" }
startproject "fm-client-headless"

outputdir = "%{cfg.buildcfg}-%{cfg.platform}"
config = "%{cfg.buildcfg}"
arch = "%{cfg.architecture}"
OS = "%{cfg.system}"
sruntime = "Off"
