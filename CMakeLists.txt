cmake_minimum_required(VERSION 3.14)
project(StickyNote)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(version 1.002)
#生成版本号
configure_file(version.hpp version.hpp)




set(QT_PATH "D:/qt6/6.6.0/msvc2019_64" CACHE PATH "qt cmake dir")
OPTION(BUILD_QT6 "Build with Qt5." ON)
OPTION(BUILD_QT5 "Build with Qt5." OFF)
OPTION(BUILD_TO_WIN7 "Run with windows 7" OFF)
#检查qt版本使用对应的C++版本
#QT6需要使用C++17
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(BUILD_QT6)
    if(COMPILER_SUPPORTS_CXX17)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    elseif(COMPILER_SUPPORTS_CXX11)#
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
    endif()
elseif(BUILD_QT5)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()





#查找qt组件
set(CMAKE_PREFIX_PATH ${QT_PATH})

if(BUILD_QT6)
find_package (Qt6Core)
find_package (Qt6Widgets)
find_package (Qt6Gui)
find_package (Qt6Core)
find_package (Qt6Network)
find_package (Qt6LinguistTools)
endif()

if(BUILD_QT5)
find_package (Qt5Core)
find_package (Qt5Widgets)
find_package (Qt5Gui)
find_package (Qt5Core)
find_package (Qt5Network)
find_package (Qt5LinguistTools)
endif()



include(CMake/Mod.cmake)
add_subdirectory(OriginUI)
add_subdirectory(Gui)
add_subdirectory(Log)
add_subdirectory(StickyNoteFile)
add_subdirectory(WinApiPackage)
add_subdirectory(GlobalShortcut)
add_subdirectory(xml)