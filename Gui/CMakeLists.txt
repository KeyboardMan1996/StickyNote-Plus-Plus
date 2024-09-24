#设置cmake的最低版本
cmake_minimum_required(VERSION 3.10)
project(StickyNote)
#设置C++版本
set(CMAKE_CXX_STANDARD 11)

#判断是否要兼容WIN10以下的系统
if(BUILD_TO_WIN7)
	add_definitions(-D_RUN_WITH_WIN7_)
endif()

#添加头文件目录
include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Log/src
	${CMAKE_SOURCE_DIR}/OriginUI
	${CMAKE_SOURCE_DIR}/StickyNoteFile
	${CMAKE_SOURCE_DIR}/WinApiPackage
	${CMAKE_SOURCE_DIR}/GlobalShortcut/gui
	${CMAKE_SOURCE_DIR}/GlobalShortcut/core
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
SET(main_src main.cpp 
	mainWindow.h 
	mainWindow.cpp
	pushpin.cpp
	pushpin.h
	SystemTrayIcon.cpp
	SystemTrayIcon.h
	StickyNotePool.cpp
	StickyNotePool.h
	AppIcon.rc
	ProcessCommunication.cpp
	ProcessCommunication.h
	ActionCommand.cpp
	ActionCommand.h
	StickyNoteSlot.cpp
	StickyNoteSlot.h
	ActionCommandStateTimer.cpp
	ActionCommandStateTimer.h
	StickyNoteContentChangeEvent.cpp
	StickyNoteContentChangeEvent.h
	EventManager.cpp
	EventManager.h
	EventSender.h
	EventSender.cpp
	CompatibilityTool.h
	CompatibilityTool.cpp
	)
SET (sticky_note_ui_src
	NoteTextEdit.cpp
	NoteTextEdit.h
	StickyNote.cpp
	StickyNote.h
	ColorWidget.cpp
	ColorWidget.h
	StickyNoteMagnetic.cpp
	StickyNoteMagnetic.h
	ToDoListItem.cpp
	ToDoListItem.h
	StickyNoteContent.h
	StickyNoteContent.cpp
	ToDoListContent.cpp
	ToDoListContent.h
	StickyNoteText.cpp
	StickyNoteText.h
	StickyNoteToDoList.cpp
	StickyNoteToDoList.h
	StickyNoteFactory.cpp
	StickyNoteFactory.h	
	)
SET (history_ui_src
	StickyNoteItem.cpp
	StickyNoteItem.h
	HistoryWidget.cpp
	HistoryWidget.h
	StickyNoteItemContent.cpp
	StickyNoteItemContent.h
	)
SET(diversity_sticky_note_src
	DiversityContent.cpp
	DiversityContent.h
	DiversityItem.h
	StickyNoteDiversity.h
	StickyNoteDiversity.cpp
)

set(moc_header
	StickyNote.h
	mainWindow.h
	NoteTextEdit.h
	SystemTrayIcon.h
	ColorWidget.h
	ProcessCommunication.h
	StickyNoteSlot.h
	ActionCommandStateTimer.h
	StickyNoteItem.h
	HistoryWidget.h
	EventManager.h
	SettingUI.h
	AutoVersion.h
	ToDoListItem.h
	ToDoListContent.h
)
set(setting_src
	Setting.cpp
	Setting.h
	SettingUI.cpp
	SettingUI.h
	AutoVersion.cpp
	AutoVersion.h
	UpdateUI.cpp
	UpdateUI.h
)
#添加外部库依赖目录
if(BUILD_QT6)
	qt6_wrap_cpp(MOC_SRC ${moc_header})
	qt6_wrap_ui(UI_SRC mainWindow.ui SettingUI.ui UpdateUI.ui ToDoListItem.ui)
	QT6_ADD_RESOURCES(RE_SRC resource.qrc)
endif()

if(BUILD_QT5)
	qt5_wrap_cpp(MOC_SRC ${moc_header})
	qt5_wrap_ui(UI_SRC mainWindow.ui SettingUI.ui UpdateUI.ui ToDoListItem.ui)
	QT5_ADD_RESOURCES(RE_SRC resource.qrc)
endif()
	


set(src
	${main_src}
	${UI_SRC} 
	${MOC_SRC}
	resource.qrc
	${RE_SRC}
	${sticky_note_ui_src}
	${history_ui_src}
	${setting_src}
	${diversity_sticky_note_src}
	)
#生成翻译文件
if(BUILD_QT5)
	qt5_create_translation(QM_FILES ${src} ./translate/StickyNoteGui.ts)
endif()

source_group("moc" FILES ${MOC_SRC})
source_group("ui_src" FILES ${UI_SRC})
source_group("src" FILES ${main_src})
source_group("stickyNoteUI" FILES ${sticky_note_ui_src})
source_group("historyUI"  FILES ${history_ui_src})
source_group("setting" FILES ${setting_src})
source_group("diversityNote" FILES ${diversity_sticky_note_src})
add_executable(StickyNote ${src} ${QM_FILES})

set(LIB_List
	CPP_LOG
	OriginUI
	StickyNoteFile
	WinApiPackage
	GlobalShortcut
)

target_link_libraries(StickyNote ${QT_LIB} ${LIB_List})
SET_BIN_DIR(StickyNote StickyNote)
#屏蔽控制台窗口
Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")