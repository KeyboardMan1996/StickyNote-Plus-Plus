#include "circleSwitch.h"
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
namespace OriginUI {
	class CircleSwitchD{
	public:
		CircleSwitchD();
		QColor backgroundColor, checkBackgroundColor, color;
	};

	CircleSwitchD::CircleSwitchD()
		:backgroundColor("#CCCCCC"),checkBackgroundColor("#FF6666"),color("#0099CC")
	{
		
	}

}

OriginUI::CircleSwitch::CircleSwitch(QWidget* parent /*= 0*/)
	:QToolButton(parent),d(new CircleSwitchD)
{
	setCheckable(true);
}

OriginUI::CircleSwitch::~CircleSwitch()
{
	delete d;
}

void OriginUI::CircleSwitch::setBackgroundColor(const QColor& color)
{
	d->backgroundColor = color;
}

QColor OriginUI::CircleSwitch::getBackgroundColor()
{
	return d->backgroundColor;
}

void OriginUI::CircleSwitch::setCheckBackgroundColor(const QColor& color)
{
	d->checkBackgroundColor = color;
}

QColor OriginUI::CircleSwitch::getCheckBackgroundColor()
{
	return d->checkBackgroundColor;
}

void OriginUI::CircleSwitch::setColor(const QColor& color)
{
	d->color = color;
}

QColor OriginUI::CircleSwitch::getColor()
{
	return d->color;
}

void OriginUI::CircleSwitch::paintEvent(QPaintEvent* event)
{
	QToolButton::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);

	if (isChecked())
	{
		painter.setPen(d->checkBackgroundColor);
		painter.setBrush(d->checkBackgroundColor);
	}else {
		painter.setPen(d->backgroundColor);
		painter.setBrush(d->backgroundColor);
	}
	//绘制时不必铺满整个窗口
	auto re = this->rect();
	const int margin = 3;
	re.setX(margin);
	re.setY(margin);
	re.setWidth(re.width() - 2 * margin);
	re.setHeight(re.height() - 2 * margin);

	painter.drawEllipse(re);
	
	QPen pen;
	pen.setColor(d->color);
	pen.setBrush(d->color);
	pen.setWidth(3);
	painter.setPen(pen);
	QPainterPath path;
	path.moveTo(re.width() * 0.2 + margin, re.height() * 0.5 + margin);
	path.lineTo(re.width() * 0.5 + margin, re.height() * 0.7 + margin);
	path.lineTo(re.width() * 0.8 + margin, re.height() * 0.3 + margin);
	painter.drawPath(path);
}

OriginUI::DeleteButton::DeleteButton(QWidget* parent)
	:CircleSwitch(parent)
{

}

void OriginUI::DeleteButton::paintEvent(QPaintEvent* event)
{
	QToolButton::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);

	auto checkBackgroundColor = getCheckBackgroundColor();
	auto backgroundColor = getBackgroundColor();
	auto color = getColor();

	if (isChecked())
	{
		painter.setPen(checkBackgroundColor);
		painter.setBrush(checkBackgroundColor);
	}
	else {
		painter.setPen(backgroundColor);
		painter.setBrush(backgroundColor);
	}
	//绘制时不必铺满整个窗口
	auto re = this->rect();
	const int margin = 3;
	re.setX(margin);
	re.setY(margin);
	re.setWidth(re.width() - 2 * margin);
	re.setHeight(re.height() - 2 * margin);

	painter.drawEllipse(re);

	QPen pen;
	pen.setColor(color);
	pen.setBrush(color);
	pen.setWidth(4);
	painter.setPen(pen);
	QPainterPath path;
	path.moveTo(re.width() * 0.2 + margin, re.height() * 0.5 + margin);
	path.lineTo(re.width() * 0.8 + margin, re.height() * 0.5 + margin);
	painter.drawPath(path);
}

