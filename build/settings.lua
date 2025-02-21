flags { "MultiProcessorCompile" }
startproject "fm-client"

outputdir = "%{cfg.buildcfg}-%{cfg.platform}"
config = "%{cfg.buildcfg}"
arch = "%{cfg.architecture}"
OS = "%{cfg.system}"
sruntime = "Off"
