#include "ColorWidget.h"
#include "ToolButtonColor.h"
#include <QHBoxLayout>
NGui::ColorWidget::ColorWidget( QColor color, QWidget* parent /*= 0*/)
	:QWidget(parent)
{
	QHBoxLayout* hlayout = new QHBoxLayout(this);
	hlayout->setSpacing(0);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	hlayout->setContentsMargins(0, 0, 0, 0);
#else
	hlayout->setMargin(0);
#endif
	setLayout(hlayout);

	defaultColor = QColor("#FFF7D1");
	//添加按钮
	{
		auto button = creatButton();
		button->setColor(defaultColor);
		button->setChecked(true);
		connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
	}
	{
		auto button = creatButton();
		button->setColor(QColor("#A1EF9B"));
		connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
	}
	{
		auto button = creatButton();
		button->setColor(QColor("#FFAFDF"));
		connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
	}
	{
		auto button = creatButton();
		button->setColor(QColor(158, 223, 255));
		connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
	}
	{
		auto button = creatButton();
		button->setColor(QColor(224, 224, 224));
		connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
	}
	{
		auto button = creatButton();
		button->setColor(color);
		button->setSettingButtonEnable(true);
		connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));
	}
}

NGui::ColorWidget::~ColorWidget()
{
	for (auto iter = buttons.begin(); iter != buttons.end();)
	{
		auto bt = *iter;
		iter = buttons.erase(iter);
		delete bt;
	}
}

/**
* @brief NGui::ColorWidget::getCheckIndex 获取当前使用的颜色索引
* @return int
*/
int NGui::ColorWidget::getCheckIndex()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i]->isChecked())
			return i;
	}
	return 0;
}

/**
* @brief NGui::ColorWidget::getButtonColor 返回索引button的颜色
* @param const int & index
* @return QT_NAMESPACE::QColor
*/
QColor NGui::ColorWidget::getButtonColor(const int& index)
{
	if (index >= buttons.size())
		return defaultColor;
	return buttons[index]->getColor();
}

/**
* @brief NGui::ColorWidget::setCustomColor 设置自定义颜色
* @param const QColor & color
* @return void
*/
void NGui::ColorWidget::setCustomColor(const QColor& color)
{
	if (!color.isValid())
		return;
	if (buttons.size() <= 0)
		return;
	auto bt = buttons.rbegin();
	(*bt)->setColor(color);
}

/**
* @brief NGui::ColorWidget::getCustomColor 获取自定以颜色按钮的颜色
* @return QT_NAMESPACE::QColor
*/
QColor NGui::ColorWidget::getCustomColor()
{
	auto bt = buttons.rbegin();
	if (bt == buttons.rend())
		return defaultColor;
	return (*bt)->getColor();
}

/**
* @brief NGui::ColorWidget::setButtonCheck 设置某一个按钮为选中状态
* @param const int & index
* @return void
*/
void NGui::ColorWidget::setButtonCheck(const int& index)
{
	if (index >= buttons.size())
		return;

	for (auto bt : buttons)
	{
		bt->setChecked(false);
	}

	buttons[index]->setChecked(true);
}

void NGui::ColorWidget::buttonClicked(bool)
{
	auto button = dynamic_cast<OriginUI::ToolButtonColor*>(sender());
	if (!button)
		return;
	Q_EMIT colorChange(button->getColor());
	button->setChecked(true);

	for(auto bt:buttons)
	{
		if(bt == button)
			continue;
		bt->setChecked(false);
	}
}

OriginUI::ToolButtonColor* NGui::ColorWidget::creatButton()
{
	auto button = new OriginUI::ToolButtonColor();
	layout()->addWidget(button);
	buttons.push_back(button);
	button->setChecked(false);
	button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	return button;
}

NGui::ColorDrawerWidget::ColorDrawerWidget(QWidget* parent /*= 0*/)
	:OriginUI::DrawerWidget(parent)
{

}

void NGui::ColorDrawerWidget::autoSize(const QSize& parentSize)
{
	const int minHeight = 32;
	auto ph = parentSize.height();
	auto h = ph / 2 > minHeight ? ph / 2 : minHeight;
	const int maxHeight = 80;
	h = h > maxHeight ? maxHeight : h;
	resize(parentSize.width(), h);
	move(0, -height());
}

void NGui::ColorDrawerWidget::focusOutEvent(QFocusEvent* event)
{
	OriginUI::DrawerWidget::focusOutEvent(event);
	//push();
}
