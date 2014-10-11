#include <log.hpp>
#include "EventDispatchProcess.hpp"
#include "InputProcess.hpp"
#include "Game/Game.hpp"

namespace process {

    EventDispatchProcess::EventDispatchProcess(game::Game *game) : AbstractProcess() {
        _game = game;
        using std::placeholders::_1;
        event::EventManager::get().AddListener(event::EventType::WINDOW_CLOSE, std::bind(&EventDispatchProcess::OnWindowClose, this, _1));
    }
    void EventDispatchProcess::VUpdate(double deltaTime) {
        auto& eventManager = event::EventManager::get();
        eventManager.Update(deltaTime);
    }

    void EventDispatchProcess::VInit() {
        _state = ProcessState::RUNNING;
    }

    void EventDispatchProcess::VEnd() {
    }

    void EventDispatchProcess::OnWindowClose(event::IEventDataSPtr &eventData) {
        _state = ProcessState::SUCCEEDED;
    }

    EventDispatchProcess::~EventDispatchProcess() {
        _onWindowClose.disconnect();
    }
}