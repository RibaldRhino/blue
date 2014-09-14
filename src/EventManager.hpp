#pragma once

#include <queue>
#include <map>
#include <boost/signals2.hpp>

#include "EventType.hpp"
#include "IEventData.hpp"
#include "IProcess.hpp"

class EventManager : IProcess
{
private:
    int _activeQueue;
    std::queue<IEventData> _eventQueue[2];
    // _event_queue[_active_queue] - dispatching is done from this queue.
    // _event_queue[(_active_queue+1)%2] - new events are stored here.
    std::map<EventType, boost::signals2::signal<void (IEventDataPtr)>> _eventBinding;

public:

    boost::signals2::connection AddListener(EventType& eventType, std::function<void (IEventDataPtr)> callback);
    void TriggerEvent(IEventDataPtr eventData);
    void QueueEvent(IEventDataPtr eventData);

    void VUpdate();
};