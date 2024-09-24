#include "UpdateUI.h"
#include "ui_UpdateUI.h"
#include "version.hpp"
#include "Setting.h"
#include <QDesktopServices>
NGui::UpdateUI::UpdateUI(QWidget* parent /*= 0*/)
	:WindowNoBoder(parent),ui(new Ui::UpdateUI())
{
	ui->setupUi(getContentWidget());
	ui->labelCurrentVersion->setText(QString::number(VERSION));
	resize(400, 300);

	connect(ui->pushButtonUpdate, &QPushButton::clicked, this, &UpdateUI::updateButtonClicked);
	connect(ui->pushButtonIgnore, &QPushButton::clicked, this, &UpdateUI::ignoreButtonClicked);
}

NGui::UpdateUI::~UpdateUI()
{
	delete ui;
}

void NGui::UpdateUI::setUrl(const QUrl& url)
{
	this->url = url;
}

void NGui::UpdateUI::setNewVersion(const double& version)
{
	this->newVersion = version;
	ui->labelNewVersion->setText(QString::number(version));
}

void NGui::UpdateUI::setContent(const QString& content)
{
	ui->textEdit->setText(content);
}

void NGui::UpdateUI::updateButtonClicked()
{
	QDesktopServices::openUrl(url);
	this->close();
}

void NGui::UpdateUI::ignoreButtonClicked()
{
	auto setting = Setting::GetInstance();
	setting->ignoreVersion->setValue(newVersion);
	setting->saveProperty();
	this->close();
}

