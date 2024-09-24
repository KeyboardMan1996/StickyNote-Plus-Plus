#pragma once
#include <QLocalServer>
#include <mutex>
#include <memory>
#include <vector>
namespace NGui {
	enum ComunicationType {
		SERVER ,	//服务端
		CLIENT		//客户端
	};

	class ProcessCommunication:public QObject {
		Q_OBJECT
	public:
		ProcessCommunication(const QString& appName = "StickyNote");
		~ProcessCommunication();

	public:
		void connetction();
		void sendMessage(const QString& msg);
		ComunicationType getType();
	private:
		void startService();
		
	public Q_SLOTS:
		void newConnection();

	private:
		QLocalServer *server;
		QString appName;
		ComunicationType type;
	};

	class ProcessMessageStack {
	public:
		~ProcessMessageStack() = default;
		ProcessMessageStack(const ProcessMessageStack&) = delete;
		ProcessMessageStack operator=(const ProcessMessageStack&) = delete;

		static std::shared_ptr<ProcessMessageStack> GetInstance() {
			static std::once_flag flag;
			std::call_once(flag, [&]() {
				instance.reset(new ProcessMessageStack());
				});
			return instance;
		}
	public:
		bool hasMessgae();
		void pushMessage(const QString& message);
		QString popMessage();
	private:
		ProcessMessageStack() = default;
		static std::shared_ptr<ProcessMessageStack> instance;
		std::vector<QString> msgs;
	};
}