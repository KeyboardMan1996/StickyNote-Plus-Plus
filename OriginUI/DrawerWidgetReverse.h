#pragma once
#include "drawerWidget.h"
namespace OriginUI {
	/*
		一个从底部弹出的弹出窗口
	*/
	class ORIGIN_UI_EXPORT DrawerWidgetReverse :public DrawerWidget{
	public:
		DrawerWidgetReverse(QWidget* parent = 0);
		~DrawerWidgetReverse();

	public:
		void initPos() override;

 	protected:
		QPoint getPopEndPos() override;
		QPoint getPushEndPos() override;

	};
}