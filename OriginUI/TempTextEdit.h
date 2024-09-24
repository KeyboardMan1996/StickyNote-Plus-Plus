#pragma once
#include <QWidget>
#include "ExportConfig.hpp"
namespace OriginUI {
	class TempTextEditD;
	//提供一个临时的文本编辑器，编辑器内含有一个确定和取消的按钮
	class ORIGIN_UI_EXPORT TempTextEdit :public QWidget{
		Q_OBJECT
	public:
		TempTextEdit(QWidget* parent = 0);
		~TempTextEdit();

	public:
		void setText(const QString& text);
		QString getText() const;
	public Q_SLOTS:
		void toolButtonClicked(bool);

	Q_SIGNALS:
		void okButtonClicked(bool);
		void cancelButtonClicked(bool);

	protected:
		void leaveEvent(QEvent* event);

	private:
		TempTextEditD* d;
	};
}
