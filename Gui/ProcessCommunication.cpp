#include "ProcessCommunication.h"
#include <QLocalSocket>
#include <iostream>
#include <QDataStream>
# include <Windows.h>
NGui::ProcessCommunication::ProcessCommunication(const QString& appName)
	:server(nullptr), appName(appName)
{
	
}

NGui::ProcessCommunication::~ProcessCommunication()
{
	if (server)
		server->close();
	delete server;
}

void NGui::ProcessCommunication::connetction()
{
	QLocalSocket socket;
	socket.connectToServer(appName);

	if (socket.waitForConnected(1000))
		type = CLIENT;
	else
		startService();

}

void NGui::ProcessCommunication::sendMessage(const QString& msg)
{
	QLocalSocket socket;
	bool connected = false;
	int timeout = 2000;
	for (int i = 0; i < 2; i++) {
		socket.connectToServer(appName);
		connected = socket.waitForConnected(timeout / 2);
		if (connected || i > 0)
			break;
		int ms = 250;
		Sleep(DWORD(ms));

	}
	if (!connected)
	{
		std::cout << "local client connect to sever failed!" << std::endl;
		return;
	}
	QByteArray array = msg.toUtf8();
	socket.write(array);
	if (socket.waitForBytesWritten(timeout))
		return;
	std::cerr << "local client send msg failed,msg: " << msg.toStdString() << std::endl;
}

NGui::ComunicationType NGui::ProcessCommunication::getType()
{
	return type;
}

void NGui::ProcessCommunication::startService()
{
	server = new QLocalServer();
	QObject::connect(server, SIGNAL(newConnection()),
		this, SLOT(newConnection()));

	if (!server->listen(appName)) {
		if (server->serverError() == QAbstractSocket::AddressInUseError) {
			server->removeServer(appName);
			server->listen(appName);
		}
	}

	if (server->isListening())
		std::cout << "local sever listen to " << appName.toStdString() << std::endl;
	else
		std::cout << "listen to " << appName.toStdString() << "filed " << std::endl;
}

void NGui::ProcessCommunication::newConnection()
{
	if (!server)
		return;
	QLocalSocket* socket = server->nextPendingConnection();
	if (!socket)
		return;

	connect(socket, SIGNAL(disconnected()),
		socket, SLOT(deleteLater()));
	if (socket->waitForReadyRead()) {
		auto msg = socket->readAll();
		ProcessMessageStack::GetInstance()->pushMessage(msg);
	}
	socket->disconnectFromServer();
}

std::shared_ptr<NGui::ProcessMessageStack> NGui::ProcessMessageStack::instance;

bool NGui::ProcessMessageStack::hasMessgae()
{
	if (msgs.size() > 0)
		return true;
	return false;
}

void NGui::ProcessMessageStack::pushMessage(const QString& message)
{
	std::cout << "push process msg : " << message.toStdString() << std::endl;
	msgs.push_back(message);
}

QString NGui::ProcessMessageStack::popMessage()
{
	if (!hasMessgae())
		return "";
	auto iter = msgs.begin();
	auto msg = *iter;
	msgs.erase(iter);
	return msg;
}
