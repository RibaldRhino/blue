#include "EventManager.hpp"

boost::signals2::connection EventManager::AddListener(EventType& eventType, std::function<void (IEventDataPtr)> callback)
{

}

void EventManager::TriggerEvent(IEventDataPtr eventData) {

}

void EventManager::QueueEvent(IEventDataPtr data) {

}

void EventManager::VUpdate() {

}
