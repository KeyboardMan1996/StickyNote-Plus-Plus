#include "WidgetNoBoder.h"
#include <windows.h>
#include <windowsx.h>
#include <QVBoxLayout>
namespace OriginUI {
	/*
		数据类
	*/
	class WidgetNoBoderD {
	public:
		WidgetNoBoderD();
	public:
		//响应鼠标边框悬停的范围
		int boundaryWidth;
	};

	WidgetNoBoderD::WidgetNoBoderD()
	{
		boundaryWidth = 6;

	}
}


OriginUI::WidgetNoBoder::WidgetNoBoder(QWidget* parent /*= 0*/)
{
	d = new WidgetNoBoderD();
	this->setWindowFlags(Qt::FramelessWindowHint);

}

OriginUI::WidgetNoBoder::~WidgetNoBoder()
{
	delete d;
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
bool OriginUI::WidgetNoBoder::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool OriginUI::WidgetNoBoder::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
	//如果需要窗口置于底层时
	//当窗口位置发生改变时，将窗口重置窗口的位置
#if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
	MSG* msg = *static_cast<MSG**>(message);
#else
	MSG* msg = static_cast<MSG*>(message);
#endif
	switch (msg->message)
	{
	case WM_NCHITTEST:
	{
		const LONG border_width = d->boundaryWidth;
		RECT winrect{};
		GetWindowRect(reinterpret_cast<HWND>(this->winId()), &winrect);

		const long x = GET_X_LPARAM(msg->lParam);
		const long y = GET_Y_LPARAM(msg->lParam);

		if (!IsZoomed(msg->hwnd)) {
			const bool resizeWidth = minimumWidth() != maximumWidth();
			const bool resizeHeight = minimumHeight() != maximumHeight();

			if (resizeWidth) {
				// left border
				if (x >= winrect.left && x < winrect.left + border_width) {
					*result = HTLEFT;
				}
				// right border
				if (x < winrect.right && x >= winrect.right - border_width) {
					*result = HTRIGHT;
				}
			}
			if (resizeHeight) {
				// bottom border
				if (y < winrect.bottom && y >= winrect.bottom - border_width) {
					*result = HTBOTTOM;
				}
				// top border
				if (y >= winrect.top && y < winrect.top + border_width) {
					*result = HTTOP;
				}
			}
			if (resizeWidth && resizeHeight) {
				// bottom left corner
				if (x >= winrect.left && x < winrect.left + border_width && y < winrect.bottom && y >= winrect.bottom - border_width) {
					*result = HTBOTTOMLEFT;
				}
				// bottom right corner
				if (x < winrect.right && x >= winrect.right - border_width && y < winrect.bottom && y >= winrect.bottom - border_width) {
					*result = HTBOTTOMRIGHT;
				}
				// top left corner
				if (x >= winrect.left && x < winrect.left + border_width && y >= winrect.top && y < winrect.top + border_width) {
					*result = HTTOPLEFT;
				}
				// top right corner
				if (x < winrect.right && x >= winrect.right - border_width && y >= winrect.top && y < winrect.top + border_width) {
					*result = HTTOPRIGHT;
				}
			}
		}

		if (0 != *result)
			return true;

	}
	}
	return false;         //此处返回false，留给其他事件处理器处理
}

