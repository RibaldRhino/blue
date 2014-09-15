#pragma once

#include <queue>
#include <map>
#include <boost/signals2.hpp>

#include "EventType.hpp"
#include "IEventData.hpp"
#include "AbstractProcess.hpp"

class EventManager
{
private:
    int _activeQueue;
    std::queue<IEventDataSPtr> _eventQueue[2];
    // _event_queue[_active_queue] - new events are stored here.
    // _event_queue[(_active_queue+1)%2] - dispatching is done from this queue.
    std::map<EventType, boost::signals2::signal<void (IEventDataSPtr)>> _eventBinding;

private:
    void switchActiveQueue();

public:
    boost::signals2::connection AddListener(EventType& eventType, std::function<void (IEventDataSPtr &)>& callback);
    void TriggerEvent(IEventDataSPtr eventData);
    void QueueEvent(IEventDataSPtr eventData);

    void Update(double deltaTime);
};