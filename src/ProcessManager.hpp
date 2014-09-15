#pragma once

#include <list>
#include "AbstractProcess.hpp"

class ProcessManager
{
private:
    ProcessUPtrList _processList;
public:
    void Update(double deltaTime);
    void AttachProcess(ProcessUPtr processUPtr);
    bool Ended();
};