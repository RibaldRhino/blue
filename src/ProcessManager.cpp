#include "ProcessManager.hpp"

void ProcessManager::AttachProcess(ProcessSPtr& processSPtr)
{
    _processList.push_back(processSPtr);

}

void ProcessManager::Update(double deltaTime)
{
    auto processIt = _processList.begin();
    while(processIt!=_processList.end())
    {
        auto& process = *processIt;
        if (!process->IsInitialized())
            process->VInit();
        if (process->IsAlive())
            process->VUpdate(deltaTime);
        if(process->IsDead())
        {
            if(process->Succeeded())
            {
                auto successors = process->GetSuccessors();
                for(auto& successor : successors)
                {
                    AttachProcess(successor);
                }
            }
            process->VEnd();
            _processList.erase(processIt++);
        }
    }
}
