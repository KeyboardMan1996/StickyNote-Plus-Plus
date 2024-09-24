#pragma once
#include <memory.h>
#include "StickyNoteFile/NoteFile.h"
#include "NoteTextEdit.h"
#include "ToDoListContent.h"
namespace NGui {
	class DiversityItem {
	public:
		virtual  void load(const std::shared_ptr<NF::StickyNote> note)  = 0;
		virtual  void save(const std::shared_ptr<NF::StickyNote> note) = 0;
	};

}