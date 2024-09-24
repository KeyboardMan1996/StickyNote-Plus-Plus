#include "NavigationBar.h"
#include "LogTool.h"
#include <QHBoxLayout>
#include <QMouseEvent>
namespace OriginUI{
	class NavigationBarD {
	public:
		NavigationBarD(NavigationBar* bar);
		~NavigationBarD();

		QHBoxLayout* creatLayout();

		QHBoxLayout* leftLyout, * rightLyout, * layout;
		QPoint mousePressPos;
		bool mousePress;
		QSize iconSize, toolButtonMiniSize;
	};

	NavigationBarD::NavigationBarD(NavigationBar* bar)
		:mousePressPos(0,0),mousePress(false),iconSize(20,20),
		toolButtonMiniSize(32,32)
	{
		leftLyout = creatLayout();
		rightLyout = creatLayout();
		layout = creatLayout();
		

		layout->addLayout(leftLyout);
		layout->addStretch();
		layout->addLayout(rightLyout);
		bar->setLayout(layout);
	}

	NavigationBarD::~NavigationBarD()
	{
		leftLyout->deleteLater();
		rightLyout->deleteLater();
		layout->deleteLater();
	}

	QHBoxLayout* NavigationBarD::creatLayout()
	{
		auto layout = new QHBoxLayout();
		layout->setSpacing(0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		layout->setContentsMargins(0, 0, 0, 0);
#else
		layout->setMargin(0);
#endif
		return layout;
	}

}




OriginUI::NavigationBar::NavigationBar(QWidget* parent /*=0*/)
	:QWidget(parent)
{
	d = new NavigationBarD(this);
}

OriginUI::NavigationBar::~NavigationBar()
{
	delete d;
}


void OriginUI::NavigationBar::addLeftWidget(QWidget* widget)
{
	if (PtrIsNull(widget))
		return;
	widget->setParent(this);
	d->leftLyout->addWidget(widget);
}

void OriginUI::NavigationBar::addLeftWidget(QAction* action)
{
	if (PtrIsNull(action))
		return;
	auto button = creatToolButton(action);
	d->leftLyout->addWidget(button);
}

void OriginUI::NavigationBar::addRightWidget(QWidget* widget)
{
	if (PtrIsNull(widget))
		return;
	widget->setParent(this);
	d->rightLyout->addWidget(widget);
}

void OriginUI::NavigationBar::setIconSize(const QSize& size)
{
	d->iconSize = size;
}

void OriginUI::NavigationBar::setToolButtonMiniSize(const QSize& size)
{
	d->toolButtonMiniSize = size;
}

void OriginUI::NavigationBar::addRightWidget(QAction* action)
{
	if (PtrIsNull(action))
		return;
	auto button = creatToolButton(action);
	if (PtrIsNull(button))
		return;
	d->rightLyout->addWidget(button);
}

void OriginUI::NavigationBar::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		d->mousePress = true;
		d->mousePressPos = event->pos();
	}
}

void OriginUI::NavigationBar::mouseReleaseEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		d->mousePress = false;
	}
}

void OriginUI::NavigationBar::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
	if (d->mousePress)
		Q_EMIT mouseMove(event->pos() - d->mousePressPos);
}

QToolButton* OriginUI::NavigationBar::creatToolButton(QAction* action)
{
	if (PtrIsNull(action))
		return nullptr;
	auto button = new QToolButton(this);
	button->setDefaultAction(action);
	button->setMinimumSize(d->toolButtonMiniSize);
	button->setIconSize(d->iconSize);
	return button;
}
