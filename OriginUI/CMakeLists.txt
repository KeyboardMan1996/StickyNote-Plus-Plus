#设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)
project(OriginUI)
#设置C++版本
set(CMAKE_CXX_STANDARD 11)

#判断是否要兼容WIN10以下的系统
if(BUILD_TO_WIN7)
	add_definitions(-D_RUN_WITH_WIN7_)
endif()

add_definitions(-D_ORIGIN_UI_)

#添加头文件目录
include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/Log/src
	${CMAKE_SOURCE_DIR}/xml
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
SET(SOURCES main.cpp 
	ToolBar.h
	ToolBar.cpp
	drawerWidget.cpp
	drawerWidget.h
	DrawerWidgetTraceMouse.h
	DrawerWidgetTraceMouse.cpp
	DrawerWidgetReverse.cpp
	DrawerWidgetReverse.h
	WidgetNoBoder.cpp
	WidgetNoBoder.h
	SimpleRichTextEdit.cpp
	SimpleRichTextEdit.h
	ToolButtonColor.cpp
	ToolButtonColor.h
	Action.cpp
	Action.h
	NavigationBar.cpp
	NavigationBar.h
	InputLineEdit.cpp
	InputLineEdit.h
	KeyboardLineEdit.cpp
	KeyboardLineEdit.h
	KeyboardToString.hpp
	WindowNoBoder.cpp
	WindowNoBoder.h
	RichTextEdit.cpp
	RichTextEdit.h
	DelayDialog.cpp
	DelayDialog.h
	circleSwitch.cpp
	circleSwitch.h
	EditLable.h
	EditLable.cpp
	TempTextEdit.cpp
	TempTextEdit.h
	NativeWindowTemplate.hpp
	widgetSplitter.h
	widgetSplitter.cpp
	)
	
set(moc_header
	DrawerWidgetTraceMouse.h
	drawerWidget.h
	ToolBar.h
	WidgetNoBoder.h
	SimpleRichTextEdit.h
	ToolButtonColor.h
	Action.h
	NavigationBar.h
	InputLineEdit.h
	WindowNoBoder.h
	DelayDialog.h
	TempTextEdit.h
	EditLable.h
)

if(BUILD_QT6)
	qt6_wrap_cpp(MOC_SRC ${moc_header})
	qt6_wrap_ui(UI_SRC DelayDialog.ui)
	QT6_ADD_RESOURCES(RE_SRC resource.qrc)
endif()

if(BUILD_QT5)
	qt5_wrap_cpp(MOC_SRC ${moc_header})
	qt5_wrap_ui(UI_SRC DelayDialog.ui)
	QT5_ADD_RESOURCES(RE_SRC resource.qrc)
endif()


source_group("moc" FILES ${MOC_SRC})
source_group("ui_src" FILES ${UI_SRC})
source_group("src" FILES ${SOURCES})
add_library(OriginUI SHARED ${SOURCES} ${UI_SRC} ${MOC_SRC}  ${RE_SRC})

set(LIB_List
	CPP_LOG
	XML
	dwmapi.lib
)

target_link_libraries(OriginUI ${QT_LIB} ${LIB_List})
SET_BIN_DIR(OriginUI OriginUI)