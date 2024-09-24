#include "drawerWidget.h"
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QSharedPointer>
#include <mutex>
#include <QStyleOption>
#include <QPainter>
namespace OriginUI {
	class DrawerWidgetD {
	public:
		DrawerWidgetD(QWidget* widget);
		~DrawerWidgetD();
	public:
		//参数控制器
		 QPropertyAnimation *pushAnimation,  *popAnimation;
		 //窗口布局
		 QVBoxLayout* layout;
		 //保证中心窗口只被设置一次
		 std::once_flag flag;
	};



}

OriginUI::DrawerWidgetD::DrawerWidgetD(QWidget* widget)
{
	popAnimation = new QPropertyAnimation(widget, "pos");
	popAnimation->setDuration(500);
	pushAnimation = new QPropertyAnimation(widget, "pos");
	pushAnimation->setDuration(500);

	//初始化布局
	layout = new QVBoxLayout();
	widget->setLayout(layout);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	layout->setContentsMargins(0, 0, 0, 0);
#else
	layout->setMargin(0);
#endif
	
	layout->setSpacing(0);
}

OriginUI::DrawerWidgetD::~DrawerWidgetD()
{
	delete popAnimation;
	delete pushAnimation;
	delete layout;
}


OriginUI::DrawerWidget::DrawerWidget(QWidget* parent /*= 0*/)
	:QWidget(parent)
{
	d = new DrawerWidgetD(this);
}

OriginUI::DrawerWidget::~DrawerWidget()
{
	delete d;
}

/**
* @brief OriginUI::DrawerWidget::pop 弹出窗口
* @return void
*/
void OriginUI::DrawerWidget::pop()
{
	if (d->popAnimation->state() == QPropertyAnimation::Running)
		return;
	if (d->pushAnimation->state() == QPropertyAnimation::Running)
		d->pushAnimation->stop();

	d->popAnimation->setStartValue(getPopStartPos());
	d->popAnimation->setEndValue(getPopEndPos());
	d->popAnimation->start();
}

/**
* @brief OriginUI::DrawerWidget::push 收起窗口
* @return void
*/
void OriginUI::DrawerWidget::push()
{
	if (d->pushAnimation->state() == QPropertyAnimation::Running)
		return;
	if (d->popAnimation->state() == QPropertyAnimation::Running)
		d->popAnimation->stop();

	d->pushAnimation->setStartValue(getPushStartPos());
	d->pushAnimation->setEndValue(getPushEndPos());
	d->pushAnimation->start();
}

QPoint OriginUI::DrawerWidget::getPopStartPos()
{
	return this->pos();
}

QPoint OriginUI::DrawerWidget::getPopEndPos()
{
	auto p = this->pos();
	p.setY(0);
	return p;
}

QPoint OriginUI::DrawerWidget::getPushEndPos()
{
	auto p = this->pos();
	p.setY(-this->height());
	return p;
}

QPoint OriginUI::DrawerWidget::getPushStartPos()
{
	return this->pos();
}

QPropertyAnimation* OriginUI::DrawerWidget::getPopAnimation()
{
	return d->popAnimation;
}

QPropertyAnimation* OriginUI::DrawerWidget::getPushAnimaiton()
{
	return d->pushAnimation;
}



void OriginUI::DrawerWidget::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	opt.initFrom(this);
#else
	opt.init(this);
#endif	
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OriginUI::DrawerWidget::moveEvent(QMoveEvent* event)
{
	QWidget::moveEvent(event);
	emit widgetMoveSignal(this->pos());
}

/**
* @brief OriginUI::DrawerWidget::setPopTime 设置弹出动画时间
* @param unsigned int mec 毫秒
* @return void
*/
void OriginUI::DrawerWidget::setPopTime(unsigned int mec)
{
	d->popAnimation->setDuration(mec);
}

/**
* @brief OriginUI::DrawerWidget::setPushTime 设置收回动画时间
* @param unsigned int mec 毫秒
* @return void
*/
void OriginUI::DrawerWidget::setPushTime(unsigned int mec)
{
	d->pushAnimation->setDuration(mec);
}

/**
* @brief OriginUI::DrawerWidget::setCenterWidget 设置中心窗口 只有第一次调用有效
* @param QWidget * widget
* @return void
*/
void OriginUI::DrawerWidget::setCenterWidget(QWidget* widget)
{
	std::call_once(d->flag, [&]() {
		d->layout->addWidget(widget);
		//widget->setParent(this);
	});
}

void OriginUI::DrawerWidget::initPos()
{
	move(0, 0);
}