#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <list>

class AbstractProcess;
typedef std::shared_ptr<AbstractProcess> ProcessSPtr;
typedef std::list<ProcessSPtr> ProcessSPtrList;
typedef std::unique_ptr<AbstractProcess> ProcessUPtr;
typedef std::list<ProcessUPtr> ProcessUPtrList;

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

protected:
    ProcessState _state;
    ProcessUPtrList _successors;

public:
    AbstractProcess();
    virtual ~AbstractProcess() { }
    virtual void VUpdate(double deltaTime) = 0;
    virtual void VInit() = 0;
    virtual void VEnd() = 0;

    ProcessUPtrList& GetSuccessors();
    void AddSuccessor(ProcessUPtr process);

    bool Succeeded() const { return _state == ProcessState::SUCCEEDED; }
    bool isUninitialized() const { return _state == ProcessState::UNINITIALIZED; }
    bool isAlive() const { return _state == ProcessState::RUNNING; }
    bool isDead() const
    {
        return _state == ProcessState::SUCCEEDED ||
               _state == ProcessState::FAILED    ||
               _state == ProcessState::ABORTED;
    }
};