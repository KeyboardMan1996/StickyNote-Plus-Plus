#pragma once
#include <QtWidgets/QMainWindow>
#include "pushpin.h"
namespace Ui{
	class MainWindow;
}
class MainWindow:public QMainWindow{
	Q_OBJECT
public: 
	MainWindow(QWidget* parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

	QWidget* testWidget;

public Q_SLOTS:
	void topClick(bool);
	void desktopClick(bool);
};