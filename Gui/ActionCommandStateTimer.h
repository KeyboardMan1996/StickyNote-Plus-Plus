#pragma  once
#include <QObject>
#include <QTimer>
namespace NGui {
	class ActionCommandStateTimer :public QObject{
		Q_OBJECT
	public:
		ActionCommandStateTimer();
		~ActionCommandStateTimer();

	public Q_SLOTS:
		void timeout();
	private:
		QTimer timer;

	};
}