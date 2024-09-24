#pragma once
#include <QSystemTrayIcon>
#include <QMenu>
#include <QWidget>

namespace OriginUI {
	class Action;
}

namespace NGui {
	class AutoVersion;
	class SystemTrayIcon :public QSystemTrayIcon{
		Q_OBJECT
	public:
		SystemTrayIcon(QWidget* parent = 0);
		~SystemTrayIcon();
	private:
		void initGUI();
	private:
		QMenu* menu;
		OriginUI::Action *acitonAutoStart;
		AutoVersion *autoVersion;
	public Q_SLOTS:
		void onActive(QSystemTrayIcon::ActivationReason reason);
		void acitonNewNoteTrigger(bool);
		void actionExitTrigger(bool);
		void actionAutoStartTrigger(bool);
		void actionOpenUrl(bool);
		void actionHistory(bool);
		void actionShowSettingUI(bool);
		void actionNewToDoListNote(bool);
	
	};
}
