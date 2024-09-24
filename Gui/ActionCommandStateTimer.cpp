#include "ActionCommandStateTimer.h"
#include "StickyNotePool.h"
#include "StickyNote.h"
NGui::ActionCommandStateTimer::ActionCommandStateTimer()
{
	timer.start(500);
	connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

NGui::ActionCommandStateTimer::~ActionCommandStateTimer()
{

}

void NGui::ActionCommandStateTimer::timeout()
{
	auto pool = StickyNotePool::GetInstance();
	for (auto note : pool->pool)
	{
		note->updateCmdState();
	}
}

