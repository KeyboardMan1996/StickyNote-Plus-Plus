#pragma once
#include <QObject>
#include <QNetworkReply>
#include <QJsonValue>
#include <QNetworkAccessManager>
namespace NGui {
	class AutoVersion :public QObject {
		Q_OBJECT
	public:
		AutoVersion();
		~AutoVersion();
	public:
		void start(int ms = 60 * 60 * 1000);
		void stop();
	protected:
		void timerEvent(QTimerEvent* event);
	//private:
	public:
		void getVersion();
		void runGetVersion();
	public slots:
		void finished(QStringList list);
		void networkReply(QNetworkReply* reply);
	Q_SIGNALS:
		void getVersionFinished(QStringList);
	private:
		bool cerrJsonError(const QJsonValue& value, const QString& key);
	private:
		double currentVerson;
		int	timerID;
	};

}