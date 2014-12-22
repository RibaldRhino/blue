#include <System/OpenGLSystem.hpp>
#include "Game.hpp"
#include "Process/Game/OutputProcess.hpp"
#include "Process/Game/EventDispatchProcess.hpp"
#include "Process/Game/InputProcess.hpp"
#include "Process/Game/WorldUpdateProcess.hpp"

namespace game {
    Game::Game() : _windowPtr(gamesystem::OpenGLSystem::getInstance().getWindow()) {
        _processManagerUPtr = std::unique_ptr<process::ProcessManager>(new process::ProcessManager());
        _worldUPtr = std::unique_ptr<World>(new World());
        _timerUPtr = std::unique_ptr<Timer>(new Timer());

        _processManagerUPtr->AttachProcess(std::unique_ptr<process::AbstractProcess>(new process::OutputProcess(this)));
        _processManagerUPtr->AttachProcess(std::unique_ptr<process::AbstractProcess>(new process::EventDispatchProcess(this)));
        _processManagerUPtr->AttachProcess(std::unique_ptr<process::AbstractProcess>(new process::WorldUpdateProcess(this)));
        _processManagerUPtr->AttachProcess(std::unique_ptr<process::AbstractProcess>(new process::InputProcess(this)));
    }

    void Game::Run() {
        _timerUPtr->Start();
        int i = 0;
        while (!_processManagerUPtr->Ended()) {
            _processManagerUPtr->Update(_timerUPtr->Delta());
        }
    }
}