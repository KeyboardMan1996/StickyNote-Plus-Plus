#include "StickyNoteToDoList.h"
#include "ToDoListContent.h"
#include "LogTool.h"
NGui::StickyNoteToDoList::StickyNoteToDoList(std::shared_ptr<NF::StickyNote> note, QWidget* parent /*= 0*/)
	:StickyNote(note, new ToDoListContent(),parent)
{
	setBottomToolbarHide(false);
	initActionCommand();
}

NGui::StickyNoteToDoList::~StickyNoteToDoList()
{

}

void NGui::StickyNoteToDoList::addItem(const QString& text)
{
	auto toDoListContent = dynamic_cast<ToDoListContent*>(getContent());
	PtrIsNull(toDoListContent);
	toDoListContent->addToDoListItem(text);
	saveNote();
}

void NGui::StickyNoteToDoList::initActionCommand()
{
	//³õÊ¼»¯°´Å¥
	initActionCommandToTop(ActionCommandTypes("cmd_new_note", true));
	//initActionCommandToTop(ActionCommandTypes("cmd_new_copy_style", true));
	initActionCommandToTop(ActionCommandTypes("cmd_color_setting", true));
	initActionCommandToTop(ActionCommandTypes("cmd_show_history", true));
	initActionCommandToTop(ActionCommandTypes("cmd_pin_icon_layer", true));
	initActionCommandToTop(ActionCommandTypes("cmd_pin_top", true));
	initActionCommandToTop(ActionCommandTypes("cmd_close", true));

#if 0
	initActionCommandToBottom(ActionCommandTypes("cmd_new_to_do_list", true));
	initActionCommandToBottom(ActionCommandTypes("cmd_to_do_list_split", true));
#endif
	initActionCommandToBottom(ActionCommandTypes("cmd_to_do_list_edit", true));
}

void NGui::StickyNoteToDoList::leaveEvent(QEvent* event)
{
	StickyNote::leaveEvent(event);
	contentLostFocus();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void NGui::StickyNoteToDoList::enterEvent(QEnterEvent* event)
#else
void NGui::StickyNoteToDoList::enterEvent(QEvent* event)
#endif
{
	StickyNote::enterEvent(event);
	setTopToolbarAutoPush(false);
	setBottomToolbarAutoPush(false);
	contentGotFocus();
}
