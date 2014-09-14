#pragma once

#include <list>
#include "AbstractProcess.hpp"

class ProcessManager
{
private:
    ProcessSPtrList _processList;
public:
    void Update(double deltaTime);
    void AttachProcess(ProcessSPtr processSPtr);
};