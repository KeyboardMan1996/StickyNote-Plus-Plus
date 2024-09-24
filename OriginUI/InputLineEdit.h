#pragma once
#include "ExportConfig.hpp"
#include <QWidget>
#include <QLineEdit>
class QToolButton;

namespace OriginUI {

	//提供一个可以发送焦点状态信号的lineEidt
	class InputLineEditFocus :public QLineEdit {
		Q_OBJECT
	Q_SIGNALS:
		void gotFocus();
		void lostFocus();
	protected:
		void focusInEvent(QFocusEvent* e);
		void focusOutEvent(QFocusEvent* e);

	};


	enum InputLineEditLayoutMod {
		BUTTON_LEFT = 0,		//按钮在左边		
		BUTTON_RIGHT		//按钮在右边
	};

	class InputLineEditD;
	class ORIGIN_UI_EXPORT InputLineEdit:public QWidget {
		Q_OBJECT
	public:
		InputLineEdit(QWidget *parent = 0);
		~InputLineEdit();

		//设置布局mod
		void setButtonLaoutMod(const InputLineEditLayoutMod& mod);
		InputLineEditLayoutMod getButtonLaoutMod();

		//设置按钮图标
		void setOkIcon(QIcon icon);
		void setCancelIcon(QIcon icon);

		//获取按钮指针
		QToolButton* getOkButton();
		QToolButton* getCancelButton();
		//获取编辑器
		QLineEdit* getLineEdit();
		//是否自动显示取消按钮
		void setAutoShowCancelButton(const bool& b);
		bool getAutoShowCancelButton();
	public Q_SLOTS:
		void textChanged(const QString& text);
	Q_SIGNALS:
		void gotFocus();
		void lostFocus();
	protected:
		void paintEvent(QPaintEvent* event);
		void keyPressEvent(QKeyEvent* event);
	private:
		InputLineEditD* d;

	};

}