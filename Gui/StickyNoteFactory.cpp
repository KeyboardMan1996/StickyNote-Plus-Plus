#include "StickyNoteFactory.h"
#include "StickyNoteText.h"
#include "StickyNoteToDoList.h"
#include "StickyNoteDiversity.h"
#include <memory>
#include "StickyNotePool.h"

void NGui::StickyNoteCreator::registerAllCreator()
{
	auto pool = StickyNotePool::GetInstance();
	pool->registerStickyNoteCreator(new TextNoteCreator());
	pool->registerStickyNoteCreator(new ToDoListNoteCreator());
	pool->registerStickyNoteCreator(new DiversityNoteCreator());

}

NGui::StickyNote* NGui::TextNoteCreator::creatStickyNote()
{
	auto nfNote = std::make_shared<NF::StickyNoteText>();
	auto setting = Setting::GetInstance();
	nfNote->width = setting->stickyNoteWidth->getValue();
	nfNote->height = setting->stickyNoteHeight->getValue();
	StickyNotePool::toDeskCenter(nfNote);
	auto note = new NGui::StickyNoteText(nfNote);
	return note;
}

NGui::StickyNote* NGui::TextNoteCreator::creatStickyNote(std::shared_ptr<NF::StickyNote> nfNote)
{
	auto note = new NGui::StickyNoteText(nfNote);
	return note;
}

std::string NGui::TextNoteCreator::getType()
{
	return NF::StickyNoteText::getStickyNoteType();
}

NGui::StickyNote* NGui::ToDoListNoteCreator::creatStickyNote()
{
	auto nfNote = std::make_shared<NF::StickyNoteToDoList>();
	auto setting = Setting::GetInstance();
	nfNote->width = setting->stickyNoteWidth->getValue();
	nfNote->height = setting->stickyNoteHeight->getValue();
	StickyNotePool::toDeskCenter(nfNote);
	auto note = new NGui::StickyNoteToDoList(nfNote);
	return note;
}

NGui::StickyNote* NGui::ToDoListNoteCreator::creatStickyNote(std::shared_ptr<NF::StickyNote> nfNote)
{
	auto note = new NGui::StickyNoteToDoList(nfNote);
	return note;
}

std::string NGui::ToDoListNoteCreator::getType()
{
	return NF::StickyNoteToDoList::getStickyNoteType();
}

NGui::StickyNote* NGui::DiversityNoteCreator::creatStickyNote()
{
	return new StickyNoteDiversity(std::shared_ptr<NF::StickyNoteText>(new NF::StickyNoteText()));
}

std::string NGui::DiversityNoteCreator::getType()
{
	return "DiversityNote";
}

NGui::StickyNote* NGui::DiversityNoteCreator::creatStickyNote(std::shared_ptr<NF::StickyNote>)
{
	return nullptr;
}
