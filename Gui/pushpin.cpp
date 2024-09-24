#include "pushpin.h"
#include "StickyNote.h"
extern "C"
{
	#include "SetWindowState.h"
	#include "FindWindow.h"
}
#include <QWidget>
#include <cassert>
#include <windows.h>
#include <windowsx.h>
#include <iostream>

NGui::Pushpin::Pushpin()
{

}

NGui::Pushpin::~Pushpin()
{

}

/**
* @brief NGui::Pushpin::toTop 将widget置于顶层窗口
* @return bool
*/
bool NGui::Pushpin::toTop(StickyNote* widget)
{
	setWindowAlwaysTop((HWND)widget->winId());
	widget->show();
	return true;
}

/**
* @brief NGui::Pushpin::toDesktop 将widget置于桌面窗口
* @return bool
*/
bool NGui::Pushpin::toDesktop(StickyNote* widget)
{
	widget->lower();
	widget->setOnlyBottom(true);
	return true;
}

bool NGui::Pushpin::toNormal(StickyNote* widget)
{
	setWindowNormal((HWND)widget->winId());
	widget->show();
	return true;
}

bool NGui::Pushpin::isTop(StickyNote* widget)
{
	return isWindowAlwaysTop((HWND)widget->winId());
}



bool NGui::Pushpin::isDesktop(StickyNote* widget)
{
	return widget->getOnlyBottom();
}

bool NGui::Pushpin::isNormal(StickyNote* widget)
{
	return isWindowNormal((HWND)widget->winId());
}
