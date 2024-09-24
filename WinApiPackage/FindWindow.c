#include "FindWindow.h"
#include <stdlib.h>
#include <malloc.h>


/**
* @brief enumWindowFindDesktopIconWindow  enumWindows的回调函数，在回调中查找图标层窗口
* @param HWND hwnd  窗口句柄
* @param LPARAM lParam 回调参数
* @return BOOL  返回False 停止枚举
*/
BOOL enumWindowFindDesktopIconWindow(HWND hwnd, LPARAM lParam)
{
	long wflags = GetWindowLong(hwnd, GWL_STYLE);
	if (!(wflags & WS_VISIBLE))
		return TRUE;
	HWND sndWnd;
	if (!(sndWnd = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL)))
		return TRUE;
	HWND targetWnd;
	if (!(targetWnd = FindWindowEx(sndWnd, NULL, "SysListView32", "FolderView")))
		return TRUE;
	HWND* resultHwnd = (HWND*)lParam;
	/*
	* 发现有的桌面上有其他软件覆盖在图标层之上，所以直接将窗口置于顶层
	*/
	*resultHwnd = hwnd;
	return FALSE;
}


/**
* @brief findDesktopIconWindow 查找图标层的窗口ID
* @return HWND
*/
HWND findDesktopIconWindow()
{
	HWND resultHwnd = NULL;
	EnumWindows((WNDENUMPROC)enumWindowFindDesktopIconWindow, (LPARAM)&resultHwnd);
	return resultHwnd;
}

typedef struct{
	HWND childID; //窗口ID
	BOOL isChild;

} IsChildPar;

BOOL enumWindowFindWindow(HWND winID, LPARAM par)
{
	IsChildPar* p = (IsChildPar*)par;

	if (winID == p->childID)
	{
		p->isChild = TRUE;
		return FALSE;
	}
	return TRUE;
};

BOOL isChild(HWND parent, HWND child)
{
	/*
	由于未知原因，使用GetParent()和IsChild()都无法正常工作
	所以只能枚举所有子窗口，然后寻找到对应的child由此验证是否为父子关系
	*/
	IsChildPar* par = malloc(sizeof(IsChildPar));

	par->childID = child;
	par->isChild = FALSE;

	EnumChildWindows(parent, enumWindowFindWindow, (LPARAM)par);

	BOOL rt = par->isChild;
	free(par);
	return rt;
}

WIN_API_EXPORT BOOL isDeskWindow(HWND hd)
{
	IsChildPar* par = malloc(sizeof(IsChildPar));

	par->childID = hd;
	par->isChild = FALSE;

	EnumWindows(enumWindowFindWindow, (LPARAM)par);

	BOOL rt = par->isChild;
	free(par);
	return rt;
}



BOOL enumWindowFindWallpaperLayer(HWND winID, LPARAM par)
{
	long wflags = GetWindowLong(winID, GWL_STYLE);
	if (!(wflags & WS_VISIBLE))
		return TRUE;
	HWND sndWnd;
	if (!(sndWnd = FindWindowEx(winID, NULL, "SHELLDLL_DefView", NULL)))
		return TRUE;
	HWND targetWnd;
	if (!(targetWnd = FindWindowEx(0, winID, L"WorkerW", 0)))
		return TRUE;
	HWND* resultHwnd = (HWND*)par;
	*resultHwnd = targetWnd;
	return FALSE;
}

HWND findWallpaperLayer()
{
	//Win10的壁纸层有两个，这样是为了避免切换壁纸时的闪烁
	//第二层壁纸只会在切换壁纸的时候出现，所以需要模拟切换壁纸发送一个消息
	int result;
	HWND windowHandle = FindWindow(L"Progman", NULL);
	SendMessageTimeout(windowHandle, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, (PDWORD_PTR)&result);
	
	HWND wallpaper = NULL;
	EnumWindows(enumWindowFindWallpaperLayer, wallpaper);

	return wallpaper;
}
