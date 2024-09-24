#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <iostream>
#include <vector>
#include "ToolBar.h"
#include "cpp_log.h"
#include <qfile.h>
#include <QTextStream>
#include "WidgetNoBoder.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//Log::CppLog::GetInstance()->onAll();

	QWidget mw;
	mw.show();
	//‘ÿ»Îqss 
	QFile file("GuiStyle.qss");
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);
	QString qss = stream.readAll();
	a.setStyleSheet(qss);
	file.close();

	return a.exec();
}
