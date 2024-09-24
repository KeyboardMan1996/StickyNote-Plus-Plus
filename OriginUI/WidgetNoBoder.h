#pragma once
#include "ExportConfig.hpp"
#include <qwidget.h>
#include <QMouseEvent>
#include <QResizeEvent>
namespace OriginUI {
	class WidgetNoBoderD;
	class ORIGIN_UI_EXPORT WidgetNoBoder :public QWidget {
		Q_OBJECT
	public:
		WidgetNoBoder(QWidget* parent = 0);
		virtual ~WidgetNoBoder();

	protected:
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
		bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result)override;
#else
		bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
	private:
		WidgetNoBoderD *d;
	};

}