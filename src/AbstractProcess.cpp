#include "AbstractProcess.hpp"

AbstractProcess::AbstractProcess()
{
    _state = ProcessState::UNINITIALIZED;
}

ProcessSPtrList& AbstractProcess::GetSuccessors()
{
    return _successors;
}

void AbstractProcess::AddSuccessor(ProcessSPtr &process)
{
    _successors.push_back(process);
}
