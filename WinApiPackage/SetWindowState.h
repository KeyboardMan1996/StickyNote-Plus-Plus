#ifndef __SET_WINDOW_STATE_H__
#define __SET_WINDOW_STATE_H__
#include "WinApiExport.h"
#include <windows.h>

//将窗口固定到顶层
WIN_API_EXPORT void setWindowAlwaysTop(HWND winID);
//判断窗口是否置于顶层
WIN_API_EXPORT BOOL isWindowAlwaysTop(HWND winID);

//设置窗口为普通窗口
WIN_API_EXPORT void setWindowNormal(HWND winID);
//判断窗口是否为普通窗口
WIN_API_EXPORT BOOL isWindowNormal(HWND winID);

//将窗口置于图标层
WIN_API_EXPORT void setWindowToIconLayer(HWND winID);
//判断窗口是否在图标层
WIN_API_EXPORT BOOL isWindowToIconLayer(HWND winID);



#endif
