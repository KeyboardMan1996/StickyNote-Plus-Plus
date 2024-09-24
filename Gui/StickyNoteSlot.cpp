#include "StickyNoteSlot.h"
#include "StickyNote.h"
#include "ActionCommand.h"
#include <iostream>

NGui::StickyNoteSlot::StickyNoteSlot()
	:note(nullptr)
{

}

void NGui::StickyNoteSlot::connectSlot(ActionCommand* action)
{
	connect(action, SIGNAL(triggered(bool)), this, SLOT(actionTrigger(bool)));
}

void NGui::StickyNoteSlot::setStickyNote(StickyNote* note)
{
	this->note = note;
}

void NGui::StickyNoteSlot::actionTrigger(bool check)
{
	if (!note)
	{
		std::cerr << "NGui::StickyNoteSlot note is nullptr!" << std::endl;
		return;
	}
		
	auto action = dynamic_cast<ActionCommand*>(sender());
	if (!action)
	{
		std::cerr << "NGui::StickyNoteSlot action isn't ActionCommand!" << std::endl;
		return;
	}
		
	action->active(note, check);
}

