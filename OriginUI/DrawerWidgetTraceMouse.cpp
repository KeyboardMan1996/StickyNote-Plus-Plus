#include "DrawerWidgetTraceMouse.h"
#include <iostream>

namespace OriginUI {
	class DrawerWidgetTraceMouseD {
	public:
		DrawerWidgetTraceMouseD();
	public:
		QPoint mousePressPos;
		bool mousePress;
	};

	DrawerWidgetTraceMouseD::DrawerWidgetTraceMouseD()
		:mousePressPos(0,0)
	{
		mousePress = false;
	}

}

OriginUI::DrawerWidgetTraceMouse::DrawerWidgetTraceMouse(QWidget* parent /*= 0*/)
{
	d = new DrawerWidgetTraceMouseD;
}

OriginUI::DrawerWidgetTraceMouse::~DrawerWidgetTraceMouse()
{
	delete d;
}

void OriginUI::DrawerWidgetTraceMouse::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		d->mousePress = true;
		d->mousePressPos = event->pos();
	}
}

void OriginUI::DrawerWidgetTraceMouse::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		d->mousePress = false;
		emit leftButtonRelease();
	}
}

void OriginUI::DrawerWidgetTraceMouse::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
	if (d->mousePress)
		Q_EMIT mouseMove(event->pos() - d->mousePressPos);
}

