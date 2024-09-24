cmake_minimum_required(VERSION 3.14)
project(WinApiPackage)

set(SRC
	main.cpp
	FindWindow.c
	FindWindow.h
	SetWindowState.c
	SetWindowState.h
	PCStartRun.c
	PCStartRun.h
)

#add_executable(CPP_LOG ${SRC})
add_library(WinApiPackage SHARED ${SRC})
source_group("src" FILES ${SRC})
add_definitions(-D_WIN_API_PACKAGE_)
SET_BIN_DIR(WinApiPackage WinApiPackage)