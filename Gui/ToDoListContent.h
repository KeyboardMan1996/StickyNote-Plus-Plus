#pragma  once
#include <QListWidget>
#include <QObject>
#include "StickyNoteContent.h"
#include <qlistwidget.h>
#include <QList>
namespace NF {
	class StickyNoteToDoListItem;
}
namespace NGui {
	class ToDoListContentD;
	class ToDoListContent :public QListWidget,public StickyNoteContent{
		Q_OBJECT
	public:
		ToDoListContent(QWidget* parent = 0);
		~ToDoListContent();

		//设置是否处于编辑状态
		void setEditState(const bool& state);
		bool getEditState();
	public:
		virtual  void loadNote(const std::shared_ptr<NF::StickyNote> note) override;
		virtual  void saveNote(const std::shared_ptr<NF::StickyNote> note) override;
		virtual  void connectStickyNote(StickyNote* stickyNote) override;
		virtual	 void disConnectStickyNote(StickyNote* stickyNote) override;
		virtual  bool isEmpty() override;
		virtual  void initGui(StickyNote* stickyNote) override;

	protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		void enterEvent(QEnterEvent* event) override;
#else
		void enterEvent(QEvent* event) override;
#endif
		void leaveEvent(QEvent* event);
	public:
		void addToDoListItem();
		void addToDoListItem(NF::StickyNoteToDoListItem& data);
		void addToDoListItem(const QString& text);
		//获取item的数量
		int getToDoListCount();
		//获取item的大小
		QSize getToDoListSize();
		//获取文本
		QList<QString> getToDOListText();
		//获取 设置 分类显示的状态 
		void setSplit(const bool& b);
		bool getSplit();
	public slots:
		void deleteItem();
		void addItemButtonClicked();
	Q_SIGNALS:
		void lostFocus();
		void gotFocus();
	private:
		//按照是否完成分类
		void splitItem();
		//取消分类
		void cancelSplitItem();
		//移除所有item 并不释放内存
		void removeAllItem();
	private:
		ToDoListContentD* d;
	};
};