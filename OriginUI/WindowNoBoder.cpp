#include "WindowNoBoder.h"
#include "NavigationBar.h"
#include <QVBoxLayout>
#include <QAction>
#include <qpainter.h>
namespace OriginUI {
	class WindowNoBoderD {
	public:
		WindowNoBoderD(WindowNoBoder *parent);

		QWidget* contentWidget;
		NavigationBar* navigationBar;
		QVBoxLayout* vLayout;
		QAction* actionClose;
	};

	WindowNoBoderD::WindowNoBoderD(WindowNoBoder* parent)
	{
		contentWidget = new QWidget(parent);
		navigationBar = new NavigationBar(parent);
		vLayout = new QVBoxLayout(parent);
		actionClose = new QAction(parent);
	}

}

OriginUI::WindowNoBoder::WindowNoBoder(QWidget* parent /*= 0*/)
	:
#ifndef _RUN_WITH_WIN7_
	NativeWindowTemplate<QWidget>(parent)
#else
	WidgetNoBoder(parent)
#endif // _RUN_WITH_WIN7_
{
	d = new WindowNoBoderD(this);
	initGUI();
}

OriginUI::WindowNoBoder::~WindowNoBoder()
{
	delete d;
}

void OriginUI::WindowNoBoder::paintEvent(QPaintEvent* event)
{
#ifndef _RUN_WITH_WIN7_
	NativeWindowTemplate<QWidget>::paintEvent(event);
#else
	WidgetNoBoder::paintEvent(event);
#endif // _RUN_WITH_WIN7_
	
	//为窗口增加一个灰色的边框
	QPainter painter(this);
	const QColor color(200, 200, 200, 100);
	painter.setBrush(QColor(0, 0, 0, 0));
	painter.setPen(color);
	auto rect = this->rect();
	rect.setWidth(rect.width() - 1);
	rect.setHeight(rect.height() - 1);
	painter.drawRect(rect);
}

OriginUI::NavigationBar* OriginUI::WindowNoBoder::getNavigationBar()
{
	return d->navigationBar;
}

QWidget* OriginUI::WindowNoBoder::getContentWidget()
{
	return d->contentWidget;
}

void OriginUI::WindowNoBoder::initGUI()
{
	auto vLayout = d->vLayout;
	vLayout->setSpacing(10);
#ifndef _RUN_WITH_WIN7_
	this->setTitleBar(d->navigationBar);
#endif // _RUN_WITH_WIN7_
	vLayout->setContentsMargins(1, 1, 1, 1);

	setLayout(vLayout);
	auto navigationBar = d->navigationBar;
	navigationBar->setMinimumHeight(40);
	navigationBar->setMaximumHeight(40);
	navigationBar->setToolButtonMiniSize(QSize(40, 40));
	navigationBar->setIconSize(QSize(20, 20));

	auto actionClose = d->actionClose;
	actionClose->setIcon(QIcon(":icon/off.svg"));
	actionClose->setToolTip(QT_TR_NOOP("close"));
	connect(actionClose, SIGNAL(triggered(bool)), this, SLOT(closeTrigger(bool)));
	navigationBar->addRightWidget(actionClose);

	vLayout->addWidget(navigationBar);
	vLayout->addWidget(d->contentWidget);
#ifdef _RUN_WITH_WIN7_
	connect(navigationBar, SIGNAL(mouseMove(QPoint)), this, SLOT(navigationBarMouseMove(QPoint)));
#endif

}


void OriginUI::WindowNoBoder::closeTrigger(bool)
{
	this->close();
}
#ifdef _RUN_WITH_WIN7_
void OriginUI::WindowNoBoder::navigationBarMouseMove(QPoint p)
{
	p += this->pos();
	this->move(p);
}
#endif
