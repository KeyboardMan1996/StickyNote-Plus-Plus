#pragma once
#include "StickyNote.h"
namespace NGui {
	class StickyNoteText :public StickyNote {
	public:
		StickyNoteText(std::shared_ptr<NF::StickyNote> note, QWidget* parent = 0);
		~StickyNoteText();
	public:
		void setText(const QString& text);
		void setHtml(const QString& html);
		virtual void loadSetting() override;
	protected:
		void initActionCommand() override;
	};
}