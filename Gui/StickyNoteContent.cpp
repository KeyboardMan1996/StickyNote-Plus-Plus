#include "StickyNoteContent.h"
#include "ToDoListContent.h"
#include "NoteTextEdit.h"
#include "ToDoListContent.h"
NGui::StickyNoteContent* NGui::StickyNoteContentCreater::creatStickyNoteContent(const std::shared_ptr<NF::StickyNote> nfNote, QWidget* parent)
{
	if (nfNote->stickyNoteType.getValue() == "stickyNoteText")
	{
		return new NoteTextEdit(parent);
	}else if (nfNote->stickyNoteType.getValue() == "contentToDoList")
	{
		return  new ToDoListContent(parent);
	}
	else {
		return new NoteTextEdit(parent);
	}
}

