#include "widgetSplitter.h"
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>

namespace OriginUI {
	class LineWidgetD {
	public:
		QColor color;

	};

	class WidgetSplitterD {
	public:
		LineWidget* leiftLine, * rightLine;
		QLabel* label;
		QColor fontColor;
	};
}



OriginUI::LineWidget::LineWidget(QWidget* parent /*= 0*/)
	:QWidget(parent),d(new LineWidgetD())
{
	d->color = Qt::gray;
}

OriginUI::LineWidget::~LineWidget()
{
	delete d;
}

void OriginUI::LineWidget::setColor(const QColor& color)
{
	d->color = color;
}

QColor OriginUI::LineWidget::getColor()
{
	return d->color;
}

void OriginUI::LineWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), d->color);
	QWidget::paintEvent(event);
}

OriginUI::WidgetSplitter::WidgetSplitter(QWidget* parent /*= 0*/)
	:QWidget(parent),d(new WidgetSplitterD())
{
	initGui();
}

OriginUI::WidgetSplitter::~WidgetSplitter()
{
	delete d;
}

void OriginUI::WidgetSplitter::setFontColor(const QColor& color)
{

}

QColor OriginUI::WidgetSplitter::getFontColor()
{
	return Qt::gray;
}

void OriginUI::WidgetSplitter::setLineColor(const QColor& color)
{
	d->rightLine->setColor(color);
	d->leiftLine->setColor(color);
}
QColor OriginUI::WidgetSplitter::getLineColor()
{
	return d->leiftLine->getColor();
}

void OriginUI::WidgetSplitter::setText(const QString& text)
{
	d->label->setText(text);
}

QString OriginUI::WidgetSplitter::getText()
{
	return d->label->text();
}

void OriginUI::WidgetSplitter::setLineHeight(const int& height)
{
	d->leiftLine->setFixedHeight(height);
	d->rightLine->setFixedHeight(height);
}

int OriginUI::WidgetSplitter::getLineHeight()
{
	return d->leiftLine->height();
}

void OriginUI::WidgetSplitter::initGui()
{
	setLayout(new QHBoxLayout(this));

	d->leiftLine = new LineWidget(this);
	d->leiftLine->setColor(Qt::gray);
	d->rightLine = new LineWidget(this);
	d->rightLine->setColor(Qt::gray);

	d->leiftLine->setFixedHeight(2);
	d->rightLine->setFixedHeight(2);

	QSizePolicy lineSizePolicy, labelSizePolicy;
	lineSizePolicy.setHorizontalPolicy(QSizePolicy::Preferred);
	lineSizePolicy.setVerticalPolicy(QSizePolicy::Fixed);

	labelSizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);
	labelSizePolicy.setVerticalPolicy(QSizePolicy::Fixed);

	d->leiftLine->setSizePolicy(lineSizePolicy);
	d->rightLine->setSizePolicy(lineSizePolicy);


	d->label = new QLabel(this);
	d->label->setSizePolicy(labelSizePolicy);

	this->layout()->addWidget(d->leiftLine);
	this->layout()->addWidget(d->label);
	this->layout()->addWidget(d->rightLine);

	
}
