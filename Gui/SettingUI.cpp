#include "SettingUI.h"
#include "ui_SettingUI.h"
#include "Setting.h"
#include "Property.h"
#include "StickyNotePool.h"
#include "HistoryWidget.h"
#include "DelayDialog.h"
#include "ActionCommand.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
NGui::SettingUI::SettingUI(QWidget* parent /*= 0*/)
	:OriginUI::WindowNoBoder(parent),ui(new Ui::SettingUI())
{
	ui->setupUi(getContentWidget());
	initGui();
	resize(650, 500);
	connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(on_pushButtonOk_clicked()));
	connect(ui->pushButtonImagePath, SIGNAL(clicked()), this, SLOT(on_pushButtonImagePath_clicked()));
}

NGui::SettingUI::~SettingUI()
{
	delete ui;
}

void NGui::SettingUI::initGui()
{
	auto setting = Setting::GetInstance();
	ui->checkBoxNoteWidgetDisplayTaskbar->setChecked(!setting->stickyNoteToolMod->getValue());
	ui->checkBoxNoteWidgetMouseMoveDisplayToolbar->setChecked(setting->stickyNoteToolbarAutoPop->getValue());
	QKeySequence key(QString::fromStdString(setting->historyWidgetShortcut->getValue()));
	ui->keySequenceEdit->setKeySequence(key);
	ui->checkBoxAutoUpdate->setChecked(setting->autoUpdate->getValue());
	ui->checkBoxAutoMagnetic->setChecked(setting->autoMagnetic->getValue());
	ui->spinBoxMagneticThreshold->setValue(setting->magneticThreshold->getValue());
	ui->spinBoxMagneticDistance->setValue(setting->magneticDistance->getValue());
	QFont font;
	font.fromString(QString::fromStdString(setting->stickyNoteFont->getValue()));
	ui->fontComboBox->setCurrentFont(font);
	ui->spinBoxFontSize->setValue(font.pointSize());
	ui->lineEditImagePath->setText(QString::fromStdString(setting->imageSavePath->getValue()));
	ui->lineEditImagePath->setEnabled(false);
	ui->checkBoxTransparentWidget->setChecked(setting->transparentWidget->getValue());
	ui->checkBoxPinIconLayer->setChecked(setting->pinIconLayer->getValue());
	ui->spinBoxDefaultWidth->setValue(setting->stickyNoteWidth->getValue());
	ui->spinBoxDefaultHeight->setValue(setting->stickyNoteHeight->getValue());
	ui->checkBoxAutoOpenURL->setChecked(setting->visitiUrl->getValue());

	//初始化cmd窗口
	auto cmdFactory = ActionCmdFactory::GetInstance();
	cmdStateListWidget = new ActionCmdStateListWidget(ui->groupBoxCmd);
	ui->groupBoxCmd->layout()->addWidget(cmdStateListWidget);
	for (auto iter = setting->cmdState.begin();iter != setting->cmdState.end();iter++)
	{
		auto cmd = cmdFactory->creatActionCommand(iter->first);
		cmdStateListWidget->addCommandCheckBox(iter->first, cmd->toolTip(), cmd->icon(), iter->second->getValue());
	}
}

