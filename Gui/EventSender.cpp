#include "EventSender.h"
#include "EventManager.h"
NGui::EventSender::EventSender()
	:isRegister(false)
{

}

NGui::EventSender::~EventSender()
{
	if (!isRegister)
		return;
	auto manager = EventManager::GetInstance();
	manager->remove(this);
}

