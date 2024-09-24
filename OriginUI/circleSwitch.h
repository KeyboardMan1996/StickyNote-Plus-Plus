#pragma once
#include <QToolButton>
#include "ExportConfig.hpp"
namespace OriginUI {
	class CircleSwitchD;
	class ORIGIN_UI_EXPORT CircleSwitch :public QToolButton{
	public:
		CircleSwitch(QWidget* parent = 0);
		~CircleSwitch();
		void setBackgroundColor(const QColor& color);
		QColor getBackgroundColor();
		void setCheckBackgroundColor(const QColor& color);
		QColor getCheckBackgroundColor();
		void setColor(const QColor& color);
		QColor getColor();
	protected:
		void paintEvent(QPaintEvent* event);
	private:
		CircleSwitchD* d;
	};

	class ORIGIN_UI_EXPORT DeleteButton :public CircleSwitch {
	public:
		DeleteButton(QWidget* parent);
	protected:
		void paintEvent(QPaintEvent* event);
	};
}