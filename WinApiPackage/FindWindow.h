#ifndef __FIND_WINDOW_H__
#define __FIND_WINDOW_H__
#include "WinApiExport.h"
#include <windows.h>

//查找图标层的窗口句柄
WIN_API_EXPORT HWND findDesktopIconWindow();

//判断窗口是否为父子关系
//似乎多层父子关系也会返回TRUE  暂时无法解决
WIN_API_EXPORT BOOL isChild(HWND parent, HWND child);

//判断是否是一个桌面窗口
WIN_API_EXPORT BOOL isDeskWindow(HWND hd);

//查找壁纸层
WIN_API_EXPORT HWND findWallpaperLayer();

#endif
