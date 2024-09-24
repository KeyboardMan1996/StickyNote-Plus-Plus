#pragma  once
#include <memory>
#include "StickyNoteFile/NoteFile.h"
#include <QTextEdit>
#include <QSize>
#include "ToDoListContent.h"
namespace NGui {
	class StickyNoteItemEventFilter;
	class StickyNoteItemContent {
	public:
		StickyNoteItemContent() = default;
		virtual ~StickyNoteItemContent() =default;
		//用于载入页面内容
		virtual void loadContent(const std::shared_ptr<NF::StickyNote> nfNote) = 0;
		//用于限制页面高度
		virtual QSize getMiniSize() = 0;
		//用于实现搜索功能
		virtual QString getContentText() = 0;
	};

	class StickyNoteItemContentCraeter {
	public:
		static StickyNoteItemContent* creatStickyNoteItem(const std::shared_ptr<NF::StickyNote> nfNote);

	};

	class StickyNoteItemTextContent :public QTextEdit, public StickyNoteItemContent {
	public:
		StickyNoteItemTextContent(QWidget* parent = 0);
		~StickyNoteItemTextContent();

		virtual void loadContent(const std::shared_ptr<NF::StickyNote> nfNote) override;
		virtual QSize getMiniSize() override;
		virtual QString getContentText() override;
	private:
		StickyNoteItemEventFilter *filter;
	protected:
		void resizeEvent(QResizeEvent* e);
	};

	class StickyNoteItemItemToDoList :public ToDoListContent, public StickyNoteItemContent {
	public:
		StickyNoteItemItemToDoList(QWidget* parent = 0);
		~StickyNoteItemItemToDoList();
		virtual void loadContent(const std::shared_ptr<NF::StickyNote> nfNote) override;
		virtual QSize getMiniSize() override;
		virtual QString getContentText() override;

	private:
		StickyNoteItemEventFilter* filter;
	};
}