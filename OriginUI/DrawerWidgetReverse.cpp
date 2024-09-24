#include "DrawerWidgetReverse.h"
#include <QPropertyAnimation>
#include <iostream>
OriginUI::DrawerWidgetReverse::DrawerWidgetReverse(QWidget* parent /*= 0*/)
	:DrawerWidget(parent)
{

}

OriginUI::DrawerWidgetReverse::~DrawerWidgetReverse()
{

}

void OriginUI::DrawerWidgetReverse::initPos()
{
	move(0, this->width());
}



QPoint OriginUI::DrawerWidgetReverse::getPopEndPos()
{
	auto pos = this->pos();
	return QPoint(0, 0);
}

QPoint OriginUI::DrawerWidgetReverse::getPushEndPos()
{
	auto p = this->pos();
	p.setY(this->height());
	return p;
}

