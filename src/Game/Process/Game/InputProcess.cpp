#include <log.hpp>
#include "InputProcess.hpp"
#include "Game/Game.hpp"

namespace process {
    InputProcess::InputProcess(game::Game* game) : AbstractProcess() {
        _game = game;

        using std::placeholders::_1;
        event::EventManager::getInstance().AddListener(event::EventType::WINDOW_CLOSE, std::bind(&InputProcess::OnWindowClose, this, _1));
    }

    void InputProcess::VUpdate(double deltaTime) {
        _game->getWindow()->PollEvents();
    }

    void InputProcess::VInit() {
        _state = ProcessState::RUNNING;
    }

    void InputProcess::VEnd() {

    }

    void InputProcess::OnWindowClose(event::IEventDataSPtr &eventData) {
        _state = ProcessState::SUCCEEDED;
    }

    InputProcess::~InputProcess() {
        _onWindowClose.disconnect();
    }
}