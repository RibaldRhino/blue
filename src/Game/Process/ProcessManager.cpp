#include "ProcessManager.hpp"

namespace process {

    void ProcessManager::AttachProcess(ProcessUPtr processUPtr) {
        _processList.push_back(std::move(processUPtr));

    }

    void ProcessManager::Update(double deltaTime) {
        auto processIt = _processList.begin();
        while (processIt != _processList.end()) {
            auto &process = *processIt;
            if (process->isUninitialized())
                process->VInit();
            if (process->isAlive())
                process->VUpdate(deltaTime);
            if (process->isDead()) {
                if (process->Succeeded()) {
                    auto &successors = process->GetSuccessors();
                    for (auto &successor : successors) {
                        AttachProcess(std::move(successor));
                    }
                }
                process->VEnd();
                _processList.erase(processIt++);
            }
            else
                processIt++;
        }
    }

    bool ProcessManager::Ended() {
        return _processList.empty();

    }
}