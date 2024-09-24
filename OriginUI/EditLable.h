#pragma  once
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "ExportConfig.hpp"
namespace OriginUI {
	class EditLableD;
	//创建一个鼠标双击时可以编辑的lable
	class ORIGIN_UI_EXPORT EditLable :public QWidget {
		Q_OBJECT
	public:
		EditLable(QWidget* parnet = 0);
		~EditLable();

		void setText(const QString& text);
		QString getText() const;
	public:
		void showLabel();
		void showLineEdit();
	signals:
		void gotFocus();
		void lostFocus();
	public slots:
		void lableDoubleClicked();
		void editOkButtonClicked(bool);
		void editCancelButtonClicked(bool);
		void lineEditOk(bool);
		void editingFinished();
	private:
		EditLableD* d;
	};

	//一个可以发出鼠标双击事件的lable
	class ORIGIN_UI_EXPORT ClickedLable :public QLabel {
		Q_OBJECT
	public:
		ClickedLable(QWidget* parent = 0);
		~ClickedLable() = default;
	Q_SIGNALS:
		void mouseDoubleClick();
	protected:
		void mouseDoubleClickEvent(QMouseEvent* event);

	};

	//一个可以发出鼠标离开事件的lineEdit
	class ORIGIN_UI_EXPORT LineEditLeave :public QLineEdit {
		Q_OBJECT
	public:
		LineEditLeave(QWidget* parent = 0);
		~LineEditLeave() = default;
	protected:
		void leaveEvent(QEvent* event) override;
	signals:
		void mouseLeave();
	};

}