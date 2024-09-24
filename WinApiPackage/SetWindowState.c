#include "SetWindowState.h"
#include "FindWindow.h"
#include <windowsx.h>
void setWindowAlwaysTop(HWND winID)
{
	if(!isDeskWindow(winID))
		SetParent(winID, GetDesktopWindow());

	SetWindowPos(winID, HWND_TOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

BOOL isWindowAlwaysTop(HWND winID)
{
	if (!isChild(GetDesktopWindow(), winID))
		return FALSE;

	auto style = GetWindowExStyle(winID);
	if (style & WS_EX_TOPMOST)
		return TRUE;
	return FALSE;
}

void setWindowNormal(HWND winID)
{
	if (!isDeskWindow(winID))
		SetParent(winID, GetDesktopWindow());

	SetWindowPos(winID, HWND_NOTOPMOST, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

BOOL isWindowNormal(HWND winID)
{
	//判断窗口是否为桌面窗口
	if (!isDeskWindow(winID))
		return FALSE;
	//判断窗口样式是否置于顶层
	auto style = GetWindowExStyle(winID);
	if (style & WS_EX_TOPMOST)
		return FALSE;

	return TRUE;
}

void setWindowToIconLayer(HWND winID)
{
	setWindowNormal(winID);

	HWND desktopHwnd = findDesktopIconWindow();
	if (!desktopHwnd)
		return ;
	SetParent(winID, desktopHwnd);
}

BOOL isWindowToIconLayer(HWND winID)
{
	HWND desktopHwnd = findDesktopIconWindow();
	if (!desktopHwnd)
		return FALSE;

	return isChild(desktopHwnd,winID);
}

