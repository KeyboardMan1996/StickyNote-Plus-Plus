#include "DiversityContent.h"
#include "NoteTextEdit.h"
#include "ToDoListContent.h"
NGui::DiversityContent::DiversityContent(QWidget* parent /*= 0*/)
{
	//添加新建代办事件项按钮
	{
		auto item = new QListWidgetItem();
		auto widget = new NoteTextEdit();
		item->setSizeHint(widget->sizeHint());
		addItem(item);
		setItemWidget(item, widget);
	}
	{
		auto item = new QListWidgetItem();
		auto widget = new ToDoListContent();
		item->setSizeHint(widget->sizeHint());
		addItem(item);
		setItemWidget(item, widget);
	}
}

NGui::DiversityContent::~DiversityContent()
{

}

void NGui::DiversityContent::loadNote(const std::shared_ptr<NF::StickyNote> note)
{

}

void NGui::DiversityContent::saveNote(const std::shared_ptr<NF::StickyNote> note)
{

}

void NGui::DiversityContent::connectStickyNote(StickyNote* stickyNote)
{

}

void NGui::DiversityContent::disConnectStickyNote(StickyNote* stickyNote)
{

}

bool NGui::DiversityContent::isEmpty()
{
	return true;
}

void NGui::DiversityContent::initGui(StickyNote* stickyNote)
{

}

