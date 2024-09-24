#include "ToolButtonColor.h"

#include <QPainter>
#include <QColorDialog>
namespace OriginUI {
	class ToolButtonColorD {
	public:
		ToolButtonColorD();
		~ToolButtonColorD();
	public:
		QColor color;
		QIcon icon;
		bool hover;
		QColor hoverCover;
		QToolButton *settingButton;
		bool settingEnable;
	};

	ToolButtonColorD::ToolButtonColorD()
		:color(Qt::white), hover(false), hoverCover(0, 0, 0, 50),
		icon(":icon/slection.svg"), settingEnable(false),settingButton(nullptr)
	{
		
	}

	ToolButtonColorD::~ToolButtonColorD()
	{
		delete settingButton;
	}

}

OriginUI::ToolButtonColor::ToolButtonColor(QWidget* parent /*= 0*/)
	:QToolButton(parent)
{
	d = new ToolButtonColorD();
	setCheckable(true);
	setMinimumSize(20, 20);

	//设置透明背景
	setStyleSheet("background:transparent; border - width:0; border - style:outset");
}

OriginUI::ToolButtonColor::~ToolButtonColor()
{

}

void OriginUI::ToolButtonColor::setColor(const QColor& c)
{
	d->color = c;
}

QColor OriginUI::ToolButtonColor::getColor()
{
	return d->color;
}

void OriginUI::ToolButtonColor::setSettingButtonEnable(const bool& b)
{
	if (b)
		addSettingButton();
	else
		removeSettingButton();
	d->settingEnable = b;
}

bool OriginUI::ToolButtonColor::getSettingButtonEnable()
{
	return d->settingEnable;
}

void OriginUI::ToolButtonColor::setIcon(const QIcon& icon)
{
	d->icon = icon;
}

void OriginUI::ToolButtonColor::settingButtonClicked(bool)
{
	auto color = QColorDialog::getColor(d->color, nullptr,
										QString::fromLocal8Bit("自定义按钮颜色"),
										QColorDialog::ColorDialogOption::ShowAlphaChannel);
	color = color.isValid() ? color : d->color;
	setColor(color);
	if (isChecked())
		Q_EMIT clicked(false);
}

void OriginUI::ToolButtonColor::paintEvent(QPaintEvent* event)
{

 	QPainter painter(this);
 	painter.fillRect(this->rect(),QBrush(d->color));

	if (d->hover)
		painter.fillRect(this->rect(), QBrush(d->hoverCover));

	QToolButton::paintEvent(event);
}

void OriginUI::ToolButtonColor::checkStateSet()
{
	QToolButton::checkStateSet();
	if (isChecked())
	{
		QToolButton::setIcon(d->icon);
	}else {
		QToolButton::setIcon(QIcon());
	}
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void OriginUI::ToolButtonColor::enterEvent(QEnterEvent* event)
#else
void OriginUI::ToolButtonColor::enterEvent(QEvent* event)
#endif
{
	QToolButton::enterEvent(event);
	d->hover = true;
}

void OriginUI::ToolButtonColor::leaveEvent(QEvent* event)
{
	QToolButton::leaveEvent(event);
	d->hover = false;
}

void OriginUI::ToolButtonColor::resizeEvent(QResizeEvent* event)
{
	QToolButton::resizeEvent(event);


	//改变自身图标大小
	double sacle = 0.5;
	auto w = width() > height() ? height() : width();
	setIconSize(QSize(w*sacle, w*sacle));

	//改变设置图标的大小与位置
	autoSettingState();
}

void test() {
};
void OriginUI::ToolButtonColor::addSettingButton()
{
	if (d->settingButton)
		return;

	//初始化右下角窗口
	d->settingButton = new QToolButton(this);
	d->settingButton->setIcon(QIcon(":icon/setting.svg"));
	d->settingButton->setMinimumSize(20, 20);
	d->settingButton->setToolTip(QString::fromLocal8Bit("自定义颜色"));
	d->settingButton->setStyleSheet("QToolButton:hover{ margin-top:0px;margin-left:0px;}\
					QToolButton{ margin-top:2px;margin-left:2px;}");

	connect(d->settingButton, SIGNAL(clicked(bool)), this, SLOT(settingButtonClicked(bool)));
	autoSettingState();
}

void OriginUI::ToolButtonColor::removeSettingButton()
{
	if (!d->settingButton)
		return;
	delete d->settingButton;
	d->settingButton = nullptr;
}

void OriginUI::ToolButtonColor::autoSettingButtonSize()
{
	if (!d->settingButton)
		return;
	double sacle = 0.3;
	double w = width() > height() ? height() : width();
	d->settingButton->resize(w * sacle, w * sacle);
	d->settingButton->setIconSize(d->settingButton->size());
}

void OriginUI::ToolButtonColor::autoSettingButtonPos()
{
	if (!d->settingButton)
		return;

#if 0
	auto w = width();
	auto h = height();
	int padding = 4;
	auto x = width() - d->settingButton->width() - padding;
	auto y = height() - d->settingButton->height() - padding;
	d->settingButton->move(x, y);
#else
	int padding = 4;
	auto x = width() - d->settingButton->width() - padding;
	auto y = height() - d->settingButton->height() - padding;
	d->settingButton->move(x,y);
#endif
	
}

/**
* @brief OriginUI::ToolButtonColor::autoSettingState 自动调整设置按钮状态
* @return void
*/
void OriginUI::ToolButtonColor::autoSettingState()
{
	//先调整大小再设置位置，因为位置需要使用大小计算
	autoSettingButtonSize();
	autoSettingButtonPos();
}

