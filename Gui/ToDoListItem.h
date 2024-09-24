#pragma  once
#include <QWidget>
#include <QDateTime>
#include "NoteFile.h"
namespace Ui {
	class ToDoListItem;
}
namespace NGui {
	class ToDoListItemD;
	class ToDoListItem :public QWidget {
		Q_OBJECT
	public:
		ToDoListItem(QWidget* parent = 0);
		~ToDoListItem();

	signals:
		void deleteItem();
		void gotFocus();
		void lostFocus();
	public:
		//读取和生成数据
		void setData(NF::StickyNoteToDoListItem data);
		NF::StickyNoteToDoListItem getData();
		//获取文本
		QString getText() const;
		void setText(const QString& text);

		QDateTime getCreatDateTime();
		QDateTime getFinishedDateTime();
		bool isFinished();

		//设置 和  取消 编辑状态
		void showEditButton();
		void hideEditButton();

	public slots:
		void on_toolButtonIsFInish_triggered(QAction* action);
		void on_toolButtonDelete_triggered(QAction* action);
		void on_toolButtonDate_triggered(QAction* action);

	private:
		void updateLableDate();
	private:
		Ui::ToDoListItem* ui;
		ToDoListItemD* d;
	};

	class ToDoListAddItemWidget :public QWidget {
		Q_OBJECT
	public:
		ToDoListAddItemWidget(QWidget* parent = 0);
		~ToDoListAddItemWidget();
	signals:
		void clicked();
	};
}