#include <log.hpp>
#include "OutputProcess.hpp"
#include "Game/Game.hpp"

namespace process {
    OutputProcess::OutputProcess(game::Game* game) : AbstractProcess() {
        _game = game;
        using std::placeholders::_1;
        event::EventManager::getInstance().AddListener(event::EventType::WINDOW_CLOSE, std::bind(&OutputProcess::OnWindowClose, this, _1));
    }

    void OutputProcess::VUpdate(double deltaTime) {
        _game->getWindow()->Clear();
        _game->getWorld()->Render();
        _game->getWindow()->SwapBuffers();
    }

    void OutputProcess::VInit() {
        _state = ProcessState::RUNNING;
    }

    void OutputProcess::VEnd() {
    }

    void OutputProcess::OnWindowClose(event::IEventDataSPtr &eventData) {
        _state = ProcessState::SUCCEEDED;
    }

    OutputProcess::~OutputProcess() {
        _onWindowClose.disconnect();
    }
}