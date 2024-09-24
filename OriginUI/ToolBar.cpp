#include "ToolBar.h"
#include "drawerWidget.h"
#include <QHBoxLayout>
#include <iostream>

//test
#include <QToolButton>
#include "DrawerWidgetTraceMouse.h"

namespace OriginUI {
	class ToolBarD {
	public:
		ToolBarD();
		~ToolBarD();

		//动画窗口
		DrawerWidget* drawerWidget; 
		//承载主要内容的窗口
		QWidget* toolBar;
		//控制动画窗口是否自动弹出或者收回
		bool autoPop, autoPush;
		//用于记录插入的窗口
		std::vector<QWidget*> insertWidgets;
		//弹出窗口的高度
		//高度一直保持不变
		

	};

	ToolBarD::ToolBarD()
	{
		autoPop = true;
		autoPush = true;
		toolBar = nullptr;
		drawerWidget = nullptr;
	}

	ToolBarD::~ToolBarD()
	{
		delete toolBar;
		delete drawerWidget;
	}


}


OriginUI::ToolBar::ToolBar(QWidget* parent /*= 0*/)
	:QWidget(parent),d(new ToolBarD)
{
	initGui();
}

OriginUI::ToolBar::ToolBar(DrawerWidget* dw, QWidget* parent /*= 0*/)
	:QWidget(parent), d(new ToolBarD)
{

	initGui(dw);
}

OriginUI::ToolBar::~ToolBar()
{
	delete d;
}

void OriginUI::ToolBar::initGui(DrawerWidget* dw /*= nullptr*/)
{
	if (dw)
	{
		d->drawerWidget = dw;
		dw->setParent(this);
	}else{
		d->drawerWidget = new DrawerWidgetTraceMouse(this);
	}
	d->drawerWidget->initPos();
	d->drawerWidget->setFixedSize(this->size());
//	d->drawerWidget->push();

	d->toolBar = new QWidget();
	d->toolBar->setLayout(new QHBoxLayout());
	d->toolBar->layout()->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	d->toolBar->layout()->setSpacing(2);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	d->toolBar->layout()->setContentsMargins(0, 0, 0, 0);
#else
	d->toolBar->layout()->setMargin(0);
#endif

	
	d->toolBar->show();
	d->drawerWidget->setCenterWidget(d->toolBar);
}


/**
* @brief OriginUI::ToolBar::addWidget 在当行栏中增加widget ，默认使用水平布局，向右靠拢
* @param QWidget * Widget
* @return void
*/
void OriginUI::ToolBar::addWidget(QWidget* Widget)
{
	d->insertWidgets.push_back(Widget);
	d->toolBar->layout()->addWidget(Widget);
}

void OriginUI::ToolBar::clear()
{
	for (auto widget : d->insertWidgets)
	{
		d->toolBar->layout()->removeWidget(widget);
		delete widget;
	}
	d->insertWidgets.clear();
}

void OriginUI::ToolBar::push()
{
	d->drawerWidget->push();
}

void OriginUI::ToolBar::pop()
{
	d->drawerWidget->pop();
}

void OriginUI::ToolBar::setAlignment(Qt::AlignmentFlag flag)
{
	d->toolBar->layout()->setAlignment(flag);
}

void OriginUI::ToolBar::setAutoPop(const bool& b)
{
	d->autoPop = b;
}

bool OriginUI::ToolBar::getAutoPop()
{
	return d->autoPop;
}

void OriginUI::ToolBar::setAutoPush(const bool& b)
{
	d->autoPush = b;
}

bool OriginUI::ToolBar::getAutoPush()
{
	return d->autoPush;
}

QWidget* OriginUI::ToolBar::getToolbarWidget()
{
	return d->toolBar;
}

void OriginUI::ToolBar::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	d->drawerWidget->setFixedSize(this->size());
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void OriginUI::ToolBar::enterEvent(QEnterEvent* event)
#else
void OriginUI::ToolBar::enterEvent(QEvent* event)
#endif
{
	QWidget::enterEvent(event);
	if(d->autoPop)
		d->drawerWidget->pop();
}

void OriginUI::ToolBar::leaveEvent(QEvent* event)
{
	QWidget::leaveEvent(event);
	if(d->autoPush)
		d->drawerWidget->push();
}


OriginUI::DrawerWidget* OriginUI::ToolBar::getDrawerWidget()
{
	return d->drawerWidget;
}

namespace OriginUI {
	class ToolBarDynamicD {
	public:
		ToolBarDynamicD();
		//工具栏的高度
		//这个高度不随主窗口的大小变化而变化
		int height;
		//窗口变化时的最小高度
		int miniHeight;
	};

	ToolBarDynamicD::ToolBarDynamicD()
		:height(20),miniHeight(10)
	{

	}
}

OriginUI::ToolBarDynamic::ToolBarDynamic(QWidget* parent /*= 0*/)
	:ToolBar(parent),d(new ToolBarDynamicD())
{
	init();
}


OriginUI::ToolBarDynamic::ToolBarDynamic(OriginUI::DrawerWidget* dw, QWidget* parent /*= 0*/)
	:ToolBar(dw,parent),d(new ToolBarDynamicD())
{
	init();
}


OriginUI::ToolBarDynamic::~ToolBarDynamic()
{
	delete d;
}

void OriginUI::ToolBarDynamic::init()
{
	connect(getDrawerWidget(), SIGNAL(widgetMoveSignal(QPoint)), this,SLOT(drawerWidgetMove(QPoint)),Qt::QueuedConnection);
}

void OriginUI::ToolBarDynamic::setMaxHeight(const int& height)
{
	d->height  = height;
	getDrawerWidget()->setFixedHeight(height);
	//resize(width(), height);
	setFixedHeight(height);
}

void OriginUI::ToolBarDynamic::pop()
{
	getDrawerWidget()->pop();
}

void OriginUI::ToolBarDynamic::push()
{
	getDrawerWidget()->push();
}

/**
* @brief OriginUI::ToolBarDynamic::drawerWidgetMove 当弹出窗口移动时，改变主窗口的大小，实现整个窗口隐藏的效果
* @param QPoint point
* @return void
*/
void OriginUI::ToolBarDynamic::drawerWidgetMove(QPoint point)
{
	auto drawerWidgetSize = getDrawerWidget()->size();
	int h = drawerWidgetSize.height() + point.y();

	//当高度小于最小高度时按照最小高度改变大小
	h = h > d->miniHeight ? h : d->miniHeight;

	setFixedHeight(h);
}

/**
* @brief OriginUI::ToolBarDynamic::resizeEvent ToolBar中设置了让弹出窗口跟随主窗口的的大小变化而变化
* 再这里重新将机制设置为，只有宽度跟随主窗口变化。
* @param QResizeEvent * event
* @return void
*/
void OriginUI::ToolBarDynamic::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	getDrawerWidget()->setFixedWidth(this->width());
}
