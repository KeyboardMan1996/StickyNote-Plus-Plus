#pragma once
#include "ExportConfig.hpp"
#include <QWidget>
#include <QToolButton>
namespace OriginUI {
	class NavigationBarD;
	class ORIGIN_UI_EXPORT NavigationBar:public QWidget{
		Q_OBJECT
	public:
		NavigationBar(QWidget* parent =0);
		~NavigationBar();
		
		//Ìí¼Ówidget
		void addLeftWidget(QWidget* widget);
		void addLeftWidget(QAction* action);
		void addRightWidget(QWidget* widget);
		void addRightWidget(QAction* action);
		void setIconSize(const QSize& size);
		void setToolButtonMiniSize(const QSize& size);
	protected:
		void mousePressEvent(QMouseEvent* event);
		void mouseReleaseEvent(QMouseEvent* event);
		void mouseMoveEvent(QMouseEvent* event);

	Q_SIGNALS:
		void mouseMove(QPoint distance);

	private:
		QToolButton* creatToolButton(QAction* action);
	private:
		NavigationBarD* d;
	};
}