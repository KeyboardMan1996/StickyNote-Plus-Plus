cmake_minimum_required(VERSION 3.14)
project(StickyNoteFile)
include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/xml
	${QT5_INCLUDE_DIR}
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
set(SRC
	NoteFile.h
	NoteFile.cpp
	Property.cpp
	Property.h
	Setting.cpp
	Setting.h
	ImageSave.cpp
	ImageSave.h
)
set(libs
	XML
)
add_library(StickyNoteFile SHARED ${SRC})
target_link_libraries(StickyNoteFile ${libs} ${QT_LIB})
source_group("src" FILES ${SRC})
add_definitions(-D_SRICKY_NOTE_FILE_)
SET_BIN_DIR(StickyNoteFile StickyNoteFile)