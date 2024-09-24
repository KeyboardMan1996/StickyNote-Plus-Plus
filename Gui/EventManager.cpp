#include "EventManager.h"
#include "EventSender.h"
std::shared_ptr<NGui::EventManager> NGui::EventManager::instance;

void NGui::EventManager::rgisterSender(EventSender* s)
{
	if (s->isRegister)
		return;
	s->isRegister = true;
	senders.push_back(s);
}

void NGui::EventManager::remove(EventSender* s)
{
	if (!s->isRegister)
		return;
	for (auto iter = senders.begin(); iter != senders.end(); iter++)
	{
		if (*iter != s)
			continue;
		senders.erase(iter);
		break;
	}
}

NGui::EventManager::~EventManager()
{

}

NGui::EventManager::EventManager(QObject* parent /*= 0*/)
	:QObject(parent)
{

} 

void NGui::EventManager::postEvent(QEvent* event)
{
	auto ins = GetInstance();
	for (auto sender : ins->senders)
	{
		sender->postEvent(event);
	}
}

