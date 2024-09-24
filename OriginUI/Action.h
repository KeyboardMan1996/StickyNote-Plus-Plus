#pragma once
#include "ExportConfig.hpp"
#include <QAction>
#include <QIcon>
namespace OriginUI {
	class ActionD;
	class ORIGIN_UI_EXPORT Action :public QAction {
		Q_OBJECT
	public:
		Action();
		~Action();
	public:
		//get set
		void setCheckIcon(const QIcon& icon);
		QIcon getCheckIcon();
		void setNoCheckIcon(const QIcon& icon);
		QIcon getNoCheckIcon();

	public Q_SLOTS:
		void actionToggled(bool checked);
	private:
		ActionD* d;
	};

}