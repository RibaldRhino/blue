#include "AbstractProcess.hpp"

AbstractProcess::AbstractProcess()
{
    _state = ProcessState::UNINITIALIZED;
}

ProcessUPtrList& AbstractProcess::GetSuccessors()
{
    return _successors;
}

void AbstractProcess::AddSuccessor(ProcessUPtr process)
{
    _successors.push_back(std::move(process));
}
