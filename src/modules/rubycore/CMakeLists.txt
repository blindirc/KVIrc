# CMakeLists for src/modules/rubycore

set(kvirubycore_SRCS
	libkvirubycore.cpp
	kvircmodule.cpp)

set(KVI_DEFAULT_FRAME_CAPTION "")

set(kvi_module_name kvirubycore)

include(${CMAKE_SOURCE_DIR}/cmake/module.rules.txt)
