#pragma once
#include <QObject>
#include <vector>
#include <mutex>
#include <memory>
namespace NGui {
	class EventSender;
	class EventManager :QObject{
	public:
		~EventManager();
		EventManager(const EventManager&) = delete;
		EventManager operator =(const EventManager&) = delete;

		static std::shared_ptr<EventManager> GetInstance() {
			static std::once_flag flag;
			std::call_once(flag, [&] {
				instance.reset(new EventManager());
				});
			return instance;
		}
		static void postEvent(QEvent*);
	private:
		EventManager(QObject* parent = 0);
		static std::shared_ptr<EventManager> instance;

	public:
		void rgisterSender(EventSender* s);
		void remove(EventSender* s);
	private:
		std::vector<EventSender*> senders;
	};
}