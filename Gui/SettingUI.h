#pragma once
#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <map>
#include "WindowNoBoder.h"
#include "NavigationBar.h"
namespace Ui {
	class SettingUI;
}
namespace NGui {
	class ActionCmdStateListWidget;
	class SettingUI :public OriginUI::WindowNoBoder {
		Q_OBJECT
	public:
		SettingUI(QWidget* parent = 0);
		~SettingUI();

		void initGui();
	public slots:
		void on_pushButtonOk_clicked();
		void on_pushButtonImagePath_clicked();
	private:
		Ui::SettingUI *ui;
		ActionCmdStateListWidget* cmdStateListWidget;
	};

	class ActionCmdStateItem :public QWidget {
	public:
		ActionCmdStateItem(QWidget* parent = 0);
		~ActionCmdStateItem() = default;;

	public:
		QLabel* label,*imageLabel;
		QCheckBox* checkBox;
	};

	class ActionCmdStateListWidget :public QWidget {
	public:
		ActionCmdStateListWidget(QWidget* parent = 0);
		~ActionCmdStateListWidget();
		
	public:
		void addCommandCheckBox(const std::string& cmd,const QString& text,const QIcon& icon,const bool& checked = false);
		bool getCommandIsChecked(const std::string& cmd);

	private:
		std::map<std::string, ActionCmdStateItem*> items;
	};

}