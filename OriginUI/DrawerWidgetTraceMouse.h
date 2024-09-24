#pragma  once
#include "drawerWidget.h"
#include <QMouseEvent>
#include <QPoint>
#include <QPaintEvent>
namespace OriginUI {
	class DrawerWidgetTraceMouseD;
	class ORIGIN_UI_EXPORT DrawerWidgetTraceMouse :public DrawerWidget {
		Q_OBJECT
	public:
		DrawerWidgetTraceMouse(QWidget* parent = 0);
		~DrawerWidgetTraceMouse();

	protected:
		void mousePressEvent(QMouseEvent* event);
		void mouseReleaseEvent(QMouseEvent* event);
		void mouseMoveEvent(QMouseEvent* event);

	Q_SIGNALS:
		void mouseMove(QPoint distance);
		void leftButtonRelease();
	private:
		DrawerWidgetTraceMouseD *d;
	};
}