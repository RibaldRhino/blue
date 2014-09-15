#include "EventManager.hpp"

boost::signals2::connection EventManager::AddListener(EventType& eventType, std::function<void (IEventDataPtr&)>& callback)
{
    return std::move(_eventBinding[eventType].connect(callback));
}

void EventManager::TriggerEvent(IEventDataPtr& eventData)
{
    if(_eventBinding.find(eventData->VGetEventType()) != _eventBinding.end())
        _eventBinding[eventData->VGetEventType()](eventData);
}

void EventManager::QueueEvent(IEventDataPtr& data)
{
    _eventQueue[_activeQueue].push(data);
}

void EventManager::Update(double deltaTime)
{
    auto dispatchQueue = _activeQueue;
    switchActiveQueue();
    while(!_eventQueue[dispatchQueue].empty())
    {
        auto eventData = _eventQueue[dispatchQueue].front();
        TriggerEvent(eventData);
        _eventQueue[dispatchQueue].pop();
    }
}

void EventManager::switchActiveQueue()
{
    _activeQueue = ( _activeQueue + 1) %2;
}
