#pragma  once
#include "ExportConfig.hpp"
#include <QWidget>
#include <QPoint>
class QPropertyAnimation;
class QBoxLayout;
namespace OriginUI {
	class  DrawerWidgetD;
	/*
		一个具有弹出动画的顶部窗口
	*/
	class ORIGIN_UI_EXPORT DrawerWidget :public QWidget {
		Q_OBJECT
	public:
		DrawerWidget(QWidget* parent = 0);
		virtual ~DrawerWidget();

		//设置动画时间
		void setPopTime(unsigned int mec);
		void setPushTime(unsigned int mec);
		//设置一个中心widget
		void setCenterWidget(QWidget* widget);
		//初始化窗口位置
		virtual void initPos();

		virtual void pop();  //弹出窗口
		virtual void push(); //收回窗口
	signals:
		void widgetMoveSignal(QPoint point);

	protected:
		//获取动画起始位置
		virtual QPoint getPopStartPos();
		virtual QPoint getPopEndPos();
		virtual QPoint getPushEndPos();
		virtual QPoint getPushStartPos();
	
		QPropertyAnimation* getPopAnimation();
		QPropertyAnimation* getPushAnimaiton();

		void paintEvent(QPaintEvent* event) override;

		//获取窗口的位置变化事件
		void moveEvent(QMoveEvent* event) override;

	private:
		DrawerWidgetD* d;
	};
}

