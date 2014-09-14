#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <list>

class AbstractProcess;
typedef std::shared_ptr<AbstractProcess> ProcessSPtr;
typedef std::list<ProcessSPtr> ProcessSPtrList;

class AbstractProcess : boost::noncopyable
{
public:
    enum class ProcessState
    {
        UNINITIALIZED,
        RUNNING,
        SUCCEEDED,
        FAILED,
        ABORTED,
    };

private:
    ProcessState _state;
    ProcessSPtrList _successors;

public:
    AbstractProcess();
    virtual ~AbstractProcess() { }
    virtual void VUpdate(double deltaTime) = 0;
    virtual void VInit() = 0;
    virtual void VEnd() = 0;

    ProcessSPtrList& GetSuccessors();
    void AddSuccessor(ProcessSPtr& process);

    bool Succeeded() const { return _state == ProcessState::SUCCEEDED; }
    bool IsInitialized() const { return _state == ProcessState::UNINITIALIZED; }
    bool IsAlive() const { return _state == ProcessState::RUNNING; }
    bool IsDead() const
    {
        return _state == ProcessState::SUCCEEDED ||
               _state == ProcessState::FAILED    ||
               _state == ProcessState::ABORTED;
    }
};