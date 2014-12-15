#include <log.hpp>
#include "WorldUpdateProcess.hpp"
#include "Game/Game.hpp"

namespace process {
    WorldUpdateProcess::WorldUpdateProcess(game::Game* game) : AbstractProcess() {
        _game = game;
        using std::placeholders::_1;
        event::EventManager::getInstance().AddListener(event::EventType::WINDOW_CLOSE, std::bind(&WorldUpdateProcess::OnWindowClose, this, _1));
    }

    void WorldUpdateProcess::VUpdate(double deltaTime) {
        _game->getWorld()->Update(deltaTime);
    }

    void WorldUpdateProcess::VInit() {
        _state = ProcessState::RUNNING;
    }

    void WorldUpdateProcess::VEnd() {
    }

    void WorldUpdateProcess::OnWindowClose(event::IEventDataSPtr &eventData) {
        _state = ProcessState::SUCCEEDED;
    }

    WorldUpdateProcess::~WorldUpdateProcess() {
        _onWindowClose.disconnect();
    }
}
