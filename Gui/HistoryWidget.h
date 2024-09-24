#pragma  once
#include <memory>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPoint>
#include <QAction>
#include <QListWidgetItem>
#include <map>
#include <QKeySequence>
#include "WidgetNoBoder.h"
#include "EventSender.h"
#include "OriginUI/NativeWindowTemplate.hpp"
class QxtGlobalShortcut;
namespace OriginUI{
	class NavigationBar;
	class InputLineEdit;
}
namespace NF {
	class StickyNote;
}
namespace NGui {
	//使用文本排序
	class ListWidgetItem :public QListWidgetItem{
	public:
		bool operator >(const QListWidgetItem& item);
		bool operator ==(const QListWidgetItem& item);
		bool operator <(const QListWidgetItem& item);
	};

	class HistoryList :public QListWidget {
		Q_OBJECT
	public:
		HistoryList(QWidget* parent = 0);
		~HistoryList() = default;
	protected:
		void wheelEvent(QWheelEvent* e);
	};

	class StickyNoteItem;
#ifdef _RUN_WITH_WIN7_
	class HistoryWidget :public OriginUI::WidgetNoBoder, public EventSender {
#else
	class HistoryWidget :public NativeWindowTemplate<QWidget>, public EventSender {
#endif // _RUN_WITH_WIN7_

		Q_OBJECT
	public:
		~HistoryWidget();
		HistoryWidget(const HistoryWidget&) = delete;
		HistoryWidget operator=(const HistoryWidget&) = delete;
		static std::shared_ptr<HistoryWidget> Getinstance();
		static void destoryInstance();
	private:
		HistoryWidget();
		static std::shared_ptr<HistoryWidget> instance;

	public Q_SLOTS:
		void actionTrigger(bool b);
		void serchEditButtonClicked(bool b);
		void listItemResize(QSize size);
#ifdef _RUN_WITH_WIN7_
		void  navigationBarMouseMove(QPoint);
#endif

	protected:
		void customEvent(QEvent* event);
		void paintEvent(QPaintEvent* event);
	private:
		void initGui();
		void intitShortcut();
	public:
		void addFileNote(const std::shared_ptr<NF::StickyNote> note);
		void removeFileNote(std::shared_ptr<NF::StickyNote> note);
		bool hasNote(std::shared_ptr<NF::StickyNote> note);
		void postEvent(QEvent* event);
		void setShortcut(const QKeySequence& key);
	private:
		OriginUI::NavigationBar* navigationBar;
		OriginUI::InputLineEdit* serchEdit;
		HistoryList* listWidget;
		QVBoxLayout* vLayout;
		QAction* actionClose,*actionNew,*actionSetting;
	private:
		using StickyNoteItemMap = std::map<std::string, StickyNoteItem*>;
		using QListWidgetItemMap = std::map<StickyNoteItem*, ListWidgetItem*>;

		StickyNoteItemMap noteItems;
		QListWidgetItemMap listItemMap;
		//全局快捷键
		QxtGlobalShortcut* shortcut;
	};

}