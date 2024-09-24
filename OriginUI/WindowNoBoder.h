#pragma once
#include "WidgetNoBoder.h"
#include "ExportConfig.hpp"
#include "NativeWindowTemplate.hpp"
#include <QPoint>
namespace OriginUI {
	class WindowNoBoderD;
	class NavigationBar;
#ifdef _RUN_WITH_WIN7_
	class ORIGIN_UI_EXPORT WindowNoBoder :public WidgetNoBoder {
#else
	class ORIGIN_UI_EXPORT WindowNoBoder :public NativeWindowTemplate<QWidget> {
#endif // _RUN_WITH_WIN7_
		Q_OBJECT
	public:
		WindowNoBoder(QWidget* parent = 0);
		~WindowNoBoder();
	protected:
		void paintEvent(QPaintEvent* event);

	public:
		NavigationBar* getNavigationBar();
		QWidget* getContentWidget();

	private:
		void initGUI();
	public slots:
		void closeTrigger(bool);
#ifdef _RUN_WITH_WIN7_
		void  navigationBarMouseMove(QPoint);
#endif
	private:
		WindowNoBoderD* d;
	};
}