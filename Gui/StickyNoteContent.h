#pragma  once
#include "NoteFile.h"
#include <memory.h>
#include <QWidget>
namespace NGui {
	class StickyNote;
	class StickyNoteContent {
	public:
		StickyNoteContent() = default;
		virtual ~StickyNoteContent() = default;
	public:
		virtual  void loadNote(const std::shared_ptr<NF::StickyNote> note) = 0;
		virtual  void saveNote(const std::shared_ptr<NF::StickyNote> note) = 0;
		virtual  void connectStickyNote(StickyNote* stickyNote) = 0;
		virtual	 void disConnectStickyNote(StickyNote* stickyNote) = 0;
		virtual	 void initGui(StickyNote* stickyNote) {};
		virtual  bool isEmpty() = 0;
	};

	class StickyNoteContentCreater {
	public:
		static StickyNoteContent* creatStickyNoteContent(const std::shared_ptr<NF::StickyNote> nfNote,QWidget *parent = 0);
	};
}