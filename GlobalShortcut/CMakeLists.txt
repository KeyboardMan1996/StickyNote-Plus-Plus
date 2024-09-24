#设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)
project(GlobalShortcut)
#设置C++版本
set(CMAKE_CXX_STANDARD 11)

add_definitions(-D_GLOBAL_SHORTCUT_)

#添加头文件目录
include_directories(${CMAKE_CURRENT_BINARY_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/core
${CMAKE_CURRENT_SOURCE_DIR}/gui
)
#添加外部库依赖目录
if(BUILD_QT6)
include_directories(
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6OpenGL_INCLUDE_DIRS}
    ${Qt6PrintSupport_INCLUDE_DIRS}
    ${Qt6Svg_INCLUDE_DIRS}
    ${Qt6Network_INCLUDE_DIRS}
    ${Qt6UiTools_INCLUDE_DIRS}
 )
list(APPEND QT_LIB
    ${Qt6Core_LIBRARIES}
    ${Qt6Widgets_LIBRARIES}
    ${Qt6OpenGL_LIBRARIES}
    ${Qt6PrintSupport_LIBRARIES}
    ${Qt6Svg_LIBRARIES}
    ${Qt6Network_LIBRARIES}
    ${Qt6UiTools_LIBRARIES}
)
endif()

if(BUILD_QT5)
include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5OpenGL_INCLUDE_DIRS}
    ${Qt5PrintSupport_INCLUDE_DIRS}
    ${Qt5Svg_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5UiTools_INCLUDE_DIRS}
 )
list(APPEND QT_LIB
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5OpenGL_LIBRARIES}
    ${Qt5PrintSupport_LIBRARIES}
    ${Qt5Svg_LIBRARIES}
    ${Qt5Network_LIBRARIES}
    ${Qt5UiTools_LIBRARIES}
)
endif()



# 将源代码添加到工程
SET(SOURCES core/qxtglobal.h 
    gui/qxtglobalshortcut_p.h 
    gui/qxtglobalshortcut.h 
    gui/qxtwindowsystem.h
	core/qxtglobal.cpp 
    gui/qxtwindowsystem.cpp 
    gui/qxtglobalshortcut.cpp
	gui/qxtwindowsystem_win.cpp 
    gui/qxtglobalshortcut_win.cpp
	)
	
set(moc_header
	gui/qxtglobalshortcut.h 
)	
if(BUILD_QT6)	
qt6_wrap_cpp(MOC_SRC ${moc_header})
endif()
if(BUILD_QT5)	
qt5_wrap_cpp(MOC_SRC ${moc_header})
endif()

source_group("moc" FILES ${MOC_SRC})
source_group("src" FILES ${SOURCES})
add_library(GlobalShortcut SHARED ${SOURCES} ${UI_SRC} ${MOC_SRC}  ${RE_SRC})

target_link_libraries(GlobalShortcut ${QT_LIB})
SET_BIN_DIR(GlobalShortcut GlobalShortcut)