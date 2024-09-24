#pragma once
#include "StickyNote.h"
#include <memory>
namespace NGui {
	class StickyNoteToDoList :public StickyNote {
	public:
		StickyNoteToDoList(std::shared_ptr<NF::StickyNote> note, QWidget* parent = 0);
		~StickyNoteToDoList();
	public:
		void addItem(const QString& text);

	protected:
		void initActionCommand() override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		void enterEvent(QEnterEvent* event) override;
#else
		void enterEvent(QEvent* event) override;
#endif
		void leaveEvent(QEvent* event) override;
	};
}