#pragma  once
#include "ExportConfig.hpp"
#include <QWidget>
#include <QEvent>
namespace OriginUI {
	class DrawerWidget;
	class ToolBarD;
	class ORIGIN_UI_EXPORT ToolBar :public QWidget {
		Q_OBJECT
	public:
		ToolBar(QWidget* parent = 0);
		ToolBar(DrawerWidget* dw, QWidget* parent = 0);
		~ToolBar();
		
	public:
		//添加窗口
		void addWidget(QWidget* Widget);
		//清理窗口
		void clear();
		//收回窗口
		virtual void push();
		//弹出窗口
		virtual void pop();
		//设置工具栏对齐规则
		void setAlignment(Qt::AlignmentFlag flag);
		//get set
		void setAutoPop(const bool& b);
		bool getAutoPop();
		void setAutoPush(const bool& b);
		bool getAutoPush();
		//获取真正的toolbar窗口
		QWidget* getToolbarWidget();
	protected:
		void resizeEvent(QResizeEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	void enterEvent(QEnterEvent* event) override;
#else
	void enterEvent(QEvent* event) override;
#endif
		
		void leaveEvent(QEvent* event) override;

		DrawerWidget* getDrawerWidget();
	private:
		void initGui(DrawerWidget* dw = nullptr);
	private:
		ToolBarD *d;
	};

	class ToolBarDynamicD;
	class ORIGIN_UI_EXPORT ToolBarDynamic :public ToolBar {
		Q_OBJECT
	public:
		ToolBarDynamic(QWidget* parent = 0);
		ToolBarDynamic(DrawerWidget* dw, QWidget* parent = 0);
		~ToolBarDynamic();
		//初始化
		void init();

		//设置控件最大高度
		void setMaxHeight(const int& height);

		void pop() override;
		void push() override;

	public slots:
		void drawerWidgetMove(QPoint point);
	protected:
		void resizeEvent(QResizeEvent* event) override;
	private:
		ToolBarDynamicD* d;
	};
}

