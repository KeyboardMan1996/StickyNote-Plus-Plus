#include "SystemTrayIcon.h"
#include "StickyNote.h"
#include "StickyNotePool.h"
#include <qaction.h>
#include <QApplication>
#include <QUrl>
#include <QDesktopServices>
#include "Action.h"
#include "HistoryWidget.h"
#include "SettingUI.h"
#include "AutoVersion.h"
extern "C" {
#include "PCStartRun.h"
}
NGui::SystemTrayIcon::SystemTrayIcon(QWidget* parent /*= 0*/)
	:QSystemTrayIcon(parent),autoVersion(new AutoVersion())
{
	initGUI();
#ifndef _DEBUG
	autoVersion->runGetVersion();
	autoVersion->start();
#endif // DEBUG
}

NGui::SystemTrayIcon::~SystemTrayIcon()
{
	delete menu;
	delete acitonAutoStart;
	actionExitTrigger(false);
}

void NGui::SystemTrayIcon::initGUI()
{
	setToolTip(tr("Sticky note\nLeft-click to display the browsing history.\
										\nRight-click to display the menu!"));

	menu = new QMenu();
	setContextMenu(menu);

	auto actionNewNote = new QAction(menu);
	actionNewNote->setText(tr("New note"));
	actionNewNote->setIcon(QIcon(":/icon/add_note.svg"));
	actionNewNote->setToolTip(tr("New sticky note"));
	menu->addAction(actionNewNote);
	connect(actionNewNote, SIGNAL(triggered(bool)), this, SLOT(acitonNewNoteTrigger(bool)));

	{
		auto action = new QAction(menu);
		action->setText(tr("New list"));
		action->setIcon(QIcon(":/icon/add_note.svg"));
		action->setToolTip(tr("New to do list note!"));
		connect(action, SIGNAL(triggered(bool)), this, SLOT(actionNewToDoListNote(bool)));
		menu->addAction(action);
	}

	{
		auto action = new QAction(menu);
		action->setText(tr("Note list"));
		action->setIcon(QIcon(":/icon/history.svg"));
		action->setToolTip(tr("Open sticky note list"));
		connect(action, SIGNAL(triggered(bool)), this, SLOT(actionHistory(bool)));
		menu->addAction(action);
	}
	{
		auto action = new QAction(menu);
		action->setText(tr("Setting"));
		action->setToolTip(tr("Open setting window"));
		connect(action, SIGNAL(triggered(bool)), this, SLOT(actionShowSettingUI(bool)));
		menu->addAction(action);
	}

	//临时添加意见反馈已经检查更新按钮
// 	{
// 		auto action = new QAction(menu);
// 		action->setText(tr("Feedback"));
// 		connect(action, SIGNAL(triggered(bool)), this, SLOT(actionOpenUrl(bool)));
// 		menu->addAction(action);
// 	}
	{
		auto action = new QAction(menu);
		action->setText(tr("Update"));
		connect(action, SIGNAL(triggered(bool)), this, SLOT(actionOpenUrl(bool)));
		menu->addAction(action);
	}

	//初始化开机启动
	acitonAutoStart = new OriginUI::Action();
	acitonAutoStart->setCheckable(true);
	acitonAutoStart->setCheckIcon(QIcon(":icon/on.svg"));
/*	acitonAutoStart->setNoCheckIcon(QIcon(":icon/off.svg"));*/
	acitonAutoStart->setText(tr("Power on"));
	acitonAutoStart->setToolTip(tr("Set up automatic startup."));
	if (IsAutoStart())
		acitonAutoStart->setChecked(true);
	else
		acitonAutoStart->setChecked(false);
	connect(acitonAutoStart, SIGNAL(triggered(bool)), this, SLOT(actionAutoStartTrigger(bool)));
	menu->addAction(acitonAutoStart);


	auto actionExit = new QAction(tr("Exit"),menu);
	actionExit->setIcon(QIcon(":/icon/exit.svg"));
	actionExit->setToolTip(tr("Exit program"));
	menu->addAction(actionExit);
	connect(actionExit, SIGNAL(triggered(bool)), this, SLOT(actionExitTrigger(bool)));

	setIcon(QIcon(":/icon/icon.svg"));

	connect(this, &QSystemTrayIcon::activated, this, &NGui::SystemTrayIcon::onActive);

}

void NGui::SystemTrayIcon::onActive(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
	{
		HistoryWidget::Getinstance()->show();
	}
}

void NGui::SystemTrayIcon::acitonNewNoteTrigger(bool)
{
	auto note = StickyNotePool::CreatStickyNote("contentText");
	note->show();
}

void NGui::SystemTrayIcon::actionExitTrigger(bool)
{
	//退出程序前，需要清理所有QObject对象
	StickyNotePool::GetInstance()->closeAllNote();
	HistoryWidget::destoryInstance();
	QApplication::exit();
}

void NGui::SystemTrayIcon::actionAutoStartTrigger(bool check)
{
	if (check)
	{
		AutoStart();
	}else {
		CanclePowerOn();
	}
	if (IsAutoStart())
		acitonAutoStart->setChecked(true);
	else
		acitonAutoStart->setChecked(false);
}

void NGui::SystemTrayIcon::actionOpenUrl(bool)
{
	QUrl url("http://cppdebug.com/archives/286");
	QDesktopServices::openUrl(url);
}

void NGui::SystemTrayIcon::actionHistory(bool)
{
	HistoryWidget::Getinstance()->show();
}

void NGui::SystemTrayIcon::actionShowSettingUI(bool)
{
	SettingUI* setting = new SettingUI();
	setting->setAttribute(Qt::WA_DeleteOnClose);
	setting->show();
}

void NGui::SystemTrayIcon::actionNewToDoListNote(bool)
{
	auto note = StickyNotePool::CreatStickyNote("contentToDoList");
	note->show();
}

