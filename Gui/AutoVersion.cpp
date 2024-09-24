#include "AutoVersion.h"
#include "version.hpp"
#include "Setting.h"
#include "UpdateUI.h"
#include "version.hpp"
#include <QTimerEvent>
#include <QtConcurrent/QtConcurrent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <iostream>
#include <QJsonDocument>
#include <QDebug>
NGui::AutoVersion::AutoVersion()
	:currentVerson(VERSION),timerID(0)
{
	connect(this, SIGNAL(getVersionFinished(QStringList)), this, SLOT(finished(QStringList)));
}

NGui::AutoVersion::~AutoVersion()
{
	stop();
}

void NGui::AutoVersion::start(int s /*= 60 * 60*/)
{
	stop();
	timerID = startTimer(s);
}

void NGui::AutoVersion::stop()
{
	killTimer(timerID);
}

void NGui::AutoVersion::timerEvent(QTimerEvent* event)
{
	if (event->timerId() == timerID)
	{
		runGetVersion();
	}	
}

void NGui::AutoVersion::getVersion()
{

}

void NGui::AutoVersion::runGetVersion()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	QtConcurrent::run([this]() {getVersion(); });
#else
	QtConcurrent::run(this, &AutoVersion::getVersion);
#endif
}

void NGui::AutoVersion::finished(QStringList list)
{
	if (list.size() < 3)
		return;
	double version = list.at(0).toDouble();
	QUrl url(list.at(1));
	QString versionNew = list.at(2);
	auto setting = Setting::GetInstance();
	double ignoreVersion = setting->ignoreVersion->getValue();

	if (version <= currentVerson || version <= ignoreVersion)
		return;
	UpdateUI* updateUI = new UpdateUI();
	updateUI->setAttribute(Qt::WA_DeleteOnClose);
	updateUI->setUrl(url);
	updateUI->setNewVersion(version);
	updateUI->setContent(versionNew);
	updateUI->setAttribute(Qt::WA_DeleteOnClose);
	updateUI->show();
}

void NGui::AutoVersion::networkReply(QNetworkReply* reply)
{

}

bool NGui::AutoVersion::cerrJsonError(const QJsonValue& value, const QString& key)
{
	if (value.isUndefined())
	{
		std::cerr << "class AutoVersion  json  key data is undefined! key: " << key.toStdString() << std::endl;
		return false;
	}
	return true;
}

