#pragma  once
#include <QEvent>
namespace NGui {
	class EventSender {
		friend class EventManager;
	public:
		EventSender();
		virtual ~EventSender();
		virtual void postEvent(QEvent* event) = 0;
	private:
		bool isRegister;
	};
}