void NGui::SettingUI::on_pushButtonOk_clicked()
{
	//保存设置
	auto setting = Setting::GetInstance();
	setting->stickyNoteToolMod->setValue(!ui->checkBoxNoteWidgetDisplayTaskbar->isChecked());
	setting->stickyNoteToolbarAutoPop->setValue(ui->checkBoxNoteWidgetMouseMoveDisplayToolbar->isChecked());
	setting->historyWidgetShortcut->setValue(ui->keySequenceEdit->keySequence().toString().toStdString());
	setting->autoUpdate->setValue(ui->checkBoxAutoUpdate->isChecked());
	setting->autoMagnetic->setValue(ui->checkBoxAutoMagnetic->isChecked());
	setting->magneticThreshold->setValue(ui->spinBoxMagneticThreshold->value());
	setting->magneticDistance->setValue(ui->spinBoxMagneticDistance->value());
	QFont font = ui->fontComboBox->currentFont();
	font.setPointSize(ui->spinBoxFontSize->value());
	setting->stickyNoteFont->setValue(font.toString().toStdString());
	setting->imageSavePath->setValue(ui->lineEditImagePath->text().toStdString());
	setting->transparentWidget->setValue(ui->checkBoxTransparentWidget->isChecked());
	setting->pinIconLayer->setValue(ui->checkBoxPinIconLayer->isChecked());
	setting->stickyNoteHeight->setValue(ui->spinBoxDefaultHeight->value());
	setting->stickyNoteWidth->setValue(ui->spinBoxDefaultWidth->value());
	setting->visitiUrl->setValue(ui->checkBoxAutoOpenURL->isChecked());

	//初始化cmd信息
	for (auto iter = setting->cmdState.begin(); iter != setting->cmdState.end(); iter++)
	{
		auto cmdName = iter->first;
		setting->cmdState[cmdName]->setValue(cmdStateListWidget->getCommandIsChecked(cmdName));
	}
#if  0 //改变置于底层的实现方式之后，不需要提醒了
	//判断窗口透明和固定到图标层是否同时开启，同时开启则提醒用户注意事项
	if (setting->transparentWidget->getValue() && setting->pinIconLayer->getValue())
	{
		OriginUI::DelayDialog dialog;
		dialog.setWindowTitle(tr("Important notice"));
		dialog.setOkButtonText(tr("I know"));
		dialog.setCancelbuttonText(tr("Cancel"));
		dialog.setContentText(tr("\
		Notice for enabling both transparent window and fix to desktop functions:\n\
			1. On some computers, the fixed transparent window may disappear.\n\
			2. If this happens, please turn off one of the functions in the settings, and then restart the software to continue using it.\n\
			3. Due to the above situation, the transparency effect will fail when fixing the transparent window to the desktop.\n\
			4. Both functions require a software restart to take effect.\n\
			5. This is due to a bug in QT, and an issue has been raised with the official website.The repair time is unknown.\n\
			"));
		dialog.exec();
		if (OriginUI::DelayDialog::OK != dialog.getResult())
		{
			return;
		}
	}
#endif
	 
	//保存设置
	setting->saveProperty();
	//现有窗口重新载入设置
	StickyNotePool::reloadSetting();
	HistoryWidget::Getinstance()->setShortcut(ui->keySequenceEdit->keySequence());
	this->close();
}

void NGui::SettingUI::on_pushButtonImagePath_clicked()
{
	QString fileName = QFileDialog::getExistingDirectory(this, tr("file dialog"), ui->lineEditImagePath->text());
	ui->lineEditImagePath->setText(fileName);
}

NGui::ActionCmdStateListWidget::ActionCmdStateListWidget(QWidget* parent /*= 0*/)
	:QWidget(parent)
{
	QVBoxLayout* vlayout = new QVBoxLayout(this);
	setLayout(vlayout);
}

NGui::ActionCmdStateListWidget::~ActionCmdStateListWidget()
{

}

void NGui::ActionCmdStateListWidget::addCommandCheckBox(const std::string& cmd, const QString& text, const QIcon& icon,const bool& checked)
{
	ActionCmdStateItem* item = new ActionCmdStateItem(this);
	item->setMinimumHeight(40);
	layout()->addWidget(item);
	item->imageLabel->setPixmap(icon.pixmap(24, 24));
	item->label->setText(text);
	item->checkBox->setChecked(checked);
	items[cmd] = item;
}

bool NGui::ActionCmdStateListWidget::getCommandIsChecked(const std::string& cmd)
{
	auto  iter = items.find(cmd);
	if (iter == items.end())
		return false;
	return iter->second->checkBox->isChecked();
}

NGui::ActionCmdStateItem::ActionCmdStateItem(QWidget* parent /*= 0*/)
	:QWidget(parent)
{
	imageLabel = new QLabel(this);
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	imageLabel->setSizePolicy(sizePolicy);
	label = new QLabel(this);
	checkBox = new QCheckBox(this);
	QHBoxLayout* hlayout = new QHBoxLayout(this);
	setLayout(hlayout);
	hlayout->addWidget(imageLabel);
	hlayout->addWidget(label);
	hlayout->addWidget(checkBox);
}
