cmake_minimum_required(VERSION 3.14)
project(CPP_LOG)

set(SRC
	src/main.cpp
	src/cpp_log.h
	src/cpp_log.cpp
	src/LogBuffer.cpp
	src/LogBuffer.h
	src/LogTool.cpp
	src/LogTool.h
)

#add_executable(CPP_LOG ${SRC})
add_library(CPP_LOG SHARED ${SRC})
source_group("src" FILES ${SRC})
add_definitions(-D_CPP_LOG_)
SET_BIN_DIR(CPP_LOG CPP_LOG)