#include "ui_mainWindow.h"
#include "mainWindow.h"
#include "pushpin.h"
MainWindow::MainWindow(QWidget* parent /*= 0*/)
	:QMainWindow(parent), ui(new Ui::MainWindow())
{
	ui->setupUi(this);

	testWidget = new QWidget();

	connect(ui->pushButtonTop, SIGNAL(clicked(bool)), this, SLOT(topClick(bool)));
	connect(ui->pushButtonDesktop, SIGNAL(clicked(bool)), this, SLOT(desktopClick(bool)));

}

MainWindow::~MainWindow()
{

}


void MainWindow::topClick(bool b)
{

}

void MainWindow::desktopClick(bool b)
{

}
