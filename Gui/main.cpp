#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <iostream>
#include <memory>
#include <QSystemTrayIcon>
#include "mainWindow.h"
#include <vector>
#include "ToolBar.h"
#include "StickyNote.h"
#include "cpp_log.h"
#include <qfile.h>
#include <QTextStream>
#include "WidgetNoBoder.h"
#include "NoteFile.h"
#include "SystemTrayIcon.h"
#include "StickyNotePool.h"
#include "ToolButtonColor.h"
#include "ColorWidget.h"
#include "ProcessCommunication.h"
#include "HistoryWidget.h"
#include "Setting.h"
#include "StickyNoteText.h"
#include "StickyNoteToDoList.h"
#include <QDir>
#include<QTranslator>
#include "circleSwitch.h"
extern "C"
{
#include "PCStartRun.h"
}
#include "WindowNoBoder.h"
#include "ImageSave.h"
#include "EditLable.h"
#include "TempTextEdit.h"
#include "CompatibilityTool.h"
#include "StickyNoteFactory.h"

//为了方便使用qt 多语言功能，对这个宏进行修改 
#define QT_TRANSLATE_NOOP(scope, x) QCoreApplication::translate(scope,x)
int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 在创建QCoreApplication对象之前设置属性
	QApplication a(argc, argv);

	//实例化一个翻译模块的对象，并载入翻译文件
	QTranslator translator;
	auto  ok = translator.load(":/translate/translate/StickyNoteGui.qm");
	//使用QApplictioin的对象，为程序安装翻译模块
	a.installTranslator(&translator);

	//设置为最后一个窗口关闭时不退出程序
	a.setQuitOnLastWindowClosed(false);

	//载入qss 
	QFile file(":/Style/qss/GuiStyle.qss");
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);
	QString qss = stream.readAll();
	a.setStyleSheet(qss);
	file.close();


#ifndef _DEBUG
	Log::CppLog::GetInstance()->onAll();
#endif

#ifndef _DEBUG
	//建立进程通信
	NGui::ProcessCommunication proC;
	proC.connetction();

	//判断是否有进程已运行，有则退出程序
	if (proC.getType() == NGui::CLIENT)
	{
		//测试发送消息
		proC.sendMessage("hello word!\n");
		return 0;
	}
#endif

	//注册便笺创造器
	NGui::StickyNoteCreator::registerAllCreator();

	//测试
	auto nt = NGui::StickyNotePool::CreatStickyNote("DiversityNote");
	nt->show();


	NGui::SystemTrayIcon trayIcon;
	trayIcon.show();

	//用标志判断是否为第一次打开程序
	bool fierstOpen = false;
	//载入历史便利贴
	auto noteFile = NF::NoteFile::GetInstance();
	//调整文件路径到user目录
#ifndef _DEBUG
	CompatibilityTool::moveConfigFile();
	QDir dir;
	auto homePath = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath() + "/../data";
	dir.setPath(homePath);
	if (!dir.exists())
		dir.mkdir(homePath);
	noteFile->setFilePath(homePath.toStdString() + "/note.xml");

	NGui::Setting::setDefaultConfigPath(homePath.toStdString() + "/config.cfg");
	NGui::Setting setting;
	//如果配置文件不存在，则保存一下配置，解决没有点击过设置应用，就会在每次启动时重复生成GUID的问题
	if (!setting.defaultPathExist())
	{
		
		fierstOpen = true;
		//设置程序开机启动
		trayIcon.actionAutoStartTrigger(true);
		//设置默认图片保存路径
		QDir d("../image");
		setting.imageSavePath->setValue(d.absolutePath().toStdString());
		setting.saveProperty();
		//第一次使用弹出使用提示
		trayIcon.showMessage(QT_TRANSLATE_NOOP("main","Welcome to use sticky notes: "),
			QT_TRANSLATE_NOOP("main", "When using it, you can right-click on the tray icon to bring up the menu! \n\
Alternatively, you can left-click on the tray icon to display the history page! \n\
Both pages allow you to create new sticky notes or perform other actions! "), QSystemTrayIcon::Information, 20 * 1000);

	}
	NF::ImageSave::setDefaultFolder(QString::fromStdString(setting.imageSavePath->getValue()));
#endif // _DEBUG


	auto notes = noteFile->getStickyNote();

	for (auto note : notes)
	{
		if (note->visible == false)
			continue;
		auto st = NGui::StickyNotePool::CreatStickyNote(note);
		st->show();
	}

	auto history = NGui::HistoryWidget::Getinstance();

	for (auto note : notes)
	{
		history->addFileNote(note);
	}

	if (fierstOpen) {
	
		//第一次使用弹出便笺列表
#if 1
		{

			auto nt = NGui::StickyNotePool::CreatStickyNote("contentText");
			auto textGuiNote = dynamic_cast<NGui::StickyNoteText*>(nt);
			textGuiNote->setHtml(QT_TRANSLATE_NOOP("main", "First help!"));
			textGuiNote->show();

			nt = NGui::StickyNotePool::CreatStickyNote("contentToDoList");
			auto GuiNote = dynamic_cast<NGui::StickyNoteToDoList*>(nt);
			GuiNote->addItem(QT_TRANSLATE_NOOP("main", "Double-click the option to edit the content!"));
			GuiNote->addItem(QT_TRANSLATE_NOOP("main", "To delete the option, you need to click the Edit button at the bottom!"));
			GuiNote->addItem(QT_TRANSLATE_NOOP("main", "Click the plus button below to add the item!"));
			GuiNote->show();

			GuiNote->move(textGuiNote->x() + textGuiNote->width() + 10, textGuiNote->y());

		}
#endif
	
	}


	return a.exec();
}
